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
#include <Cedar/Core/OutOfRangeException.h>
#include <Cedar/Core/Container/List.h>
#include <Cedar/Core/Container/Array.h>
#include <Cedar/Core/Math/MathFunctions.h>

#include <Cedar/Core/Text/Unicode.h>

using namespace Cedar::Core;
using namespace Cedar::Core::Container;
using namespace Cedar::Core::Text;

Array<Index> computeKMPTable(const String& pattern) {
    Size m = pattern.length();
    Array<Index> lps(m);
    Size len = 0;
    lps[0] = 0;
    Size i = 1;

    while (i < m) {
        Rune runeI = Unicode::extractRuneAt(reinterpret_cast<const Byte *>(pattern.rawString()), i);
        Rune runeLen = Unicode::extractRuneAt(reinterpret_cast<const Byte *>(pattern.rawString()), len);

        if (runeI == runeLen) {
            len++;
            lps[i] = len;
            i += Unicode::calculateRuneLength(pattern[i]);
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i += Unicode::calculateRuneLength(pattern[i]);
            }
        }
    }
    return lps;
}

Array<Index> KMPSearch(const String& text, const String& pattern) {
    Array<Index> matches;
    Array<Index> lps = computeKMPTable(pattern);
    Size n = text.length();
    Size m = pattern.length();
    Index i = 0;
    Index j = 0;

    while (i < n) {
        Rune runeI = Unicode::extractRuneAt(reinterpret_cast<const Byte *>(text.rawString()), i);
        Rune runeJ = (j < m) ? Unicode::extractRuneAt(reinterpret_cast<const Byte *>(pattern.rawString()), j) : 0;

        if (runeI == runeJ) {
            i += Unicode::calculateRuneLength(text[i]);
            j += Unicode::calculateRuneLength(pattern[j]);
        }

        if (j >= m) {
            matches.append(i - j);
            j = lps[j - 1];
        } else if (i < n && runeI != runeJ) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i += Unicode::calculateRuneLength(text[i]);
            }
        }
    }
    return matches;
}

struct String::Impl {
    Memory::UniquePointer<Byte[]> data;
    Size size;

    Impl() : size(0) {}

    Impl(CString str, Size len) : size(len) {
        if (str && len > 0) {
            Byte* rawData = static_cast<Byte*>(Memory::allocate(len + 1));
            Memory::copy(rawData, (Byte*) str, len);
            rawData[len] = '\0';
            data.reset(rawData);
        } else {
            size = 0;
            data.reset(nullptr);
        }
    }

    ~Impl() = default;

//    Impl(Impl&& other) noexcept : data(Memory::move(other.data)), size(other.size) {
//        other.size = 0;
//    }

    Impl& operator=(Impl&& other) noexcept {
        if (this != &other) {
            data = Memory::move(other.data);
            size = other.size;
            other.size = 0;
        }
        return *this;
    }
};

String::String() : pImpl(new Impl()) {}

String::String(CString str) : String(str, Memory::calcCStringLength(str)) {}
String::String(CString str, Size len) : pImpl(new Impl(str, len)) {}

String::String(const String& other) : String(other.rawString(), other.rawLength()) {}

String::String(String&& other) noexcept : pImpl(other.pImpl) {
    other.pImpl = nullptr;
}

String::String(Rune rune): String(Unicode::encodeRuneToString(rune)){}

String::~String() {
    delete pImpl;
}

Size String::length() const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (!pImpl->data.get()) {
        return 0;
    }

    Size length = 0;
    for (Size i = 0; i < pImpl->size; i += Unicode::calculateRuneLength(pImpl->data[i])) {
        length++;
    }

    return length;
}

Rune String::at(Index index) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    Size actualLength = this->length();
    if (index < 0) {
        index += actualLength;
    }

    if (index < 0 || index >= actualLength) {
        throw OutOfRangeException("Index out of range");
    }

    Size charCount = 0;
    for (Size i = 0; i < pImpl->size; i += Unicode::calculateRuneLength(pImpl->data[i])) {
        if (charCount == index) {
            return Unicode::extractRuneAt(pImpl->data.get(), i);
        }
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
    while (i < pImpl->size && Unicode::isSpace(pImpl->data[i])) {
        i += Unicode::calculateRuneLength(pImpl->data[i]); // Advance by rune, not byte
    }
    return {reinterpret_cast<CString>(pImpl->data.get() + i), pImpl->size - i};
}

String String::trimEnd() const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    Size i = pImpl->size;
    while (i > 0) {
        Size prev = i - 1;
        while (prev > 0 && (pImpl->data[prev] & 0xC0) == 0x80) prev--; // Move back to the start of the rune
        if (!Unicode::isSpace(pImpl->data[prev])) break;
        i = prev;
    }
    return {reinterpret_cast<CString>(pImpl->data.get()), i};
}

String String::trim() const {
    return this->trimStart().trimEnd();
}

String String::stripPrefix(const String& prefix) const {
    if (this->startsWith(prefix)) {
        Size newLen = pImpl->size - prefix.pImpl->size;
        return {reinterpret_cast<CString>(pImpl->data.get() + prefix.pImpl->size), newLen};
    }
    return *this;
}

String String::stripSuffix(const String& suffix) const {
    if (this->endsWith(suffix)) {
        Size newLen = pImpl->size - suffix.pImpl->size;
        return {reinterpret_cast<CString>(pImpl->data.get()), newLen};
    }
    return *this;
}

