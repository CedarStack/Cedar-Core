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
#include "Cedar/Core/String.h"
#include "Cedar/Core/OutOfRangeException.h"
#include "Cedar/Core/Container/List.h"

#include "Cedar/Core/Math/Vector.h"

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

        EXPECT_EQ(temp[0], 2.4f);
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

    TEST(VectorTest, Length) {
        Math::Vec2f v2f(3.0f, 4.0f);
        EXPECT_FLOAT_EQ(v2f.length(), 5.0f);
    }
}
