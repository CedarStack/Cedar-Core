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

#include <Cedar/Core/Math/MathFunctions.h>

using namespace Cedar::Core;
using namespace Cedar::Core::Math;

template<>
Float32 Math::NaN<Float32>() {
    UInt32 nanBits = 0x7FC00000;
    return *reinterpret_cast<Float32*>(&nanBits);
}

template<>
Float64 Math::NaN<Float64>() {
    UInt64 nanBits = 0x7FF8000000000000;
    return *reinterpret_cast<Float64*>(&nanBits);
}

template<>
Float32 Math::Inf<Float32>() {
    UInt32 infBits = 0x7F800000;
    return *reinterpret_cast<Float32*>(&infBits);
}

template<>
Float64 Math::Inf<Float64>() {
    UInt64 infBits = 0x7FF0000000000000;
    return *reinterpret_cast<Float64*>(&infBits);
}

template<>
Float32 Math::Epsilon<Float32>() {
    return 1.1920928955078125e-07f;
}

template<>
Float64 Math::Epsilon<Float64>() {
    return 2.2204460492503131e-16;
}

template<>
Float32 Math::SmallestNormal<Float32>() {
    return 1.17549435e-38f;
}

template<>
Float64 Math::SmallestNormal<Float64>() {
    return 2.2250738585072014e-308;
}

// Maximal values using hexadecimal notation
template<> UInt8 Math::MaxValue<UInt8>() { return 0xFF; }                   // 255
template<> Int8 Math::MaxValue<Int8>() { return 0x7F; }                    // 127
template<> UInt16 Math::MaxValue<UInt16>() { return 0xFFFF; }              // 65535
template<> Int16 Math::MaxValue<Int16>() { return 0x7FFF; }                // 32767
template<> UInt32 Math::MaxValue<UInt32>() { return 0xFFFFFFFFU; }         // 4294967295U
template<> Int32 Math::MaxValue<Int32>() { return 0x7FFFFFFF; }            // 2147483647
template<> UInt64 Math::MaxValue<UInt64>() { return 0xFFFFFFFFFFFFFFFFULL; }  // 18446744073709551615ULL
template<> Int64 Math::MaxValue<Int64>() { return 0x7FFFFFFFFFFFFFFFLL; }     // 9223372036854775807LL

template<> UInt8 Math::MinValue<UInt8>() { return 0x00; }                   // 0
template<> Int8 Math::MinValue<Int8>() { return -0x80; }                    // -128
template<> UInt16 Math::MinValue<UInt16>() { return 0x0000; }               // 0
template<> Int16 Math::MinValue<Int16>() { return -0x8000; }                // -32768
template<> UInt32 Math::MinValue<UInt32>() { return 0x00000000U; }          // 0U
template<> Int32 Math::MinValue<Int32>() { return -0x80000000; }            // -2147483648
template<> UInt64 Math::MinValue<UInt64>() { return 0x0000000000000000ULL; }  // 0ULL
template<> Int64 Math::MinValue<Int64>() { return -0x8000000000000000LL; }     // -9223372036854775808LL

template<> Float32 Math::MaxValue<Float32>() { return 3.402823e+38F; }      // Maximum for Float32
template<> Float64 Math::MaxValue<Float64>() { return 1.7976931348623157e+308; }   // Maximum for Float64

template<> Float32 Math::MinValue<Float32>() { return -3.402823e+38F; }     // Minimum for Float32
template<> Float64 Math::MinValue<Float64>() { return -1.7976931348623157e+308; }  // Minimum for Float64


Boolean Math::isNaN(Float32 value) {
    FloatBits32 u = { value };

    UInt32 exponent = (u.intValue >> 23) & 0xFF;
    UInt32 mantissa = u.intValue & 0x7FFFFF;

    return (exponent == 0xFF) && (mantissa != 0);
}

Boolean Math::isNaN(Float64 value) {
    FloatBits64 u = { value };

    UInt64 exponent = (u.intValue >> 52) & 0x7FF;
    UInt64 mantissa = u.intValue & 0xFFFFFFFFFFFFF;

    return (exponent == 0x7FF) && (mantissa != 0);
}

Boolean Math::isInf(Float32 value) {
    FloatBits32 u = { value };

    UInt32 exponent = (u.intValue >> 23) & 0xFF;
    UInt32 mantissa = u.intValue & 0x7FFFFF;

    return (exponent == 0xFF) && (mantissa == 0);
}

Boolean Math::isInf(Float64 value) {
    FloatBits64 u = { value };

    UInt64 exponent = (u.intValue >> 52) & 0x7FF;
    UInt64 mantissa = u.intValue & 0xFFFFFFFFFFFFF;

    return (exponent == 0x7FF) && (mantissa == 0);
}

FractionalParts<Float32> Math::splitFractional(Float32 value) {
    FloatBits32 u = { value };
    Int32 exponent = static_cast<Int32>(u.intValue >> 23) & 0xFF;
    Int32 e = exponent - 0x7F;
    FractionalParts<Float32> result{};

    if (e >= 23) {
        result.integerPart = value;
        result.fractionalPart = 0.0f;
        return result;
    }
    if (e < 0) {
        result.integerPart = 0.0f;
        result.fractionalPart = value;
        return result;
    }

    UInt32 mask = (1 << (23 - e)) - 1;
    if ((u.intValue & mask) == 0) {
        result.integerPart = value;
        result.fractionalPart = 0.0f;
        return result;
    }

    u.intValue &= ~mask;
    result.integerPart = u.floatValue;
    result.fractionalPart = value - u.floatValue;
    return result;
}

FractionalParts<Float64> Math::splitFractional(Float64 value) {
    FloatBits64 u = { value };
    Int32 exponent = static_cast<Int32>(u.intValue >> 52) & 0x7FF;
    Int32 e = exponent - 0x3FF;
    FractionalParts<Float64> result{};

    if (e >= 52) {
        result.integerPart = value;
        result.fractionalPart = 0.0;
        return result;
    }
    if (e < 0) {
        result.integerPart = 0.0;
        result.fractionalPart = value;
        return result;
    }

    UInt64 mask = (1ULL << (52 - e)) - 1;
    if ((u.intValue & mask) == 0) {
        result.integerPart = value;
        result.fractionalPart = 0.0;
        return result;
    }

    u.intValue &= ~mask;
    result.integerPart = u.floatValue;
    result.fractionalPart = value - u.floatValue;
    return result;
}
