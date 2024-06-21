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

#include <Cedar/Core/String.h>
#include <Cedar/Core/Memory.h>
#include <Cedar/Core/InvalidStateException.h>
#include <Cedar/Core/Container/List.h>
#include <Cedar/Core/Math/MathFunctions.h>

using namespace Cedar::Core;
using namespace Cedar::Core::Container;

struct String::Impl {
    Byte* data;
    Size size;

    Impl() : data(nullptr), size(0) {}

    explicit Impl(CString str) {
        if (str) {
            size = Memory::calcCStringLength(str);
            data = static_cast<Byte*>(Memory::allocate(size + 1));
            Memory::copyCString(reinterpret_cast<CString>(data), str);
        } else {
            size = 0;
            data = nullptr;
        }
    }

    Impl(const Impl& other) {
        if (other.data) {
            size = other.size;
            data = static_cast<Byte*>(Memory::allocate(size + 1));
            Memory::copyCString(reinterpret_cast<CString>(data), reinterpret_cast<CString>(other.data));
        } else {
            data = nullptr;
            size = 0;
        }
    }

    explicit Impl(CString str, Size len) {
        if (str) {
            size = len;
            data = static_cast<Byte*>(Memory::allocate(size + 1));
            Memory::copy(data, (Pointer) str, size);
            data[size] = '\0'; // Ensure null termination
        } else {
            size = 0;
            data = nullptr;
        }
    }

    ~Impl() {
        Memory::release(data);
    }

    Impl& operator=(const Impl& other) {
        if (this != &other) {
            Byte* newData = nullptr;
            if (other.data) {
                newData = static_cast<Byte*>(Memory::allocate(other.size + 1));
                Memory::copyCString(reinterpret_cast<CString>(newData), reinterpret_cast<CString>(other.data));
            }
            Memory::release(data);
            data = newData;
            size = other.size;
        }
        return *this;
    }

    static Size bytesInRune(Byte firstByte) {
        if (firstByte < 128) return 1; // 0xxxxxxx
        else if ((firstByte & 0xE0) == 0xC0) return 2; // 110xxxxx
        else if ((firstByte & 0xF0) == 0xE0) return 3; // 1110xxxx
        else if ((firstByte & 0xF8) == 0xF0) return 4; // 11110xxx
        return 0; // Invalid first byte in a UTF-8 sequence
    }

    [[nodiscard]] Rune decodeRuneAt(Size i) const {
        const Byte* bytes = &data[i];
        Rune rune = 0;
        Size count = bytesInRune(bytes[0]);

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
};

String::String() : pImpl(new Impl()) {}

String::String(const char* str) : pImpl(new Impl(str)) {}

String::String(CString str, Size len) : pImpl(new Impl(str, len)) {}

String::String(const String& other) : pImpl(new Impl(*other.pImpl)) {}

String::String(String&& other) noexcept : pImpl(other.pImpl) {
    other.pImpl = nullptr;
}

String::~String() {
    delete pImpl;
}

Size String::length() const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (!pImpl->data) {
        return 0;
    }

    Size length = 0;
    Size i = 0;
    while (i < pImpl->size) {
        i += Impl::bytesInRune(pImpl->data[i]);
        length++;
    }

    return length;
}


Rune String::at(Index index) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (!pImpl->data || index >= pImpl->size) {
        throw OutOfRangeException("Out of range");
    }

    Size charCount = 0;
    Size i = 0;
    while (i < pImpl->size) {
        if (charCount == index) {
            return pImpl->decodeRuneAt(i);
        }
        i += Impl::bytesInRune(pImpl->data[i]);
        charCount++;
    }

    return 0;
}

Rune String::operator[](Index index) const {
    return this->at(index);
}


String String::trimStart() const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    Size i = 0;
    while (i < pImpl->size && isWhitespace(pImpl->data[i])) {
        i += Impl::bytesInRune(pImpl->data[i]); // Advance by rune, not byte
    }
    return {reinterpret_cast<CString>(pImpl->data + i), pImpl->size - i};
}

String String::trimEnd() const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    Size i = pImpl->size;
    while (i > 0) {
        Size prev = i - 1;
        while (prev > 0 && (pImpl->data[prev] & 0xC0) == 0x80) prev--; // Move back to the start of the rune
        if (!isWhitespace(pImpl->data[prev])) break;
        i = prev;
    }
    return {reinterpret_cast<CString>(pImpl->data), i};
}

String String::trim() const {
    return this->trimStart().trimEnd();
}

String String::stripPrefix(const String& prefix) const {
    if (this->startsWith(prefix)) {
        Size newLen = pImpl->size - prefix.pImpl->size;
        return {reinterpret_cast<CString>(pImpl->data + prefix.pImpl->size), newLen};
    }
    return *this; // Return original string if no prefix found
}

String String::stripSuffix(const String& suffix) const {
    if (this->endsWith(suffix)) {
        Size newLen = pImpl->size - suffix.pImpl->size;
        return {reinterpret_cast<CString>(pImpl->data), newLen};
    }
    return *this; // Return original string if no suffix found
}

Boolean String::contains(const String &substring) const {
    return this->find(substring) != -1;
}

Boolean String::startsWith(const String& prefix) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (prefix.pImpl->size > pImpl->size) return false;
    return Memory::compare(pImpl->data, prefix.pImpl->data, prefix.pImpl->size) == 0;
}

