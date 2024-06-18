//
// Created by itshu on 18/6/2024.
//

#include <Cedar/Core/String.h>
#include <Cedar/Core/Memory.h>

using namespace Cedar::Core;
//using namespace Cedar::Core::Container;

struct String::Impl {
    Byte* data;
    Size size;

    Impl(): data(nullptr), size(0) {}

    explicit Impl(CString str) {
        if (str) {
            size = Memory::calcCStringLength(str);
            data = static_cast<Byte *>(Memory::allocate(size + 1));
            Memory::copyCString(reinterpret_cast<CString>(data), str);
        } else {
            size = 0;
            data = nullptr;
        }
    }

    Impl(const Impl& other) {
        if (other.data) {
            size = other.size;
            data = static_cast<Byte *>(Memory::allocate(size + 1));
            Memory::copyCString(reinterpret_cast<CString>(data), reinterpret_cast<CString>(other.data));
        } else {
            data = nullptr;
            size = 0;
        }
    }

    ~Impl() {
        Memory::release(data);
    }

    Impl& operator=(const Impl& other) {
        if (this != &other) {
            Byte* newData = new Byte[other.size + 1];
            Memory::copyCString(reinterpret_cast<CString>(data), reinterpret_cast<CString>(other.data));
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
};

String::String() : pImpl(new Impl()) {}

String::String(const char* str) : pImpl(new Impl(str)) {}

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

Rune String::operator[](Index index) const {
    return this->at(index);
}

Rune String::at(Index index) const {
    if (!pImpl->data || index >= pImpl->size) return 0; // Return 0 for out of range

    Size charCount = 0;
    Size i = 0;
    while (i < pImpl->size) {
        if (charCount == index) {
            return pImpl->decodeRuneAt(i);
        }
        i += Cedar::Core::String::Impl::bytesInRune(pImpl->data[i]);
        charCount++;
    }

    return 0;
}
