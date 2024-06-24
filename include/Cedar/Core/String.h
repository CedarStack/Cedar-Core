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

namespace Cedar::Core {
    namespace Container {
        template<typename T>
        class List;

        template<typename T>
        class Array;
    }

    class String {
    public:
        String();

        //NOLINTNEXTLINE
        String(CString string);
        String(CString str, Size len);
        String(const Container::Array<Byte>& byteArray);
        String(const String& other);
        String(String&& other) noexcept;
        String(Rune rune);

        ~String();

        [[nodiscard]] Size length() const;

        [[nodiscard]] Rune at(Index index) const;
        [[nodiscard]] Rune operator[](Index index) const;

        [[nodiscard]] String trim() const;
        [[nodiscard]] String trimStart() const;
        [[nodiscard]] String trimEnd() const;
        [[nodiscard]] String stripPrefix(const String& prefix) const;
        [[nodiscard]] String stripSuffix(const String& suffix) const;
        [[nodiscard]] String substring(Size start, Size len = NPos) const;
        [[nodiscard]] String replace(const String& oldStr, const String& newStr) const;

        [[nodiscard]] Boolean contains(const String& substring) const;
        [[nodiscard]] Boolean startsWith(const String& prefix) const;
        [[nodiscard]] Boolean endsWith(const String& suffix) const;

        [[nodiscard]] Container::List<String> split(const String& delimiter) const;
        [[nodiscard]] Container::List<String> getLines() const;
        [[nodiscard]] Index find(const String& substring, Index startIndex = 0) const;

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;
        String operator+(const String& other) const;
        String operator+=(const String& other);
        Boolean operator==(const String& other) const;
        Boolean operator!=(const String& other) const;

        [[nodiscard]] Container::Array<Byte> toBytes() const;

        [[nodiscard]] CString rawString() const;
        [[nodiscard]] Size rawLength() const;

        [[nodiscard]] Container::Array<wchar_t> toWCString() const;

        static const Index NPos = -1;

// TODO
//        class Iterator {
//        public:
//            Iterator(const Byte* ptr, const Byte* end);
//            Iterator& operator++();
//            Iterator operator++(int);
//            Rune operator*() const ;
//            bool operator!=(const Iterator& other) const;
//            bool operator==(const Iterator& other) const;
//        private:
//            const Byte* ptr;
//            const Byte* end;
//        };
//        Iterator begin() const;
//        Iterator end() const;

    private:
        struct Impl;
        Impl* pImpl;
    };
}
