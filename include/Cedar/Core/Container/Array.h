/*
 * Project: Cedar-Core
 * Copyright (C) 2024 Cedar Community
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <Cedar/Core/BasicTypes.h>
#include <Cedar/Core/Memory.h>
#include <Cedar/Core/Threading/Mutex.h>
#include <Cedar/Core/Threading/LockGuard.h>
#include <Cedar/Core/Exceptions/OutOfRangeException.h>
#include <initializer_list>

namespace Cedar::Core::Container {
    template<typename T>
    class Array {
    private:
        Memory::UniquePointer<T[]> m_data;
        Size m_size;
        Size m_capacity;
        mutable Threading::Mutex m_mtx;

    public:
        Array() : m_size(0), m_capacity(0) {}

        explicit Array(Size initialCapacity) : m_size(0), m_capacity(initialCapacity) {
            m_data.reset(new T[m_capacity]);
        }

        Array(const Array& other) {
            Threading::LockGuard<Threading::Mutex> lock(other.m_mtx);
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            T* newData = new T[m_capacity];
            for (Size i = 0; i < m_size; ++i) {
                newData[i] = other.m_data[i];
            }
            m_data.reset(newData);
        }

        Array(std::initializer_list<T> initList) : m_size(initList.size()), m_capacity(initList.size()) {
            T* newData = new T[m_capacity];
            Size i = 0;
            for (const T& value : initList) {
                newData[i++] = value;
            }
            m_data.reset(newData);
        }

        Array(T* src, Size length) : m_size(length), m_capacity(length) {
            m_data.reset(new T[m_capacity]);
            for (Size i = 0; i < m_size; ++i) {
                m_data[i] = src[i];
            }
        }

        ~Array() = default;

        [[nodiscard]] Size size() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_size;
        }

        [[nodiscard]] T* data() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data.get();
        }

        Array& operator=(const Array& other) = delete;

        T& operator[](Size index) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        const T& operator[](Size index) const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        using Iterator = T*;
        using ConstIterator = const T*;

        Iterator begin() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data.get();
        }

        Iterator end() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data.get() + m_size;
        }

        ConstIterator begin() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data.get();
        }

        ConstIterator end() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data.get() + m_size;
        }
    };
}
