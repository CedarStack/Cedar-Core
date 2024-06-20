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

#include <Cedar/Core/Memory.h>

// NOLINTNEXTLINE
#include <string.h>

using namespace Cedar::Core;

void Memory::copy(Cedar::Core::Pointer target, Cedar::Core::Pointer source, Cedar::Core::Size size) {
    memcpy(target, source, size);
}

Int32 Memory::compare(Pointer p1, Pointer p2, Size size) {
    return memcmp(p1, p2, size);
}

void Memory::copyCString(Cedar::Core::CString target, Cedar::Core::CString source) {
    strcpy((char*) target, source);
}

Size Memory::calcCStringLength(Cedar::Core::CString string) {
    return strlen(string);
}

Pointer Memory::allocate(Size size) {
    return new char[size]();
}

void Memory::release(Pointer memory) {
    delete[] static_cast<char*>(memory);
}
