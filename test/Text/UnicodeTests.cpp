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
    TEST(UnicodeTest, IsLetter) {
        // 阿拉伯字母
        EXPECT_TRUE(Unicode::isLetter(0x0627));  // 'ا' (阿拉伯字母)

        // 希腊字母
        EXPECT_TRUE(Unicode::isLetter(0x0391));  // 'Α' (希腊大写字母 Alpha)
        EXPECT_TRUE(Unicode::isLetter(0x03B1));  // 'α' (希腊小写字母 Alpha)

        // 法语字母
        EXPECT_TRUE(Unicode::isLetter(0x00E9));  // 'é' (法语字母)

        // 西里尔字母
        EXPECT_TRUE(Unicode::isLetter(0x0410));  // 'А' (西里尔大写字母 A)
        EXPECT_TRUE(Unicode::isLetter(0x0430));  // 'а' (西里尔小写字母 a)

        // 非字母
        EXPECT_FALSE(Unicode::isLetter(0x0031)); // '1'
        EXPECT_FALSE(Unicode::isLetter(0x0020)); // ' '
        EXPECT_FALSE(Unicode::isLetter(0x4E16)); // '世'
    }

    TEST(UnicodeTest, IsDigit) {
        // 阿拉伯数字
        EXPECT_TRUE(Unicode::isDigit(0x0031));   // '1'
        EXPECT_TRUE(Unicode::isDigit(0x0661));   // '١' (阿拉伯-印度数字 1)

        // 罗马数字
        EXPECT_FALSE(Unicode::isDigit(0x2160));  // 'Ⅰ' (罗马数字 1)

        // 非数字
        EXPECT_FALSE(Unicode::isDigit(0x0041));  // 'A'
        EXPECT_FALSE(Unicode::isDigit(0x0061));  // 'a'
        EXPECT_FALSE(Unicode::isDigit(0x0020));  // ' '
        EXPECT_FALSE(Unicode::isDigit(0x4E16));  // '世'
    }

    TEST(UnicodeTest, IsSpace) {
        // 常见空白字符
        EXPECT_TRUE(Unicode::isSpace(0x0020));   // ' '
        EXPECT_TRUE(Unicode::isSpace(0x0009));   // '\t' (Tab)
        EXPECT_TRUE(Unicode::isSpace(0x000A));   // '\n' (Newline)
        EXPECT_TRUE(Unicode::isSpace(0x000D));   // '\r' (Carriage Return)

        // 非空白字符
        EXPECT_FALSE(Unicode::isSpace(0x0041));  // 'A'
        EXPECT_FALSE(Unicode::isSpace(0x0061));  // 'a'
        EXPECT_FALSE(Unicode::isSpace(0x0031));  // '1'
        EXPECT_FALSE(Unicode::isSpace(0x4E16));  // '世'
    }

    TEST(UnicodeTest, ToUpperCase) {
        // ASCII
        EXPECT_EQ(Unicode::toUpperCase(0x0041), 0x0041); // 'A' -> 'A'
        EXPECT_EQ(Unicode::toUpperCase(0x0061), 0x0041); // 'a' -> 'A'

        // 希腊字母
        EXPECT_EQ(Unicode::toUpperCase(0x03B1), 0x0391); // 'α' -> 'Α'

        // 法语字母
        EXPECT_EQ(Unicode::toUpperCase(0x00E9), 0x00C9); // 'é' -> 'É'

        // 西里尔字母
        EXPECT_EQ(Unicode::toUpperCase(0x0430), 0x0410); // 'а' -> 'А'

        // 非字母字符
        EXPECT_EQ(Unicode::toUpperCase(0x0031), 0x0031); // '1' -> '1'
        EXPECT_EQ(Unicode::toUpperCase(0x4E16), 0x4E16); // '世' -> '世' (unchanged)
    }

    TEST(UnicodeTest, ToLowerCase) {
        // ASCII
        EXPECT_EQ(Unicode::toLowerCase(0x0041), 0x0061); // 'A' -> 'a'
        EXPECT_EQ(Unicode::toLowerCase(0x0061), 0x0061); // 'a' -> 'a'

        // 希腊字母
        EXPECT_EQ(Unicode::toLowerCase(0x0391), 0x03B1); // 'Α' -> 'α'

        // 法语字母
        EXPECT_EQ(Unicode::toLowerCase(0x00C9), 0x00E9); // 'É' -> 'é'

        // 西里尔字母
        EXPECT_EQ(Unicode::toLowerCase(0x0410), 0x0430); // 'А' -> 'а'

        // 非字母字符
        EXPECT_EQ(Unicode::toLowerCase(0x0031), 0x0031); // '1' -> '1'
        EXPECT_EQ(Unicode::toLowerCase(0x4E16), 0x4E16); // '世' -> '世' (unchanged)
    }
}