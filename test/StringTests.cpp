#include <gtest/gtest.h>
#include <Cedar/Core/String.h>
#include <Cedar/Core/OutOfRangeException.h>
#include <Cedar/Core/Container/List.h>

namespace Cedar::Core {

    TEST(StringTest, DefaultConstructor) {
        String s;
        EXPECT_EQ(s.rawString(), nullptr);  // 默认构造的String应为空
    }

    TEST(StringTest, ConstructFromStringLiteral) {
        String s("Hello");
        EXPECT_STREQ(s.rawString(), "Hello");
    }

    TEST(StringTest, CopyConstructor) {
        String original("Hello");
        String copy(original);
        EXPECT_STREQ(copy.rawString(), original.rawString());
    }

    TEST(StringTest, MoveConstructor) {
        String original("Hello");
        String moved(std::move(original));
        EXPECT_STREQ(moved.rawString(), "Hello");
        EXPECT_EQ(original.rawString(), nullptr);
    }

    TEST(StringTest, AccessOperator) {
        String s("Hello");
        EXPECT_EQ(s[0], 'H');
        EXPECT_EQ(s.at(0), 'H');
        EXPECT_THROW(s.at(5), OutOfRangeException);
    }

    TEST(StringTest, Concatenation) {
        String s1("Hello");
        String s2(" World");
        String result = s1 + s2;
        EXPECT_STREQ(result.rawString(), "Hello World");
    }

    TEST(StringTest, Equality) {
        String s1("test");
        String s2("test");
        String s3("Test");
        EXPECT_TRUE(s1 == s2);
        EXPECT_FALSE(s1 == s3);
    }

    TEST(StringTest, Inequality) {
        String s1("test");
        String s2("Test");
        EXPECT_TRUE(s1 != s2);
    }

    TEST(StringTest, UnicodeOprtations) {
        String s1("你好，🌏");
        auto parts = s1.split("，");
        EXPECT_EQ(parts[0], "你好");
        EXPECT_EQ(parts[1], "🌏");
    }
}  // namespace Cedar::Core
