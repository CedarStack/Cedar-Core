#include <gtest/gtest.h>
#include <Cedar/Core/String.h>
#include <Cedar/Core/OutOfRangeException.h>
#include <Cedar/Core/Container/List.h>

#include <Cedar/Core/Math/Vector.h>

namespace Cedar::Core {

// Vector2 float
    TEST(VectorTest, DefaultConstructor) {
        Math::Vec2f v2f{};
        EXPECT_EQ(v2f[0], 0.0f);
    }

    TEST(VectorTest, ArgsConstructor) {
        Math::Vec2f v2f(1.0f, 3.2f);
        EXPECT_EQ(v2f[0], 1.0f);
        EXPECT_EQ(v2f[1], 3.2f);
    }

    TEST(VectorTest, PositiveOp) {
        Math::Vec2f v2f(-2.4f, 6.7f);

        Math::Vec2f temp = +v2f;

        EXPECT_EQ(temp[0], 2.4f);
        EXPECT_EQ(temp[1], 6.7f);
    }

    TEST(VectorTest, NegativeOp) {
        Math::Vec2f v2f(-2.4f, 6.7f);

        Math::Vec2f temp = -v2f;

        EXPECT_EQ(temp[0], -2.4f);
        EXPECT_EQ(temp[1], -6.7f);
    }

    TEST(VectorTest, AddOp) {
        Math::Vec2f v2f_0(-2.4f, 6.7f);
        Math::Vec2f v2f_1(-12.73f, -7.31f);

        Math::Vec2f temp = v2f_0 + v2f_1;

        EXPECT_EQ(temp[0], (-2.4f + -12.73f));
        EXPECT_EQ(temp[1], (6.7f + -7.31f));
    }

    TEST(VectorTest, SubOp) {
        Math::Vec2f v2f_0(-2.4f, 6.7f);
        Math::Vec2f v2f_1(-12.73f, -7.31f);

        Math::Vec2f temp = v2f_0 - v2f_1;

        EXPECT_EQ(temp[0], (-2.4f - -12.73f));
        EXPECT_EQ(temp[1], (6.7f - -7.31f));
    }

    TEST(VectorTest, MulOp) {
        Math::Vec2f v2f_0(-2.4f, 6.7f);
        Math::Vec2f v2f_1(-12.73f, -7.31f);

        Math::Vec2f temp = v2f_0 * v2f_1;

        EXPECT_EQ(temp[0], (-2.4f * -12.73f));
        EXPECT_EQ(temp[1], (6.7f * -7.31f));
    }

    TEST(VectorTest, DivOp) {
        Math::Vec2f v2f_0(-2.4f, 6.7f);
        Math::Vec2f v2f_1(-12.73f, -7.31f);

        Math::Vec2f temp = v2f_0 / v2f_1;

        EXPECT_EQ(temp[0], (-2.4f / -12.73f));
        EXPECT_EQ(temp[1], (6.7f / -7.31f));
    }

    TEST(VectorTest, EqualOp) {
        Math::Vec2f v2f_0(-2.4f, 6.7f);
        Math::Vec2f v2f_1(-12.73f, -7.31f);
        Boolean temp = v2f_0 == v2f_1;
        EXPECT_EQ(temp, false);

        Math::Vec2f v2f_2(-2.4f, 6.7f);
        Math::Vec2f v2f_3(-2.4f, 6.7f);
        Boolean temp_1 = v2f_2 == v2f_3;
        EXPECT_EQ(temp_1, true);
    }

    TEST(VectorTest, InverseEqualOp) {
        Math::Vec2f v2f_0(-2.4f, 6.7f);
        Math::Vec2f v2f_1(-12.73f, -7.31f);
        Boolean temp = v2f_0 != v2f_1;
        EXPECT_EQ(temp, true);

        Math::Vec2f v2f_2(-2.4f, 6.7f);
        Math::Vec2f v2f_3(-2.4f, 6.7f);
        Boolean temp_1 = v2f_2 != v2f_3;
        EXPECT_EQ(temp_1, false);
    }

}
