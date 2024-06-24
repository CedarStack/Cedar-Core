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

#include <gtest/gtest.h>
#include <Cedar/Core/Text/Unicode.h>

namespace Cedar::Core::Text {

    // Tests whether various characters are correctly identified as letters.
    TEST(UnicodeTest, IsLetter) {
        // Arabic letter
        EXPECT_TRUE(Unicode::isLetter(0x0627));  // 'ا' (Arabic letter Alef)

        // Greek letters
        EXPECT_TRUE(Unicode::isLetter(0x0391));  // 'Α' (Greek Capital Letter Alpha)
        EXPECT_TRUE(Unicode::isLetter(0x03B1));  // 'α' (Greek Small Letter Alpha)

        // French letter with diacritic
        EXPECT_TRUE(Unicode::isLetter(0x00E9));  // 'é' (French letter é)

        // Cyrillic letters
        EXPECT_TRUE(Unicode::isLetter(0x0410));  // 'А' (Cyrillic Capital Letter A)
        EXPECT_TRUE(Unicode::isLetter(0x0430));  // 'а' (Cyrillic Small Letter a)

        // Non-letter characters
        EXPECT_FALSE(Unicode::isLetter(0x0031)); // '1'
        EXPECT_FALSE(Unicode::isLetter(0x0020)); // Space character
        EXPECT_FALSE(Unicode::isLetter(0x4E16)); // Chinese character '世'
    }

    // Tests whether various characters are correctly identified as digits.
    TEST(UnicodeTest, IsDigit) {
        // Arabic numerals
        EXPECT_TRUE(Unicode::isDigit(0x0031));   // '1'
        EXPECT_TRUE(Unicode::isDigit(0x0661));   // '١' (Arabic-Indic Digit One)

        // Roman numeral
        EXPECT_FALSE(Unicode::isDigit(0x2160));  // 'Ⅰ' (Roman Numeral One)

        // Non-digit characters
        EXPECT_FALSE(Unicode::isDigit(0x0041));  // 'A'
        EXPECT_FALSE(Unicode::isDigit(0x0061));  // 'a'
        EXPECT_FALSE(Unicode::isDigit(0x0020));  // Space character
        EXPECT_FALSE(Unicode::isDigit(0x4E16));  // Chinese character '世'
    }

    // Tests whether various characters are correctly identified as whitespace.
    TEST(UnicodeTest, IsSpace) {
        // Common whitespace characters
        EXPECT_TRUE(Unicode::isSpace(0x0020));   // Space character
        EXPECT_TRUE(Unicode::isSpace(0x0009));   // Horizontal tab
        EXPECT_TRUE(Unicode::isSpace(0x000A));   // Line feed
        EXPECT_TRUE(Unicode::isSpace(0x000D));   // Carriage return

        // Non-space characters
        EXPECT_FALSE(Unicode::isSpace(0x0041));  // 'A'
        EXPECT_FALSE(Unicode::isSpace(0x0061));  // 'a'
        EXPECT_FALSE(Unicode::isSpace(0x0031));  // '1'
        EXPECT_FALSE(Unicode::isSpace(0x4E16));  // Chinese character '世'
    }

    // Tests conversion of characters to uppercase.
    TEST(UnicodeTest, ToUpperCase) {
        // ASCII characters
        EXPECT_EQ(Unicode::toUpperCase(0x0041), 0x0041); // 'A' to 'A'
        EXPECT_EQ(Unicode::toUpperCase(0x0061), 0x0041); // 'a' to 'A'

        // Greek letters
        EXPECT_EQ(Unicode::toUpperCase(0x03B1), 0x0391); // 'α' to 'Α'

        // French letter with diacritic
        EXPECT_EQ(Unicode::toUpperCase(0x00E9), 0x00C9); // 'é' to 'É'

        // Cyrillic letters
        EXPECT_EQ(Unicode::toUpperCase(0x0430), 0x0410); // 'а' to 'А'

        // Non-letter characters unchanged
        EXPECT_EQ(Unicode::toUpperCase(0x0031), 0x0031); // '1' unchanged
        EXPECT_EQ(Unicode::toUpperCase(0x4E16), 0x4E16); // Chinese character '世' unchanged
    }

    // Tests conversion of characters to lowercase.
    TEST(UnicodeTest, ToLowerCase) {
        // ASCII characters
        EXPECT_EQ(Unicode::toLowerCase(0x0041), 0x0061); // 'A' to 'a'
        EXPECT_EQ(Unicode::toLowerCase(0x0061), 0x0061); // 'a' to 'a'

        // Greek letters
        EXPECT_EQ(Unicode::toLowerCase(0x0391), 0x03B1); // 'Α' to 'α'

        // French letter with diacritic
        EXPECT_EQ(Unicode::toLowerCase(0x00C9), 0x00E9); // 'É' to 'é'

        // Cyrillic letters
        EXPECT_EQ(Unicode::toLowerCase(0x0410), 0x0430); // 'А' to 'а'

        // Non-letter characters unchanged
        EXPECT_EQ(Unicode::toLowerCase(0x0031), 0x0031); // '1' unchanged
        EXPECT_EQ(Unicode::toLowerCase(0x4E16), 0x4E16); // Chinese character '世' unchanged
    }
}
