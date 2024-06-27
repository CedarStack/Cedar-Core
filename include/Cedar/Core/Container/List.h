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
#include <Cedar/Core/Threading/Mutex.h>

#include <Cedar/Core/Exceptions/OutOfRangeException.h>

namespace Cedar::Core::Container {
    template<typename T>
    class ListNode {
    public:
        T value;
        ListNode<T>* next;

        explicit ListNode(T val) : value(val), next(nullptr) {}
    };

    template<typename T>
    class List {
    private:
        ListNode<T>* m_head;
        ListNode<T>* m_tail;
        Size m_size;
        Threading::Mutex m_mtx;

    public:
        List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

        ~List() {
            clear();
        }

        void append(T value) {
            m_mtx.lock();
            auto* newNode = new ListNode<T>(value);
            if (!m_head) {
                m_head = newNode;
                m_tail = newNode;
            } else {
                m_tail->next = newNode;
                m_tail = newNode;
            }
            m_size++;
            m_mtx.unlock();
        }

        Boolean remove(T value) {
            m_mtx.lock();
            ListNode<T>* current = m_head;
            ListNode<T>* prev = nullptr;

            while (current != nullptr) {
                if (current->value == value) {
                    if (prev) {
                        prev->next = current->next;
                    } else {
                        m_head = current->next;
                    }

                    if (current == m_tail) {
                        m_tail = prev;
                    }

                    delete current;
                    m_size--;
                    m_mtx.unlock();
                    return true;
                }
                prev = current;
                current = current->next;
            }
            m_mtx.unlock();
            return false;
        }

        void clear() {
            m_mtx.lock();
            ListNode<T>* current = m_head;
            while (current != nullptr) {
                ListNode<T>* next = current->next;
                delete current;
                current = next;
            }
            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
            m_mtx.unlock();
        }

        [[nodiscard]] Size size() const {
            m_mtx.lock();
            Size currentSize = m_size;
            m_mtx.unlock();
            return currentSize;
        }

        ListNode<T>* getNodeAt(Size index) const {
            ListNode<T>* current = m_head;
            Size currentIndex = 0;
            while (current != nullptr && currentIndex < index) {
                current = current->next;
                currentIndex++;
            }
            return current;
        }

        T& operator[](Size index) {
            m_mtx.lock();
            ListNode<T>* node = getNodeAt(index);
            if (node == nullptr) {
                m_mtx.unlock();
                throw OutOfRangeException("Index out of range");
            }
            T& value = node->value;
            m_mtx.unlock();
            return value;
        }

        const T& operator[](Size index) const {
            m_mtx.lock();
            ListNode<T>* node = getNodeAt(index);
            if (node == nullptr) {
                m_mtx.unlock();
                throw OutOfRangeException("Index out of range");
            }
            const T& value = node->value;
            m_mtx.unlock();
            return value;
        }

        class Iterator {
        public:
            explicit Iterator(ListNode<T>* node) : m_current(node) {}

            T& operator*() const {
                return m_current->value;
            }

            Iterator& operator++() {
                if (m_current) m_current = m_current->next;
                return *this;
            }

            Boolean operator!=(const Iterator& other) const {
                return m_current != other.m_current;
            }

        private:
            ListNode<T>* m_current;
        };

        Iterator begin() const {
            return Iterator(m_head);
        }

        Iterator end() const {
            return Iterator(nullptr);
        }
    };

}

