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

namespace Cedar::Core {
    using Int8 = signed char;
    using Int16 = short;
    using Int32 = int;
    using Int64 = long long;

    using UInt8 = unsigned char;
    using UInt16 = unsigned short;
    using UInt32 = unsigned int;
    using UInt64 = unsigned long long;

    using Float32 = float;
    using Float64 = double;

    using Boolean = bool;

    using Byte = unsigned char;
    using Rune = char32_t;

    using CString = const char*;
    using Pointer = void*;

//    using Char = char;

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
    using SizeType = long long;
    using USizeType = unsigned long long;
#else
    using SizeType = int;
    using USizeType = unsigned int;
#endif

    using Size = USizeType;
    using Index = SizeType;
}
