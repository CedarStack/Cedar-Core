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
    class ArrayList {
    private:
        Memory::UniquePointer<T[]> m_data;
        Size m_size;
        Size m_capacity;
        Memory::Allocator<T> m_allocator;
        mutable Threading::Mutex m_mtx;

        void resizeInternal(Size newCapacity) {
            T *newData = m_allocator.allocate(newCapacity);
            for (Size i = 0; i < m_size; ++i) {
                m_allocator.construct(newData + i, TypeTraits::move(m_data[i]));
                m_allocator.destroy(m_data.get() + i);
            }
            m_allocator.deallocate(m_data.release());
            m_data.reset(newData);
            m_capacity = newCapacity;
        }

    public:
        ArrayList() : m_size(0), m_capacity(0) {}

        explicit ArrayList(Size initialCapacity) : m_size(0), m_capacity(initialCapacity) {
            m_data.reset(m_allocator.allocate(m_capacity));
        }

        ArrayList(const ArrayList &other) {
            Threading::LockGuard<Threading::Mutex> lock(other.m_mtx);
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_data.reset(m_allocator.allocate(m_capacity));
            for (Size i = 0; i < m_size; ++i) {
                m_allocator.construct(m_data.get() + i, other.m_data[i]);
            }
        }

        ArrayList &operator=(const ArrayList &other) {
            if (this != &other) {
                Threading::LockGuard<Threading::Mutex> lock_this(m_mtx), lock_other(other.m_mtx);
                T *newData = m_allocator.allocate(other.m_capacity);
                for (Size i = 0; i < other.m_size; ++i) {
                    m_allocator.construct(newData + i, other.m_data[i]);
                }
                m_allocator.deallocate(m_data.release());
                m_data.reset(newData);
                m_capacity = other.m_capacity;
                m_size = other.m_size;
            }
            return *this;
        }

        ~ArrayList() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            for (Size i = 0; i < m_size; ++i) {
                m_allocator.destroy(m_data.get() + i);
            }
            m_allocator.deallocate(m_data.release());
        }

        void append(const T &value) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (m_size == m_capacity) {
                resizeInternal(m_capacity == 0 ? 1 : m_capacity * 2);
            }
            m_allocator.construct(m_data.get() + m_size++, value);
        }

        Boolean remove(const T &value) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            for (Size i = 0; i < m_size; i++) {
                if (m_data[i] == value) {
                    removeAt(i);
                    return true;
                }
            }
            return false;
        }

        void insertAt(Size index, const T &value) {
            if (index > m_size) {
                throw OutOfRangeException("Index out of range");
            }
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (m_size == m_capacity) {
                resizeInternal(m_capacity == 0 ? 1 : m_capacity * 2);
            }
            for (Size i = m_size; i > index; --i) {
                m_allocator.construct(m_data.get() + i, TypeTraits::move(m_data[i - 1]));
                m_allocator.destroy(m_data.get() + i - 1);
            }
            m_allocator.construct(m_data.get() + index, value);
            ++m_size;
        }

        void removeAt(Size index) {
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            m_allocator.destroy(m_data.get() + index);
            for (Size i = index; i < m_size - 1; ++i) {
                m_allocator.construct(m_data.get() + i, TypeTraits::move(m_data[i + 1]));
                m_allocator.destroy(m_data.get() + i + 1);
            }
            --m_size;
        }

        void clear() {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            for (Size i = 0; i < m_size; ++i) {
                m_allocator.destroy(m_data.get() + i);
            }
            m_allocator.deallocate(m_data.release());
            m_data.reset(m_allocator.allocate(m_capacity));
            m_size = 0;
        }

        [[nodiscard]] Size size() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_size;
        }

        [[nodiscard]] T *data() const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            return m_data.get();
        }

        T &operator[](Size index) {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        const T &operator[](Size index) const {
            Threading::LockGuard<Threading::Mutex> lock(m_mtx);
            if (index >= m_size) {
                throw OutOfRangeException("Index out of range");
            }
            return m_data[index];
        }

        class Iterator {
        private:
            T *m_ptr;
        public:
            explicit Iterator(T *ptr) : m_ptr(ptr) {}

            Iterator &operator++() {
                ++m_ptr;
                return *this;
            }

            Iterator operator++(Int32) {
                Iterator tmp = *this;
                ++m_ptr;
                return tmp;
            }

            T &operator*() const {
                return *m_ptr;
            }

            T *operator->() const {
                return m_ptr;
            }

            Boolean operator==(const Iterator &other) const {
                return m_ptr == other.m_ptr;
            }

            Boolean operator!=(const Iterator &other) const {
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
            const T *m_ptr;
        public:
            explicit ConstIterator(const T *ptr) : m_ptr(ptr) {}

            ConstIterator &operator++() {
                ++m_ptr;
                return *this;
            }

            ConstIterator operator++(Int32) {
                ConstIterator tmp = *this;
                ++m_ptr;
                return tmp;
            }

            const T &operator*() const {
                return *m_ptr;
            }

            const T *operator->() const {
                return m_ptr;
            }

            Boolean operator==(const ConstIterator &other) const {
                return m_ptr == other.m_ptr;
            }

            Boolean operator!=(const ConstIterator &other) const {
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
