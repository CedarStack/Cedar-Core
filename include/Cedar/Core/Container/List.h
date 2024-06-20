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

#include <Cedar/Core/OutOfRangeException.h>

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
        ListNode<T>* head;
        ListNode<T>* tail;
        Size size;
        Threading::Mutex mtx;

    public:
        List() : head(nullptr), tail(nullptr), size(0) {}

        ~List() {
            clear();
        }

        void append(T value) {
            mtx.lock();
            auto* newNode = new ListNode<T>(value);
            if (!head) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            size++;
            mtx.unlock();
        }

        bool remove(T value) {
            mtx.lock();
            ListNode<T>* current = head;
            ListNode<T>* prev = nullptr;

            while (current != nullptr) {
                if (current->value == value) {
                    if (prev) {
                        prev->next = current->next;
                    } else {
                        head = current->next;
                    }

                    if (current == tail) {
                        tail = prev;
                    }

                    delete current;
                    size--;
                    mtx.unlock();
                    return true;
                }
                prev = current;
                current = current->next;
            }
            mtx.unlock();
            return false;
        }

        void clear() {
            mtx.lock();
            ListNode<T>* current = head;
            while (current != nullptr) {
                ListNode<T>* next = current->next;
                delete current;
                current = next;
            }
            head = nullptr;
            tail = nullptr;
            size = 0;
            mtx.unlock();
        }

        [[nodiscard]] Size getSize() const {
            mtx.lock();
            Size currentSize = size;
            mtx.unlock();
            return currentSize;
        }

        ListNode<T>* getNodeAt(Index index) const {
            ListNode<T>* current = head;
            Index currentIndex = 0;
            while (current != nullptr && currentIndex < index) {
                current = current->next;
                currentIndex++;
            }
            return current;
        }

        T& operator[](Index index) {
            mtx.lock();
            ListNode<T>* node = getNodeAt(index);
            if (node == nullptr) {
                mtx.unlock();
                throw OutOfRangeException("Index out of range");
            }
            T& value = node->value;
            mtx.unlock();
            return value;
        }

        const T& operator[](Index index) const {
            mtx.lock();
            ListNode<T>* node = getNodeAt(index);
            if (node == nullptr) {
                mtx.unlock();
                throw OutOfRangeException("Index out of range");
            }
            const T& value = node->value;
            mtx.unlock();
            return value;
        }

        class Iterator {
        public:
            explicit Iterator(ListNode<T>* node) : current(node) {}

            T& operator*() const {
                return current->value;
            }

            Iterator& operator++() {
                if (current) current = current->next;
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }

        private:
            ListNode<T>* current;
        };

        Iterator begin() const {
            return Iterator(head);
        }

        Iterator end() const {
            return Iterator(nullptr);
        }
    };

}

