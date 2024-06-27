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
#include <Cedar/Core/Exceptions/InvalidStateException.h>
#include <Cedar/Core/Exceptions/OutOfRangeException.h>
#include <Cedar/Core/Container/List.h>
#include <Cedar/Core/Container/Array.h>

#include <Cedar/Core/Text/Unicode.h>

using namespace Cedar::Core;
using namespace Cedar::Core::Container;
using namespace Cedar::Core::Text;

// Internal implementation class for encapsulating string data and operations
struct String::Impl {
    Memory::UniquePointer<Byte[]> data; // Pointer to string data
    Size size;                          // Byte length of the string
    Size runeCount;                     // Count of Unicode runes in the string

    Impl() : size(0), runeCount(0) {}

    Impl(CString str, Size len) : size(len), runeCount(0) {
        str = str ? str : "";
        data.reset(static_cast<Byte *>(Memory::allocate(len + 1)));
        Memory::copy(data.get(), (Byte *) str, len);
        data[len] = '\0'; // Null terminate for safety
        calculateRuneCount();
    }

    Impl(const Impl& other) : size(other.size), runeCount(other.runeCount) {
        if (size >= 0) {
            data.reset(static_cast<Byte*>(Memory::allocate(size + 1)));
            Memory::copy(data.get(), other.data.get(), size);
            data[size] = '\0';
        }
    }

    ~Impl() = default;

    void checkValidState() const {
        if (!data.get()) throw InvalidStateException("Invalid state: object has been moved or is uninitialized.");
    }

    void calculateRuneCount() {
        runeCount = 0;
        for (Size i = 0; i < size; i += Unicode::calculateRuneLength(data[i])) {
            ++runeCount;
        }
    }

    Impl& operator=(const Impl& other) {
        if (this != &other) {
            data.reset(new Byte[other.size + 1]);
            Memory::copy(data.get(), other.data.get(), other.size + 1);
            size = other.size;
            runeCount = other.runeCount;
        }
        return *this;
    }
};

String::String() : pImpl(new Impl("", 0)) {}

String::String(CString str) : String(str, Memory::calcCStringLength(str)) {}

String::String(CString str, Size len) : pImpl(new Impl(str, len)) {}

String::String(const Container::Array<Byte>& byteArray): String(reinterpret_cast<CString>(byteArray.data()), byteArray.size()) {}

String::String(Rune rune): String(Unicode::encodeRuneToString(rune)) {}

String::String(const String& other) : pImpl(new Impl(*other.pImpl)) {}

String::String(String&& other) noexcept : pImpl(std::move(other.pImpl)) {
    other.pImpl = nullptr;
}

String::~String() {
    delete pImpl;
}

void String::checkValidState() const {
    if (!pImpl)
        throw InvalidStateException("Attempt to use a moved-from String object.");
    pImpl->checkValidState();
}

Size String::length() const {
    checkValidState();
    return pImpl->runeCount;
}

Rune String::at(SSize index) const {
    checkValidState();

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

    return 0; // Should never reach here if checks are correct
}

Rune String::operator[](SSize index) const {
    return this->at(index);
}

String String::trimStart() const {
    checkValidState();

    Size i = 0;
    while (i < pImpl->size && Unicode::isSpace(pImpl->data[i])) {
        i += Unicode::calculateRuneLength(pImpl->data[i]); // Advance by rune, not byte
    }
    return {reinterpret_cast<CString>(pImpl->data.get() + i), pImpl->size - i};
}

