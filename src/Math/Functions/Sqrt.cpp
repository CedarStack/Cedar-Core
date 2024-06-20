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

constexpr const UInt16 RSqrtTable[128] = {
        0xb451,0xb2f0,0xb196,0xb044,0xaef9,0xadb6,0xac79,0xab43,
        0xaa14,0xa8eb,0xa7c8,0xa6aa,0xa592,0xa480,0xa373,0xa26b,
        0xa168,0xa06a,0x9f70,0x9e7b,0x9d8a,0x9c9d,0x9bb5,0x9ad1,
        0x99f0,0x9913,0x983a,0x9765,0x9693,0x95c4,0x94f8,0x9430,
        0x936b,0x92a9,0x91ea,0x912e,0x9075,0x8fbe,0x8f0a,0x8e59,
        0x8daa,0x8cfe,0x8c54,0x8bac,0x8b07,0x8a64,0x89c4,0x8925,
        0x8889,0x87ee,0x8756,0x86c0,0x862b,0x8599,0x8508,0x8479,
        0x83ec,0x8361,0x82d8,0x8250,0x81c9,0x8145,0x80c2,0x8040,
        0xff02,0xfd0e,0xfb25,0xf947,0xf773,0xf5aa,0xf3ea,0xf234,
        0xf087,0xeee3,0xed47,0xebb3,0xea27,0xe8a3,0xe727,0xe5b2,
        0xe443,0xe2dc,0xe17a,0xe020,0xdecb,0xdd7d,0xdc34,0xdaf1,
        0xd9b3,0xd87b,0xd748,0xd61a,0xd4f1,0xd3cd,0xd2ad,0xd192,
        0xd07b,0xcf69,0xce5b,0xcd51,0xcc4a,0xcb48,0xca4a,0xc94f,
        0xc858,0xc764,0xc674,0xc587,0xc49d,0xc3b7,0xc2d4,0xc1f4,
        0xc116,0xc03c,0xbf65,0xbe90,0xbdbe,0xbcef,0xbc23,0xbb59,
        0xba91,0xb9cc,0xb90a,0xb84a,0xb78c,0xb6d0,0xb617,0xb560,
};

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
