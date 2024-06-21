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
    }

    class String {
    public:
        /**
         *  Default constructor.
         */
        String();

        //NOLINTNEXTLINE
        /**
         * constructor.
         *
         * @param string Text value, use Char* as input.
         */
        String(CString string);

        /**
         * constructor.
         * String length can be limited.
         *
         * @param str Text value.
         * @param len A string length value,
         */
        String(CString str, Size len);

        /**
         * constructor.
         *
         * @param other Text value, use String as input.
         */
        String(const String &other);

        /**
         * constructor.
         *
         * @param other
         */
        String(String &&other) noexcept;

        /**
         * constructor.
         *
         * @param rune Rune value, use Char as input.
         */
        String(Rune rune);

        /**
         * analytic groove function.
         */
        ~String();

        [[nodiscard]] Size length() const;

        /**
         * Get the character at index.
         *
         * @param index Need character index.
         * @return A char vaule.
         */
        [[nodiscard]] Rune at(Index index) const;

        [[nodiscard]] Rune operator[](Index index) const;

        /**
         * Remove string head and tail space.
         *
         * @return String value
         */
        [[nodiscard]] String trim() const;

        /**
         * Remove string head space.
         *
         * @return String value
         */
        [[nodiscard]] String trimStart() const;

        /**
         * Remove string tail space.
         *
         * @return String value
         */
        [[nodiscard]] String trimEnd() const;

        /**
         * The form head starts remove The string.
         *
         * @param prefix String to replace.
         * @return String value.
         */
        [[nodiscard]] String stripPrefix(const String &prefix) const;

        /**
         * The form tail starts remove The string.
         *
         * @param prefix String to replace.
         * @return String value.
         */
        [[nodiscard]] String stripSuffix(const String &suffix) const;


        /**
         * Create a new sub string.
         * @param start
         * @param len
         * @return String value.
         */
        [[nodiscard]] String substring(Size start, Size len = NPos) const;

        /**
         * String replace.
         *
         * @param oldStr 
         * @param newStr
         * @return String value.
         */
        [[nodiscard]] String replace(const String &oldStr, const String &newStr) const;

        /**
         * Is string in current string
         * @param substring
         * @return String value.
         */
        [[nodiscard]] Boolean contains(const String &substring) const;

        /**
         * Is there string at current string head.
         *
         * @param prefix
         * @return String value.
         */
        [[nodiscard]] Boolean startsWith(const String &prefix) const;

         /**
         * Is there string at current string tail.
         *
         * @param prefix
         * @return String value.
         */
        [[nodiscard]] Boolean endsWith(const String &suffix) const;

        /**
         * Split this String, return some String values.
         *
         * @param delimiter String as delimiter.
         * @return String values.
         */
        [[nodiscard]] Container::List<String> split(const String &delimiter) const;

        /**
         * Get all line as lines list.
         * @return String lines list.
         */
        [[nodiscard]] Container::List<String> getLines() const;

        /**
         * Find string form start index.
         * @param substring
         * @param startIndex
         * @return Index value.
         */
        [[nodiscard]] Index find(const String &substring, Index startIndex = 0) const;

        String &operator=(const String &other);

        String &operator=(String &&other) noexcept;

        String operator+(const String &other) const;

        Boolean operator==(const String &other) const;

        Boolean operator!=(const String &other) const;

        /**
         * String value to CString value.
         * @return Return CString value.
         */
        [[nodiscard]] CString rawString() const noexcept;

        /**
         * Is white space in input Rune value.
         * @param rune
         * @return
         */
        static Boolean isWhitespace(Rune rune);

        static const Index NPos = -1;

    private:
        struct Impl;
        Impl *pImpl;
    };
} // namespace Cedar::Core
