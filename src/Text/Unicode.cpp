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

#include <Cedar/Core/Text/Unicode.h>

#include "./Unicode/UnicodeTables.h"

using namespace Cedar::Core;
using namespace Cedar::Core::Container;
using namespace Cedar::Core::Text;

Boolean isInRanges(UInt32 codepoint, const Array<UnicodeRange>& ranges) {
    for (Index i = 0; i < ranges.size(); ++i) {
        if (codepoint >= ranges[i].start && codepoint <= ranges[i].end) {
            return true;
        }
    }
    return false;
}

Boolean Unicode::isLetter(Rune codepoint) {
    return isInRanges(codepoint, LuRanges) ||
            isInRanges(codepoint, LlRanges) ||
            isInRanges(codepoint, LtRanges) ||
            isInRanges(codepoint, LmRanges) ||
            isInRanges(codepoint, LoRanges);
}

Boolean Unicode::isDigit(Rune codepoint) {
    return isInRanges(codepoint, NdRanges);
}

Boolean Unicode::isSpace(Rune codepoint) {
    return isInRanges(codepoint, ZsRanges) ||
           isInRanges(codepoint, ZlRanges) ||
           isInRanges(codepoint, ZpRanges) ||
           isInRanges(codepoint, CcRanges);
}

Rune Unicode::toUpperCase(Rune codepoint) {
    if (codepoint >= 'a' && codepoint <= 'z') {
        return codepoint - ('a' - 'A');
    }

    auto* result = ToUpperMap.find(codepoint);
    return result ? *result : codepoint;
}

Rune Unicode::toLowerCase(Rune codepoint) {
    if (codepoint >= 'A' && codepoint <= 'Z') {
        return codepoint + ('a' - 'A');
    }

    auto* result = ToLowerMap.find(codepoint);
    return result ? *result : codepoint;
}

Size Unicode::calculateRuneLength(Byte firstByte) {
    if (firstByte < 0x80) {
        return 1; // 0xxxxxxx, 1 byte
    } else if ((firstByte & 0xE0) == 0xC0) {
        return 2; // 110xxxxx, 2 bytes
    } else if ((firstByte & 0xF0) == 0xE0) {
        return 3; // 1110xxxx, 3 bytes
    } else if ((firstByte & 0xF8) == 0xF0) {
        return 4; // 11110xxx, 4 bytes
    }

    return 0;
}

Rune Unicode::extractRuneAt(const Byte *data, Index i) {
    const Byte* bytes = &data[i];
    Rune rune = 0;
    Size count = calculateRuneLength(bytes[0]);

    if (count == 1) {
        rune = bytes[0];
    } else if (count == 2) {
        rune = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
    } else if (count == 3) {
        rune = ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
    } else if (count == 4) {
        rune = ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
    }

    return rune;
}

String Unicode::encodeRuneToString(Rune codepoint) {
    Byte buffer[5] = {0};

    if (codepoint < 0x80) {
        buffer[0] = codepoint;
    } else if (codepoint < 0x800) {
        buffer[0] = 0xC0 | (codepoint >> 6);
        buffer[1] = 0x80 | (codepoint & 0x3F);
    } else if (codepoint < 0x10000) {
        buffer[0] = 0xE0 | (codepoint >> 12);
        buffer[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        buffer[2] = 0x80 | (codepoint & 0x3F);
    } else if (codepoint <= 0x10FFFF) {
        buffer[0] = 0xF0 | (codepoint >> 18);
        buffer[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        buffer[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        buffer[3] = 0x80 | (codepoint & 0x3F);
    } else {
        throw OutOfRangeException("Rune is outside the valid Unicode range");
    }
    return (CString)buffer;
}
