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

namespace Cedar::Core::Math {
    constexpr Float64 PI = 3.14159265358979323846;
    constexpr Float64 EulerNumber = 2.71828182845904523536;
    constexpr Float64 Sqrt2 = 1.41421356237309504880;
    constexpr Float64 Sqrt1_2 = 0.70710678118654752440;
    constexpr Float64 Log2E = 1.44269504088896340736;
    constexpr Float64 Log10E = 0.43429448190325182765;
    constexpr Float64 Ln2 = 0.69314718055994530942;
    constexpr Float64 Ln10 = 2.30258509299404568402;
    constexpr Float64 GoldenRatio = 1.61803398874989484820;

    typedef union {
        Float32 floatValue;
        UInt32 intValue;
    } FloatBits32;

    typedef union {
        Float64 floatValue;
        UInt64 intValue;
    } FloatBits64;

    template<typename T>
    struct FractionalParts {
        T integerPart;
        T fractionalPart;
    };

    template<typename T>
    T NaN();

    template<typename T>
    T Inf();

    template<typename T>
    T Epsilon();

    template<typename T>
    T SmallestNormal();

    template<typename T>
    T MaxValue();

    template<typename T>
    T MinValue();

    template<typename T>
    inline const T max(const T& a, const T& b) {
        return a > b ? a : b;
    }

    template<typename T>
    inline const T min(const T& a, const T& b) {
        return a > b ? b : a;
    }

    Boolean isNaN(Float32 value);
    Boolean isNaN(Float64 value);

    Boolean isInf(Float32 value);
    Boolean isInf(Float64 value);

    FractionalParts<Float32> splitFractional(Float32 value);
    FractionalParts<Float64> splitFractional(Float64 value);

    Float32 floor(Float32 value);
    Float64 floor(Float64 value);

    Float32 ceil(Float32 value);
    Float64 ceil(Float64 value);

    Float32 round(Float32 value);
    Float64 round(Float64 value);

    Int32 abs(Int32 value);
    Int64 abs(Int64 value);
    Float32 abs(Float32 value);
    Float64 abs(Float64 value);

    Float64 sqrt(Float64 value);
}
