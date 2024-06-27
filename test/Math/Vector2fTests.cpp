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

#include <Cedar/Core/Math/Vector.h>

namespace Cedar::Core::Math {

    // Test the default constructor
    TEST(VectorTest, DefaultConstructor) {
        Vec2f v2f{};
        EXPECT_FLOAT_EQ(v2f[0], 0.0f);
        EXPECT_FLOAT_EQ(v2f[1], 0.0f); // Added to test the second component as well
    }

    // Test the parameterized constructor
    TEST(VectorTest, ArgsConstructor) {
        Vec2f v2f(1.0f, 3.2f);
        EXPECT_FLOAT_EQ(v2f[0], 1.0f);
        EXPECT_FLOAT_EQ(v2f[1], 3.2f);
    }

    // Test addition operator
    TEST(Vector3dTest, AddOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 24.3);
        Math::Vec3d v3d_1(-12.73, -7.31, 337.21);

        Math::Vec3d temp = v3d_0 + v3d_1;

        EXPECT_NEAR(temp[0], -15.13, 0.00001);
        EXPECT_NEAR(temp[1], -0.61, 0.00001);
        EXPECT_NEAR(temp[2], 361.51, 0.00001);
    }

    // Test unary negative operator
    TEST(Vector3dTest, NegativeOp) {
        Math::Vec3d v3d(-2.4, 6.7, -24.2);

        Math::Vec3d temp = -v3d;

        EXPECT_NEAR(temp[0], 2.4, 0.00001);
        EXPECT_NEAR(temp[1], -6.7, 0.00001);
        EXPECT_NEAR(temp[2], 24.2, 0.00001);
    }

    // Test addition operator
    TEST(VectorTest, AddOp) {
        Vec2f v2f_0(-2.4f, 6.7f);
        Vec2f v2f_1(-12.73f, -7.31f);

        Vec2f temp = v2f_0 + v2f_1;

        EXPECT_FLOAT_EQ(temp[0], (-2.4f + -12.73f));
        EXPECT_FLOAT_EQ(temp[1], (6.7f + -7.31f));
    }

    // Test subtraction operator
    TEST(VectorTest, SubOp) {
        Vec2f v2f_0(-2.4f, 6.7f);
        Vec2f v2f_1(-12.73f, -7.31f);

        Vec2f temp = v2f_0 - v2f_1;

        EXPECT_FLOAT_EQ(temp[0], (-2.4f - -12.73f));
        EXPECT_FLOAT_EQ(temp[1], (6.7f - -7.31f));
    }

    // Test multiplication operator (component-wise multiplication)
    TEST(VectorTest, MulOp) {
        Vec2f v2f_0(-2.4f, 6.7f);
        Vec2f v2f_1(-12.73f, -7.31f);

        Vec2f temp = v2f_0 * v2f_1;

        EXPECT_FLOAT_EQ(temp[0], (-2.4f * -12.73f));
        EXPECT_FLOAT_EQ(temp[1], (6.7f * -7.31f));
    }

    // Test division operator (component-wise division)
    TEST(VectorTest, DivOp) {
        Vec2f v2f_0(-2.4f, 6.7f);
        Vec2f v2f_1(-12.73f, -7.31f);

        Vec2f temp = v2f_0 / v2f_1;

        EXPECT_FLOAT_EQ(temp[0], (-2.4f / -12.73f));
        EXPECT_FLOAT_EQ(temp[1], (6.7f / -7.31f));
    }

    // Test equality operator
    TEST(VectorTest, EqualOp) {
        Vec2f v2f_0(-2.4f, 6.7f);
        Vec2f v2f_1(-12.73f, -7.31f);
        Boolean temp = v2f_0 == v2f_1;
        EXPECT_FALSE(temp);

        Vec2f v2f_2(-2.4f, 6.7f);
        Vec2f v2f_3(-2.4f, 6.7f);
        Boolean temp_1 = v2f_2 == v2f_3;
        EXPECT_TRUE(temp_1);
    }

    // Test inequality operator
    TEST(VectorTest, InverseEqualOp) {
        Vec2f v2f_0(-2.4f, 6.7f);
        Vec2f v2f_1(-12.73f, -7.31f);
        Boolean temp = v2f_0 != v2f_1;
        EXPECT_TRUE(temp);

        Vec2f v2f_2(-2.4f, 6.7f);
        Vec2f v2f_3(-2.4f, 6.7f);
        Boolean temp_1 = v2f_2 != v2f_3;
        EXPECT_FALSE(temp_1);
    }

    // Test length calculation
    TEST(VectorTest, Length) {
        Vec2f v2f(3.0f, 4.0f);
        EXPECT_FLOAT_EQ(v2f.length(), 5.0f); // Right triangle with sides 3 and 4 has hypotenuse 5
    }
}