Boolean String::endsWith(const String& suffix) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (suffix.pImpl->size > pImpl->size) return false;
    return Memory::compare(pImpl->data + (pImpl->size - suffix.pImpl->size), suffix.pImpl->data, suffix.pImpl->size) == 0;
}

List<String> String::split(const String& delimiter) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    List<String> result;
    Size delimiterLength = delimiter.pImpl->size;
    if (delimiterLength == 0) {
        result.append(String(reinterpret_cast<const char*>(pImpl->data), pImpl->size));
        return result;
    }

    const Byte* start = pImpl->data;
    const Byte* end = pImpl->data + pImpl->size;
    const Byte* current = start;

    while (current < end) {
        const Byte* found = nullptr;
        for (Size i = 0; i <= (end - current - delimiterLength); ++i) {
            if (Memory::compare((Pointer) reinterpret_cast<CString>(current + i),
                                (Pointer) reinterpret_cast<CString>(delimiter.pImpl->data), delimiterLength) == 0) {
                found = current + i;
                break;
            }
        }

        if (found) {
            Size segmentSize = found - current;
            result.append(String(reinterpret_cast<CString>(current), segmentSize));
            current = found + delimiterLength;
        } else {
            result.append(String(reinterpret_cast<CString>(current), end - current));
            break;
        }
    }

    return result;
}

List<String> String::getLines() const {
    return this->split("\n");
}

Index String::find(const String& substring) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (!pImpl->data || !substring.pImpl->data) return -1;

    if (substring.pImpl->size == 0) return 0;

    List<Rune> runes;
    List<Size> runeByteOffsets;
    for (Size i = 0; i < pImpl->size; ) {
        runeByteOffsets.append(i);
        Rune rune = pImpl->decodeRuneAt(i);
        runes.append(rune);
        i += Impl::bytesInRune(pImpl->data[i]);
    }

    List<Rune> subRunes;
    for (Size i = 0; i < substring.pImpl->size; ) {
        Rune rune = substring.pImpl->decodeRuneAt(i);
        subRunes.append(rune);
        i += Impl::bytesInRune(substring.pImpl->data[i]);
    }

    Size thisSize = runes.size();
    Size subSize = subRunes.size();
    if (subSize > thisSize || subSize == 0) return -1;

    const UInt64 prime = 101;
    UInt64 subHash = 0, curHash = 0, power = 1;

    for (Index i = 0; i < subSize; ++i) {
        subHash = (subHash * prime + subRunes[i]) % Math::Max<UInt64>();
        curHash = (curHash * prime + runes[i]) % Math::Max<UInt64>();
        if (i > 0) power = (power * prime) % Math::Max<UInt64>();
    }

    for (Index i = 0; i <= thisSize - subSize; ++i) {
        if (subHash == curHash) {
            bool match = true;
            for (Index j = 0; j < subSize; ++j) {
                if (runes[i + j] != subRunes[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return i;
            }
        }

        if (i < thisSize - subSize) {
            curHash = (prime * (curHash - runes[i] * power) + runes[i + subSize]) % Math::Max<UInt64>();
        }
    }

    return -1;
}

String& String::operator=(const String& other) {
    if (this != &other) {
        *pImpl = *other.pImpl;
    }
    return *this;
}

String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        delete pImpl;
        pImpl = other.pImpl;
        other.pImpl = nullptr;
    }
    return *this;
}

String String::operator+(const String& other) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    Size newSize = this->pImpl->size + other.pImpl->size;
    Byte* newData = static_cast<Byte*>(Memory::allocate(newSize + 1));
    Memory::copy(newData, this->pImpl->data, this->pImpl->size);
    Memory::copy(newData + this->pImpl->size, other.pImpl->data, other.pImpl->size);
    newData[newSize] = '\0';
    return {reinterpret_cast<CString>(newData), newSize};
}

bool String::operator==(const String& other) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    return this->pImpl->size == other.pImpl->size &&
           Memory::compare(this->pImpl->data, other.pImpl->data, this->pImpl->size) == 0;
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

CString String::rawString() const noexcept {
    return !pImpl ? nullptr : reinterpret_cast<CString>(pImpl->data);
}

bool String::isWhitespace(Rune rune) {
    switch (rune) {
        case 0x09: // Character TABULATION
        case 0x0A: // LINE FEED
        case 0x0B: // VERTICAL TAB
        case 0x0C: // FORM FEED
        case 0x0D: // CARRIAGE RETURN
        case 0x20: // SPACE
        case 0x85: // NEXT LINE (NEL)
        case 0xA0: // NO-BREAK SPACE
        case 0x1680: // OGHAM SPACE MARK
        case 0x2000: // EN QUAD
        case 0x2001: // EM QUAD
        case 0x2002: // EN SPACE
        case 0x2003: // EM SPACE
        case 0x2004: // THREE-PER-EM SPACE
        case 0x2005: // FOUR-PER-EM SPACE
        case 0x2006: // SIX-PER-EM SPACE
        case 0x2007: // FIGURE SPACE
        case 0x2008: // PUNCTUATION SPACE
        case 0x2009: // THIN SPACE
        case 0x200A: // HAIR SPACE
        case 0x2028: // LINE SEPARATOR
        case 0x2029: // PARAGRAPH SEPARATOR
        case 0x202F: // NARROW NO-BREAK SPACE
        case 0x205F: // MEDIUM MATHEMATICAL SPACE
        case 0x3000: // IDEOGRAPHIC SPACE
            return true;
        default:
            return false;
    }
}
