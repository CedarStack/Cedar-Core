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
#include <Cedar/Core/Container/List.h>

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

    Rune decodeRuneAt(Size i) const {
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

    static Size calcCStringLength(const char* str) {
        Size len = 0;
        while (str[len] != '\0') {
            len++;
        }
        return len;
    }

    static void copyCString(char* dest, const char* src) {
        while ((*dest++ = *src++));
    }

    List<String> split(const String& delimiter) const {
        List<String> result;
        Size delimiterLength = delimiter.pImpl->size;
        if (delimiterLength == 0) {
            result.append(String(reinterpret_cast<const char*>(data), size));
            return result;
        }

        const Byte* start = data;
        const Byte* end = data + size;
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

Rune String::at(Index index) const {
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

CString String::rawString() const noexcept {
    return pImpl != nullptr ?
        reinterpret_cast<CString>(pImpl->data) : nullptr;
}

List<String> String::split(const String& delimiter) const {
    return pImpl->split(delimiter);
}

String String::operator+(const String& other) const {
    Size newSize = this->pImpl->size + other.pImpl->size;
    Byte* newData = static_cast<Byte*>(Memory::allocate(newSize + 1));
    Memory::copy(newData, this->pImpl->data, this->pImpl->size);
    Memory::copy(newData + this->pImpl->size, other.pImpl->data, other.pImpl->size);
    newData[newSize] = '\0';
    return String(reinterpret_cast<CString>(newData), newSize);
}

bool String::operator==(const String& other) const {
    return this->pImpl->size == other.pImpl->size &&
           Memory::compare(this->pImpl->data, other.pImpl->data, this->pImpl->size) == 0;
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}