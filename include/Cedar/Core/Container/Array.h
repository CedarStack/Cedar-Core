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

#include <Cedar/Core/OutOfRangeException.h>

namespace Cedar::Core::Container {
    template<typename T, Size initialCapacity = 1>
    class Array {
    private:
        T* m_data;
        Size m_size;
        Size m_capacity;
        Threading::Mutex m_mtx;

        void resize(Size newCapacity) {
            T* newData = new T[newCapacity];
            if (m_data) {
                Memory::copy(newData, m_data, m_size * sizeof(T));
                delete[] m_data;
            }
            m_data = newData;
            m_capacity = newCapacity;
        }

    public:
        Array() : m_data(new T[initialCapacity]), m_size(0), m_capacity(initialCapacity) {}

        Array(const Array& other) {
            m_mtx.lock();
            m_data = new T[other.m_capacity];
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            Memory::copy(m_data, other.m_data, m_size * sizeof(T));
            m_mtx.unlock();
        }

        Array& operator=(const Array& other) {
            if (this != &other) {
                m_mtx.lock();
                resize(other.m_capacity);
                m_size = other.m_size;
                Memory::copy(m_data, other.m_data, m_size * sizeof(T));
                m_mtx.unlock();
            }
            return *this;
        }

        ~Array() {
            delete[] m_data;
        }

        void append(const T& value) {
            m_mtx.lock();
            if (m_size == m_capacity) {
                resize(m_capacity * 2);  // Double the capacity when needed
            }
            m_data[m_size++] = value;
            m_mtx.unlock();
        }

        bool remove(const T& value) {
            m_mtx.lock();
            for (Size i = 0; i < m_size; i++) {
                if (m_data[i] == value) {
                    Memory::copy(m_data + i, m_data + i + 1, (m_size - i - 1) * sizeof(T));
                    m_size--;
                    m_mtx.unlock();
                    return true;
                }
            }
            m_mtx.unlock();
            return false;
        }

        void clear() {
            m_mtx.lock();
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
            m_mtx.unlock();
        }

        [[nodiscard]] Size size() const {
            return m_size;
        }

        T& operator[](Size index) {
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        const T& operator[](Size index) const {
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }
    };
}
