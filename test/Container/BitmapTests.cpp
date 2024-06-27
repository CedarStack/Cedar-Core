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
#include <Cedar/Core/Exceptions/OutOfRangeException.h>
#include <Cedar/Core/Container/Bitmap.h>

namespace Cedar::Core::Container {
// Test the constructor functionality
    TEST(BitmapTest, Constructor) {
        Bitmap<int> bitmap(10, 20);
        EXPECT_EQ(bitmap.width(), 10);
        EXPECT_EQ(bitmap.height(), 20);
    }

// Test pixel data access and modification
    TEST(BitmapTest, PixelAccess) {
        Bitmap<int> bitmap(10, 20);
        bitmap(0, 0) = 123;
        EXPECT_EQ(bitmap(0, 0), 123);

        bitmap(19, 9) = 456; // Access the last pixel
        EXPECT_EQ(bitmap(19, 9), 456);
    }

// Test out of range access
    TEST(BitmapTest, OutOfRangeAccess) {
        Bitmap<int> bitmap(10, 20);
        // Testing row out of range
        EXPECT_THROW(bitmap(0, 10), OutOfRangeException);
        // Testing column out of range
        EXPECT_THROW(bitmap(20, 0), OutOfRangeException);
    }

// Test the copy constructor to ensure deep copy
    TEST(BitmapTest, CopyConstructor) {
        Bitmap<int> bitmap(10, 20);
        bitmap(0, 0) = 123;

        Bitmap<int> bitmapCopy = bitmap;
        EXPECT_EQ(bitmapCopy(0, 0), bitmap(0, 0));

        // Change original after copy to test for deep copy
        bitmap(0, 0) = 321;
        EXPECT_NE(bitmapCopy(0, 0), bitmap(0, 0));
    }

// Optionally, test memory management
    TEST(BitmapTest, MemoryManagement) {
        Bitmap<int>* bitmap = new Bitmap<int>(10, 20);
        bitmap->operator()(5, 5) = 2024;  // Set a value
        EXPECT_EQ(bitmap->operator()(5, 5), 2024);
        delete bitmap;  // Ensure no memory leaks (implicitly tested)
    }
}
