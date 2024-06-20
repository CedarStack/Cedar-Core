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
    struct RemoveReference<T&> {
        typedef T type;
    };

    template<typename T>
    struct RemoveReference<T&&> {
        typedef T type;
    };


    template<typename T>
    typename Memory::RemoveReference<T>::type&& move(T&& arg) {
        return static_cast<typename Memory::RemoveReference<T>::type&&>(arg);
    }
}