String String::trimEnd() const {
    checkValidState();

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
    checkValidState();

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
    checkValidState();

    if (oldStr.pImpl->size == 0) {
        return *this;
    }

    String result;
    Size start = 0;
    Size foundPos = this->find(oldStr);

    while (foundPos != NPos) {
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
    return this->find(substring) != NPos;
}

Boolean String::startsWith(const String& prefix) const {
    checkValidState();

    if (prefix.pImpl->size > pImpl->size) return false;
    return Memory::compare(pImpl->data.get(), prefix.pImpl->data.get(), prefix.pImpl->size) == 0;
}

Boolean String::endsWith(const String& suffix) const {
    checkValidState();

    if (suffix.pImpl->size > pImpl->size) return false;
    return Memory::compare(pImpl->data.get() + (pImpl->size - suffix.pImpl->size), suffix.pImpl->data.get(), suffix.pImpl->size) == 0;
}

List<String> String::split(const String& delimiter) const {
    checkValidState();

    List<String> result;
    Size delimiterLength = delimiter.pImpl->size;
    if (delimiterLength == 0) {
        result.append(String(reinterpret_cast<CString>(pImpl->data.get()), pImpl->size));
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

SSize String::find(const String& substring, SSize startIndex) const {
    checkValidState();

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

    Size thisSize = runes.size();
    Size subSize = subRunes.size();

    if (subSize > thisSize) return NPos;

    if (startIndex < 0) {
        startIndex = thisSize + startIndex;
        for (Size i = startIndex; i >= 0; --i) {
            Boolean found = true;
            for (Size j = 0; j < subSize; ++j) {
                if (i + j >= thisSize || runes[i + j] != subRunes[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return i;
        }
    } else {
        if (startIndex > thisSize - subSize) {
            return NPos;
        }

        for (Size i = startIndex; i <= thisSize - subSize; ++i) {
            Boolean found = true;
            for (Size j = 0; j < subSize; ++j) {
                if (runes[i + j] != subRunes[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return i;
        }
    }

    return NPos;
}

String& String::operator=(const String& other) {
    if (this != &other) {
        Byte* srcData = other.pImpl->data.get();
        Size srcSize = other.pImpl->size;

        if (srcSize != pImpl->size) {
            pImpl->data.reset(new Byte[srcSize + 1]);
            pImpl->size = srcSize;
        }

        Byte* destData = pImpl->data.get();
        Memory::copy(destData, srcData, srcSize);
        destData[srcSize] = '\0';
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
    checkValidState();
    other.checkValidState();

    Size newSize = pImpl->size + other.pImpl->size;
    Byte* newData = static_cast<Byte*>(Memory::allocate(newSize + 1));

    Memory::copy(newData, pImpl->data.get(), pImpl->size);
    Memory::copy(newData + pImpl->size, other.pImpl->data.get(), other.pImpl->size);
    newData[newSize] = '\0';

    return {reinterpret_cast<CString>(newData), newSize};
}

Boolean String::operator==(const String& other) const {
    checkValidState();
    other.checkValidState();

    return pImpl->size == other.pImpl->size &&
           Memory::compare(pImpl->data.get(), other.pImpl->data.get(), pImpl->size) == 0;
}

Boolean String::operator!=(const String& other) const {
    this->checkValidState();
    other.checkValidState();

    return !(*this == other);
}

Array<Byte> String::toBytes() const {
    checkValidState();
    return Array<Byte>(pImpl->data.get(), pImpl->size);
}

CString String::rawString() const {
    checkValidState();
    return reinterpret_cast<CString>(pImpl->data.get());
}

Size String::rawLength() const {
    checkValidState();
    return pImpl->size;
}

Array<wchar_t> String::toWCString() const {
    checkValidState();

#ifdef _WIN32
    Size utf16Len = 0;
    Size length = this->length();
    for (Size i = 0; i < length; ++i) {
        Rune rune = this->at(i);
        utf16Len += (rune <= 0xFFFF) ? 1 : 2;
    }

    Array<wchar_t> wstr(utf16Len + 1);

    Size wstrIndex = 0;
    for (Size i = 0; i < length; ++i) {
        Rune rune = this->at(i);
        if (rune <= 0xFFFF) {
            wstr[wstrIndex++] = static_cast<wchar_t>(rune);
        } else {
            rune -= 0x10000;
            wstr[wstrIndex++] = static_cast<wchar_t>((rune >> 10) + 0xD800);  // 高代理项
            wstr[wstrIndex++] = static_cast<wchar_t>((rune & 0x3FF) + 0xDC00);  // 低代理项
        }
    }
    wstr[wstrIndex] = L'\0';

#else
    Size length = this->length();

    Array<wchar_t> wstr(length + 1);

    for (Size i = 0; i < length; ++i) {
        Rune rune = this->at(i);
        wstr[i] = static_cast<wchar_t>(rune);
    }
    wstr[length] = L'\0';
#endif

    return wstr;
}
