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

#include "RSqrtTable.h"

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
    return 1.1920928955078125e-07F;
}

template<>
Float64 Math::Epsilon<Float64>() {
    return 2.2204460492503131e-16;
}

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
    int e = exponent - 0x3FF;
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

Float32 Math::floor(Float32 value) {
    if (isNaN(value) || isInf(value)) {
        return value;
    }

    auto parts = splitFractional(value);
    if (parts.fractionalPart < 0) {
        parts.integerPart -= 1.0f;
    }
    return parts.integerPart;
}

Float64 Math::floor(Float64 value) {
    if (isNaN(value) || isInf(value)) {
        return value;
    }

    auto parts = splitFractional(value);
    if (parts.fractionalPart < 0) {
        parts.integerPart -= 1.0;
    }
    return parts.integerPart;
}

Float32 Math::ceil(Float32 value) {
    if (isNaN(value) || isInf(value)) {
        return value;
    }

    auto parts = splitFractional(value);
    if (parts.fractionalPart > 0) {
        parts.integerPart += 1.0f;
    }
    return parts.integerPart;
}

Float64 Math::ceil(Float64 value) {
    if (isNaN(value) || isInf(value)) {
        return value;
    }

    auto parts = splitFractional(value);
    if (parts.fractionalPart > 0) {
        parts.integerPart += 1.0;
    }
    return parts.integerPart;
}

Float32 round(Float32 value) {
    if (isNaN(value) || isInf(value)) {
        return value;
    }

    auto parts = splitFractional(value);
    if (value > 0.0f) {
        if (parts.fractionalPart >= 0.5f) {
            parts.integerPart += 1.0f;
        }
    } else {
        if (parts.fractionalPart <= -0.5f) {
            parts.integerPart -= 1.0f;
        }
    }
    return parts.integerPart;
}

Float64 Math::round(Float64 value) {
    if (isNaN(value) || isInf(value)) {
        return value;
    }

    auto parts = splitFractional(value);
    if (value > 0.0) {
        if (parts.fractionalPart >= 0.5) {
            parts.integerPart += 1.0;
        }
    } else {
        if (parts.fractionalPart <= -0.5) {
            parts.integerPart -= 1.0;
        }
    }
    return parts.integerPart;
}

Int32 Math::abs(Int32 value) {
    return value < 0 ? -value : value;
}

Int64 Math::abs(Int64 value) {
    return value < 0 ? -value : value;
}

Float32 Math::abs(Float32 value) {
    return value < 0 ? -value : value;
}

Float64 Math::abs(Float64 value) {
    return value < 0 ? -value : value;
}

static inline UInt32 mul32(UInt32 a, UInt32 b) {
    return static_cast<UInt64>(a) * b >> 32;
}

static inline UInt64 mul64(UInt64 a, UInt64 b) {
    UInt64 ahi = a >> 32;
    UInt64 alo = a & 0xffffffff;
    UInt64 bhi = b >> 32;
    UInt64 blo = b & 0xffffffff;
    return ahi * bhi + (ahi * blo >> 32) + (alo * bhi >> 32);
}

Float64 Math::sqrt(Float64 value) {
    constexpr UInt64 multiplier = 0xc0000000;

    UInt64 bits = *reinterpret_cast<UInt64*>(&value);
    UInt64 exponent = bits >> 52;

    if (exponent - 0x001 >= 0x7ff - 0x001) {
        if (bits * 2 == 0 || bits == 0x7ff0000000000000) {
            return value;
        }
        if (bits > 0x7ff0000000000000) {
            return NaN<Float64>();
        }

        Float64 temp = value * 0x1p52;
        bits = *reinterpret_cast<UInt64*>(&temp);
        exponent = bits >> 52;
        exponent -= 52;
    }

    Int32 isOdd = (Int32) exponent & 1;
    UInt64 mantissa = (bits << 11) | 0x8000000000000000;
    if (isOdd) mantissa >>= 1;
    exponent = (exponent + 0x3ff) >> 1;

    UInt64 initialGuess = static_cast<UInt32>(RSqrtTable[(bits >> 46) % 128]) << 16;
    UInt64 scaledValue = mul32(mantissa >> 32, initialGuess);
    UInt64 approximation = mul32(scaledValue, initialGuess);
    UInt64 correction = multiplier - approximation;
    initialGuess = mul32(initialGuess, correction) << 1;
    scaledValue = mul32(scaledValue, correction) << 1;

    approximation = mul32(scaledValue, initialGuess);
    correction = multiplier - approximation;
    initialGuess = mul32(initialGuess, correction) << 1;
    initialGuess = initialGuess << 32;
    scaledValue = mul64(mantissa, initialGuess);
    approximation = mul64(scaledValue, initialGuess);
    correction = (multiplier << 32) - approximation;
    scaledValue = mul64(scaledValue, correction);

    scaledValue = (scaledValue - 2) >> 9;
    UInt64 residual = (mantissa << 42) - scaledValue * scaledValue;
    UInt64 adjustment = scaledValue - residual;

    scaledValue += adjustment >> 63;
    scaledValue &= 0x000fffffffffffff;
    scaledValue |= exponent << 52;

    return *reinterpret_cast<Float64*>(&scaledValue);
}