String String::substring(Size start, Size len) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    Size numRunes = 0, startPos = 0;
    Size actualLength = 0;

    for (Size i = 0; numRunes < start && i < pImpl->size; numRunes++) {
        i += Unicode::calculateRuneLength(pImpl->data[i]);
        startPos = i;
    }

    if (numRunes < start) {
        throw OutOfRangeException("Start index is out of range");
    }

    String result;
    for (Size i = startPos; i < pImpl->size && (len == NPos || actualLength < len); actualLength++) {
        Rune rune = Unicode::extractRuneAt(pImpl->data.get(), i);
        result = result + String(rune);
        i += Unicode::calculateRuneLength(pImpl->data[i]);
    }

    return result;
}

String String::replace(const String& oldStr, const String& newStr) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (oldStr.pImpl->size == 0) {
        return *this;
    }

    String result;
    Size start = 0;
    int foundPos = this->find(oldStr);

    while (foundPos != -1) {
        result = result + this->substring(start, foundPos - start);
        result = result + newStr;
        start = foundPos + oldStr.length();
        if (start >= this->length()) break;
        foundPos = this->find(oldStr, start);
    }

    if (start < this->length()) {
        result = result + this->substring(start);
    }
    return result;
}

Boolean String::contains(const String &substring) const {
    return this->find(substring) != -1;
}

Boolean String::startsWith(const String& prefix) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (prefix.pImpl->size > pImpl->size) return false;
    return Memory::compare(pImpl->data.get(), prefix.pImpl->data.get(), prefix.pImpl->size) == 0;
}

Boolean String::endsWith(const String& suffix) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (suffix.pImpl->size > pImpl->size) return false;
    return Memory::compare(pImpl->data.get() + (pImpl->size - suffix.pImpl->size), suffix.pImpl->data.get(), suffix.pImpl->size) == 0;
}

List<String> String::split(const String& delimiter) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    List<String> result;
    Size delimiterLength = delimiter.pImpl->size;
    if (delimiterLength == 0) {
        result.append(String(reinterpret_cast<const char*>(pImpl->data.get()), pImpl->size));
        return result;
    }

    const Byte* start = pImpl->data.get();
    const Byte* end = pImpl->data.get() + pImpl->size;
    const Byte* current = start;

    while (current < end) {
        const Byte* found = nullptr;
        for (Size i = 0; i <= (end - current - delimiterLength); ++i) {
            if (Memory::compare((Pointer) reinterpret_cast<CString>(current + i),
                                (Pointer) reinterpret_cast<CString>(delimiter.pImpl->data.get()), delimiterLength) == 0) {
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

Index String::find(const String& substring, Index startIndex) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    if (!pImpl->data.get() || !substring.pImpl->data.get()) return -1;

    List<Rune> runes;
    for (Size i = 0; i < pImpl->size; ) {
        Rune rune = Unicode::extractRuneAt(pImpl->data.get(), i);
        runes.append(rune);
        i += Unicode::calculateRuneLength(pImpl->data[i]);
    }

    List<Rune> subRunes;
    for (Size i = 0; i < substring.pImpl->size; ) {
        Rune rune = Unicode::extractRuneAt(substring.pImpl->data.get(), i);
        subRunes.append(rune);
        i += Unicode::calculateRuneLength(substring.pImpl->data[i]);
    }

    int thisSize = runes.size();
    int subSize = subRunes.size();

    if (subSize > thisSize) return -1;

    if (startIndex < 0) {
        startIndex = thisSize + startIndex;
        for (int i = startIndex; i >= 0; --i) {
            bool found = true;
            for (int j = 0; j < subSize; ++j) {
                if (i + j >= thisSize || runes[i + j] != subRunes[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return i;
        }
    } else {
        if (startIndex > thisSize - subSize) {
            return -1;
        }

        for (int i = startIndex; i <= thisSize - subSize; ++i) {
            bool found = true;
            for (int j = 0; j < subSize; ++j) {
                if (runes[i + j] != subRunes[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return i;
        }
    }

    return -1;
}

String& String::operator=(const String& other) {
    if (this != &other) {
        *pImpl = Memory::move(*other.pImpl);
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
    if (pImpl == nullptr || other.pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }
    Size newSize = this->pImpl->size + other.pImpl->size;
    Byte* newData = static_cast<Byte*>(Memory::allocate(newSize + 1));
    if (newData) {
        Memory::copy(newData, this->pImpl->data.get(), this->pImpl->size);
        Memory::copy(newData + this->pImpl->size, other.pImpl->data.get(), other.pImpl->size);
        newData[newSize] = '\0';
    }
    return {reinterpret_cast<CString>(newData), newSize};
}

String String::operator+=(const String& other) {
    *this = *this + other;
    return *this;
}

bool String::operator==(const String& other) const {
    if (pImpl == nullptr) {
        throw InvalidStateException("Attempt to use a moved-from String object.");
    }

    return this->pImpl->size == other.pImpl->size &&
           Memory::compare(this->pImpl->data.get(), other.pImpl->data.get(), this->pImpl->size) == 0;
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

CString String::rawString() const noexcept {
    return !pImpl ? nullptr : reinterpret_cast<CString>(pImpl->data.get());
}

Size String::rawLength() const noexcept {
    return !pImpl ? 0 : pImpl->size;
}