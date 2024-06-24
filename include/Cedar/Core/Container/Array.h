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
        T* m_data;
        Size m_size;
        Size m_capacity;
        Memory::Allocator<T> m_allocator;
        mutable Threading::Mutex m_mtx;

        void resize(Size newCapacity) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            T* newData = m_allocator.allocate(newCapacity);
            for (Index i = 0; i < m_size; ++i) {
                m_allocator.construct(newData + i, Memory::move(m_data[i]));
                m_allocator.destroy(m_data + i);
            }
            m_allocator.deallocate(m_data);
            m_data = newData;
            m_capacity = newCapacity;
        }

    public:
        Array() : m_data(nullptr), m_size(0), m_capacity(0) {}

        explicit Array(Size initialCapacity) : m_size(0), m_capacity(initialCapacity) {
            m_data = m_allocator.allocate(m_capacity);
        }

        Array(T* src, Size length) : m_size(length), m_capacity(length) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            m_data = m_allocator.allocate(m_capacity);
            for (Size i = 0; i < m_size; ++i) {
                m_allocator.construct(m_data + i, Memory::move(src[i]));
            }
        }

        Array(const Array& other) {
            Threading::LockGuard<Threading::Mutex> lock(other.m_mtx);
            m_data = m_allocator.allocate(other.m_capacity);
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            for (Size i = 0; i < m_size; ++i) {
                m_allocator.construct(m_data + i, other.m_data[i]);
            }
        }

        template<typename Iterable>
        Array(const Iterable& other) : m_size(other.size()), m_capacity(other.size()) {
            m_data = m_allocator.allocate(m_capacity);
            Size index = 0;
            for (const auto& item : other) {
                m_allocator.construct(m_data + index++, item);
            }
        }

        Array(std::initializer_list<T> initList) : m_size(initList.size()), m_capacity(initList.size()) {
            m_data = m_allocator.allocate(m_capacity);
            Index i = 0;
            for (const T& value : initList) {
                m_allocator.construct(m_data + i++, value);
            }
        }

        Array& operator=(const Array& other) {
            if (this != &other) {
                Threading::LockGuard<Threading::Mutex> lock_this(m_mtx), lock_other(other.m_mtx);
                resize(other.m_capacity);
                for (Index i = 0; i < other.m_size; ++i) {
                    m_allocator.construct(m_data + i, other.m_data[i]);
                }
                m_size = other.m_size;
            }
            return *this;
        }

        ~Array() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            for (Index i = 0; i < m_size; ++i) {
                m_allocator.destroy(m_data + i);
            }
            m_allocator.deallocate(m_data);
        }

        void append(const T& value) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (m_size == m_capacity) {
                resize(m_capacity == 0 ? 1 : m_capacity * 2);
            }
            m_allocator.construct(m_data + m_size++, value);
        }

        bool remove(const T& value) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            for (Index i = 0; i < m_size; i++) {
                if (m_data[i] == value) {
                    m_allocator.destroy(m_data + i);
                    for (Index j = i; j < m_size - 1; ++j) {
                        m_allocator.construct(m_data + j, Memory::move(m_data[j + 1]));
                        m_allocator.destroy(m_data + j + 1);
                    }
                    --m_size;
                    return true;
                }
            }
            return false;
        }

        void clear() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            for (Index i = 0; i < m_size; ++i) {
                m_allocator.destroy(m_data + i);
            }
            m_allocator.deallocate(m_data);
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }

        [[nodiscard]] Index size() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_size;
        }

        [[nodiscard]] T* data() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data;
        }

        T& operator[](Index index) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        const T& operator[](Index index) const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        class Iterator {
        private:
            T* m_ptr;
        public:
            explicit Iterator(T* ptr) : m_ptr(ptr) {}

            Iterator& operator++() {
                ++m_ptr;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                ++m_ptr;
                return tmp;
            }

            T& operator*() const {
                return *m_ptr;
            }

            T* operator->() const {
                return m_ptr;
            }

            bool operator==(const Iterator& other) const {
                return m_ptr == other.m_ptr;
            }

            bool operator!=(const Iterator& other) const {
                return m_ptr != other.m_ptr;
            }
        };

        Iterator begin() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return Iterator(m_data);
        }

        Iterator end() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return Iterator(m_data + m_size);
        }

        class ConstIterator {
        private:
            const T* m_ptr;
        public:
            explicit ConstIterator(const T* ptr) : m_ptr(ptr) {}

            ConstIterator& operator++() {
                ++m_ptr;
                return *this;
            }

            ConstIterator operator++(int) {
                ConstIterator tmp = *this;
                ++m_ptr;
                return tmp;
            }

            const T& operator*() const {
                return *m_ptr;
            }

            const T* operator->() const {
                return m_ptr;
            }

            bool operator==(const ConstIterator& other) const {
                return m_ptr == other.m_ptr;
            }

            bool operator!=(const ConstIterator& other) const {
                return m_ptr != other.m_ptr;
            }
        };

        ConstIterator begin() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return ConstIterator(m_data);
        }

        ConstIterator end() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return ConstIterator(m_data + m_size);
        }
    };
}
