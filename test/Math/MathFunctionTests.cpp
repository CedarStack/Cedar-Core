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
#include <cmath>
#include <chrono>

#include <Cedar/Core/Math/MathFunctions.h>

namespace Cedar::Core::Math {

    // Tests to ensure the absolute value function has precision comparable to std::abs
    TEST(MathFunctionsTest, AbsPrecision) {
        // Tests for integer and long long
        EXPECT_EQ(Math::abs(-42), std::abs(-42));
        EXPECT_EQ(Math::abs(-42LL), std::abs(-42LL));

        // Tests for floating-point numbers
        EXPECT_FLOAT_EQ(Math::abs(-42.0f), std::abs(-42.0f));
        EXPECT_DOUBLE_EQ(Math::abs(-42.0), std::abs(-42.0));
    }

    // Measures the time taken to execute a function repeatedly
    double MeasureTime(std::function<double(double)> func, double value) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100000; ++i) {
            func(value);  // Call function repeatedly to get a measurable time duration
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        return duration.count();
    }

    // Tests to verify that the custom square root function matches the standard library's results
    TEST(MathFunctionsTest, SqrtTest) {
        // Test sqrt for perfect squares
        EXPECT_DOUBLE_EQ(Math::sqrt(4.0), std::sqrt(4.0));
        EXPECT_DOUBLE_EQ(Math::sqrt(9.0), std::sqrt(9.0));
        EXPECT_DOUBLE_EQ(Math::sqrt(16.0), std::sqrt(16.0));

        // Test sqrt for Pi
        EXPECT_DOUBLE_EQ(Math::sqrt(M_PI), std::sqrt(M_PI));

        // Test sqrt for edge cases
        EXPECT_DOUBLE_EQ(Math::sqrt(0.0), std::sqrt(0.0));
        EXPECT_DOUBLE_EQ(Math::sqrt(1.0), std::sqrt(1.0));

        // Test sqrt for negative number (should return NaN)
        EXPECT_TRUE(std::isnan(Math::sqrt(-1.0)));
    }

    // Optional: Adding a performance test for sqrt function
    TEST(MathFunctionsTest, SqrtPerformance) {
        auto customTime = MeasureTime(Math::sqrt, 16.0);
        auto stdTime = MeasureTime([](double x) -> double { return std::sqrt(x); } , 16.0);
        std::cout << "Custom sqrt time: " << customTime << ", Standard sqrt time: " << stdTime << std::endl;
    }
}
