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
#include <Cedar/Core/String.h>
#include <Cedar/Core/Container/List.h>
#include <Cedar/Core/Exceptions/InvalidStateException.h>

namespace Cedar::Core {

    // Tests default constructor of the String class.
    TEST(StringTest, DefaultConstructor) {
        String str;
        EXPECT_EQ(str.length(), 0);
        EXPECT_TRUE(str.rawString() == nullptr);
    }

    // Tests the constructor that initializes with a C-style string.
    TEST(StringTest, ConstructFromCString) {
        String str("hello");
        EXPECT_EQ(str.length(), 5);
        EXPECT_STREQ(str.rawString(), "hello");
    }

    // Tests the copy constructor for correctness.
    TEST(StringTest, CopyConstructor) {
        String original("test");
        String copy(original);
        EXPECT_EQ(copy, original);
    }

    // Tests the move constructor ensuring the original object's state post-move.
    TEST(StringTest, MoveConstructor) {
        String original("move");
        String moved(std::move(original));

        EXPECT_EQ(moved.length(), 4);
        EXPECT_TRUE(original.rawString() == nullptr);

        // Verify InvalidStateException is thrown when using the moved-from object.
        EXPECT_THROW(original.length(), InvalidStateException);
    }

    // Tests handling of Unicode characters in strings.
    TEST(StringTest, UnicodeHandling) {
        String unicode("你好🌍");
        EXPECT_EQ(unicode.length(), 3);
        EXPECT_STREQ(unicode.rawString(), "你好🌍");
    }

    // Tests proper counting and retrieval of emojis as single characters.
    TEST(StringTest, EmojiHandling) {
        String emoji("😊👍");
        EXPECT_EQ(emoji.length(), 2);
        EXPECT_STREQ(emoji.rawString(), "😊👍");
    }

    // Tests string access using both positive and negative indices.
    TEST(StringTest, AccessWithPositiveAndNegativeIndices) {
        String testStr(u8"Hello, 世界! 👋");

        // Positive indices
        EXPECT_EQ(testStr.at(0), 'H');
        EXPECT_EQ(testStr.at(7), u'世');
        EXPECT_EQ(testStr.at(11), U'👋');

        // Negative indices
        EXPECT_EQ(testStr.at(-1), U'👋');
        EXPECT_EQ(testStr.at(-3), '!');
        EXPECT_EQ(testStr.at(-4), u'界');

        // Out-of-range access
        EXPECT_THROW(testStr.at(testStr.length()), OutOfRangeException);
        EXPECT_THROW(testStr.at(-testStr.length() - 1), OutOfRangeException);
    }

    // Tests various string manipulation operations.
    TEST(StringTest, StringOperations) {
        String s("  trim me  ");
        EXPECT_EQ(s.trim(), "trim me");

        String prefix("unimportant");
        String important("important");
        EXPECT_TRUE(important.startsWith(prefix.stripPrefix("un")));
        EXPECT_TRUE(important.endsWith("tant"));
    }

    // Tests string splitting into substrings.
    TEST(StringTest, SplitStrings) {
        String s("one,two,three");
        auto parts = s.split(",");
        ASSERT_EQ(parts.size(), 3);
        EXPECT_EQ(parts[0], "one");
        EXPECT_EQ(parts[1], "two");
        EXPECT_EQ(parts[2], "three");
    }

    // Tests substring extraction with Unicode characters.
    TEST(StringTest, SubstringUnicode) {
        String text("Hello, 世界🌏!");

        String sub = text.substring(7, 3);
        ASSERT_STREQ(sub.rawString(), "世界🌏");

        sub = text.substring(9, 2);
        ASSERT_STREQ(sub.rawString(), "🌏!");

        sub = text.substring(0, 5);
        ASSERT_STREQ(sub.rawString(), "Hello");

        EXPECT_THROW(text.substring(15, 1), OutOfRangeException);
        sub = text.substring(7, 10);
        ASSERT_STREQ(sub.rawString(), "世界🌏!");
    }

    // Tests string replacement functionality.
    TEST(StringTest, ReplaceMethod) {
        String original("hello world");
        String result = original.replace("world", "there");
        ASSERT_EQ(result, "hello there");

        result = original.replace("test", "there");
        ASSERT_EQ(result, "hello world");

        result = original.replace("", "there");
        ASSERT_EQ(result, "hello world");

        result = original.replace("world", "everyone here");
        ASSERT_EQ(result, "hello everyone here");

        result = original.replace("world", "all");
        ASSERT_EQ(result, "hello all");

        result = original.replace("world", "🌏");
        ASSERT_EQ(result, "hello 🌏");
    }

    // Tests string search and position finding.
    TEST(StringFindTest, Find) {
        String asciiStr("Hello, World!");
        EXPECT_EQ(asciiStr.find(String("World")), 7);
        EXPECT_EQ(asciiStr.find(String("world")), -1);
        EXPECT_EQ(asciiStr.find(String("Hello")), 0);
        EXPECT_EQ(asciiStr.find(String("Z")), -1);

        String chineseStr("你好，世界！");
        EXPECT_EQ(chineseStr.find(String("你好")), 0);
        EXPECT_EQ(chineseStr.find(String("，世界")), 2);
        EXPECT_EQ(chineseStr.find(String("地球")), -1);

        String emojiStr("🚀🌕🌟");
        EXPECT_EQ(emojiStr.find(String("🚀")), 0);
        EXPECT_EQ(emojiStr.find(String("🌕")), 1);
        EXPECT_EQ(emojiStr.find(String("⭐")), -1);

        String pathStr("/usr/bin/bash");
        EXPECT_EQ(pathStr.find(String("/"), -1), 8);
        EXPECT_EQ(pathStr.find(String("/"), 1), 4);
        EXPECT_EQ(pathStr.find(String("/"), 0), 0);
    }
}
