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

#include <Cedar/Core/BasicTypes.h>

namespace Cedar::Core {
    template<>
    Hash hash<Int8>(const Int8 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<Int16>(const Int16 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<Int32>(const Int32 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<Int64>(const Int64 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<UInt8>(const UInt8 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<UInt16>(const UInt16 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<UInt32>(const UInt32 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<UInt64>(const UInt64 &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<Float32>(const Float32 &val) {
        return *reinterpret_cast<const UInt32 *>(&val);
    }

    template<>
    Hash hash<Float64>(const Float64 &val) {
        return *reinterpret_cast<const UInt64 *>(&val);
    }

    template<>
    Hash hash<Boolean>(const Boolean &val) {
        return static_cast<Hash>(val);
    }

    template<>
    Hash hash<Pointer>(const Pointer &val) {
        auto ptrVal = reinterpret_cast<Size>(val);
        return static_cast<Hash>(ptrVal);
    }

    template<>
    Hash hash<Rune>(const Rune &val) {
        return static_cast<Hash>(val);
    }
}