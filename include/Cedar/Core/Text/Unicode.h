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
#include <Cedar/Core/String.h>

namespace Cedar::Core::Text {
    class Unicode {
    public:
        static Boolean isLetter(Rune codepoint);
        static Boolean isDigit(Rune codepoint);
        static Boolean isSpace(Rune codepoint);

        static Rune toUpperCase(Rune codepoint);
        static Rune toLowerCase(Rune codepoint);

        static Size calculateRuneLength(Byte firstByte);
        static Rune extractRuneAt(const Byte *data, Index i);
        static String encodeRuneToString(Rune codepoint);
    };
}
