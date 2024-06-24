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
#include <Cedar/Core/OutOfMemoryException.h>

namespace Cedar::Core::Memory {
    void copy(Pointer target, Pointer source, Size size);

    void copyCString(CString target, CString source);

    Size calcCStringLength(CString string);

    Int32 compare(Pointer p1, Pointer p2, Size size);

    Pointer allocate(Size size);

    void release(Pointer pointer);

    template<typename T>
    struct RemoveReference {
        typedef T type;
    };

    template<typename T>
    struct RemoveReference<T &> {
        typedef T type;
    };

    template<typename T>
    struct RemoveReference<T &&> {
        typedef T type;
    };

    template<typename T>
    typename RemoveReference<T>::type &&move(T &&arg) {
        return static_cast<typename Memory::RemoveReference<T>::type &&>(arg);
    }

    template<typename T>
    T&& forward(typename RemoveReference<T>::type& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    T&& forward(typename RemoveReference<T>::type&& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    class Allocator {
    public:
        T* allocate(Size numObjects) {
            if (numObjects == 0) {
                return nullptr;
            }
            void* ptr = ::operator new(numObjects * sizeof(T));
            if (!ptr) {
                throw OutOfMemoryException("Out of memory");
            }
            return reinterpret_cast<T*>(ptr);
        }

        void deallocate(T* ptr) {
            ::operator delete(ptr);
        }

        template<typename... Args>
        void construct(T* ptr, Args&&... args) {
            new (ptr) T(forward<Args>(args)...);
        }

        void destroy(T* ptr) {
            ptr->~T();
        }
    };

    template<typename T>
    class UniquePointer {
    public:
        explicit UniquePointer(T* p = nullptr): m_pointer(p) {}

        UniquePointer(const UniquePointer&) = delete;
        UniquePointer& operator=(const UniquePointer&) = delete;

        UniquePointer(UniquePointer&& moving) noexcept : UniquePointer(moving.m_pointer) {
            moving.m_pointer = nullptr;
        }

        UniquePointer& operator=(UniquePointer&& moving) noexcept {
            if (this != &moving) {
                delete m_pointer;
                m_pointer = moving.m_pointer;
                moving.m_pointer = nullptr;
            }
            return *this;
        }

        T& operator*() const { return *m_pointer; }
        T* operator->() const { return m_pointer; }
        T* get() const { return m_pointer; }
        T* release() {
            T* tmp = m_pointer;
            m_pointer = nullptr;
            return tmp;
        }
        void reset(T* p = nullptr) {
            T* old = m_pointer;
            m_pointer = p;
            delete old;
        }
    private:
            T* m_pointer;
    };

    template<typename T>
    class UniquePointer<T[]> {
    public:
        explicit UniquePointer(T* p = nullptr): m_pointer(p) {}

        UniquePointer(const UniquePointer&) = delete;
        UniquePointer& operator=(const UniquePointer&) = delete;

        UniquePointer(UniquePointer&& moving) noexcept : m_pointer(moving.m_pointer) {
            moving.m_pointer = nullptr;
        }

        UniquePointer& operator=(UniquePointer&& moving) noexcept {
            if (this != &moving) {
                delete[] m_pointer;
                m_pointer = moving.m_pointer;
                moving.m_pointer = nullptr;
            }
            return *this;
        }

        ~UniquePointer() {
            delete[] m_pointer;
        }

        T& operator[](std::size_t idx) const { return m_pointer[idx]; }
        T* get() const { return m_pointer; }
        T* release() {
            T* tmp = m_pointer;
            m_pointer = nullptr;
            return tmp;
        }
        void reset(T* p = nullptr) {
            T* old = m_pointer;
            m_pointer = p;
            delete[] old;
        }

    private:
        T* m_pointer;
    };


    template<typename T>
    class SharedPointer {
    public:
        explicit SharedPointer(T* p = nullptr) : m_pointer(p), m_count(new unsigned(1)) {}

        SharedPointer(const SharedPointer& sp) : m_pointer(sp.m_pointer), m_count(sp.m_count) {
            (*m_count)++;
        }

        ~SharedPointer() {
            release();
        }

        SharedPointer& operator=(const SharedPointer& sp) {
            if (this != &sp) {
                release();
                m_pointer = sp.m_pointer;
                m_count = sp.m_count;
                (*m_count)++;
            }
            return *this;
        }

        T* get() const { return m_pointer; }

        T& operator*() const { return *m_pointer; }

        T* operator->() const { return m_pointer; }

        unsigned useCount() const { return *m_count; }

        void reset(T* p = nullptr) {
            release();
            m_pointer = p;
            m_count = new unsigned(1);
        }
    private:
        T* m_pointer;
        unsigned* m_count;

        void release() {
            if (m_count && --(m_count) == 0) {
                delete m_pointer;
                delete m_count;
            }
        }
    };
}
