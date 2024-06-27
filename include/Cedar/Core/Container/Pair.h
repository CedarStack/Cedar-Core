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

#include <Cedar/Core/Memory.h>

namespace Cedar::Core::Container {

    template<typename T1, typename T2>
    class Pair {
    public:
        T1 first;
        T2 second;

        Pair() : first(T1()), second(T2()) {}

        Pair(const T1& x, const T2& y) : first(x), second(y) {}
        Pair(const Pair& other) : first(other.first), second(other.second) {}
        Pair(Pair&& other) noexcept : first(TypeTraits::move(other.first)), second(TypeTraits::move(other.second)) {}

        Pair& operator=(const Pair& other) {
            if (this != &other) {
                first = other.first;
                second = other.second;
            }
            return *this;
        }

        Pair& operator=(Pair&& other) noexcept {
            if (this != &other) {
                first = TypeTraits::move(other.first);
                second = TypeTraits::move(other.second);
            }
            return *this;
        }
    };
}
