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
#include <Cedar/Core/Container/Pair.h>

#include <initializer_list>

namespace Cedar::Core::Container {
    template<typename KeyType, typename ValueType>
    class HashNode {
    public:
        KeyType key;
        ValueType value;
        HashNode *next;

        HashNode(const KeyType &key, const ValueType &value) : key(key), value(value), next(nullptr) {}
    };

    template<typename KeyType, typename ValueType, Size TableSize = 256>
    class HashMap {
    private:
        HashNode<KeyType, ValueType> *buckets[TableSize];
        Threading::Mutex locks[TableSize];

        Hash myHash(const KeyType &key) const {
            return hash<KeyType>(key) % TableSize;
        }

    public:
        HashMap() {
            for (Size i = 0; i < TableSize; ++i) {
                buckets[i] = nullptr;
            }
        }

        HashMap(std::initializer_list<Pair<const KeyType, ValueType>> list) : HashMap() {
            for (const auto &element: list) {
                insert(element.first, element.second);
            }
        }

        ~HashMap() {
            clear();
        }

        void insert(const KeyType &key, const ValueType &value) {
            Size index = myHash(key);
            HashNode<KeyType, ValueType> *newNode = new HashNode<KeyType, ValueType>(key, value);
            locks[index].lock();
            HashNode<KeyType, ValueType> *node = buckets[index];
            if (node == nullptr) {
                buckets[index] = newNode;
            } else {
                while (node->next != nullptr) {
                    node = node->next;
                }
                node->next = newNode;
            }
            locks[index].unlock();
        }

        ValueType *find(const KeyType &key) const {
            Size index = myHash(key);
            locks[index].lock();
            HashNode<KeyType, ValueType> *node = buckets[index];
            while (node != nullptr) {
                if (node->key == key) {
                    locks[index].unlock();
                    return &node->value;
                }
                node = node->next;
            }
            locks[index].unlock();
            return nullptr;
        }

        Boolean remove(const KeyType &key) {
            Size index = myHash(key);
            locks[index].lock();
            HashNode<KeyType, ValueType> *node = buckets[index];
            HashNode<KeyType, ValueType> *prev = nullptr;
            while (node != nullptr) {
                if (node->key == key) {
                    if (prev == nullptr) {
                        buckets[index] = node->next;
                    } else {
                        prev->next = node->next;
                    }
                    delete node;
                    locks[index].unlock();
                    return true;
                }
                prev = node;
                node = node->next;
            }
            locks[index].unlock();
            return false;
        }

        void clear() {
            for (Size i = 0; i < TableSize; ++i) {
                locks[i].lock();
                HashNode<KeyType, ValueType> *node = buckets[i];
                while (node != nullptr) {
                    HashNode<KeyType, ValueType> *next = node->next;
                    delete node;
                    node = next;
                }
                buckets[i] = nullptr;
                locks[i].unlock();
            }
        }

        ValueType &operator[](const KeyType &key) {
            Size index = myHash(key);
            HashNode<KeyType, ValueType> *node = buckets[index];

            while (node != nullptr) {
                if (node->key == key) {
                    return node->value;
                }
                node = node->next;
            }

            auto *newNode = new HashNode<KeyType, ValueType>(key, ValueType());
            newNode->next = buckets[index];
            buckets[index] = newNode;
            return newNode->value;
        }

        class Iterator {
        private:
            HashNode<KeyType, ValueType> **buckets;
            HashNode<KeyType, ValueType> *current;
            Size bucketIndex;
            Size tableSize;

        public:
            Iterator(HashNode<KeyType, ValueType> **buckets, Size index, Size tableSize, Boolean end = false)
                    : buckets(buckets), bucketIndex(index), tableSize(tableSize), current(nullptr) {
                if (!end) {
                    while (bucketIndex < tableSize && !buckets[bucketIndex]) {
                        bucketIndex++;
                    }
                    if (bucketIndex < tableSize) {
                        current = buckets[bucketIndex];
                    }
                }
            }

            Iterator &operator++() {
                if (current && current->next) {
                    current = current->next;
                } else {
                    do {
                        bucketIndex++;
                    } while (bucketIndex < tableSize && !buckets[bucketIndex]);

                    current = (bucketIndex < tableSize) ? buckets[bucketIndex] : nullptr;
                }
                return *this;
            }

            Pair<const KeyType &, ValueType &> operator*() const {
                return Pair<const KeyType &, ValueType &>(current->key, current->value);
            }

            Boolean operator!=(const Iterator &other) const {
                return current != other.current;
            }
        };

        Iterator begin() const {
            return Iterator(buckets, 0, TableSize);
        }

        Iterator end() const {
            return Iterator(buckets, TableSize, TableSize, true);
        }
    };
}
