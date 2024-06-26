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
#include <Cedar/Core/Container/ArrayList.h>

namespace Cedar::Core::Container {
    TEST(ArrayListTest, InsertAtAndRemoveAt) {
        Cedar::Core::Container::ArrayList<int> list;
        list.append(1);
        list.append(3);

        list.insertAt(1, 2);
        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[1], 2);
        EXPECT_EQ(list[2], 3);

        list.insertAt(3, 4);
        EXPECT_EQ(list.size(), 4);
        EXPECT_EQ(list[3], 4);

        EXPECT_THROW({ list.insertAt(10, 5); }, OutOfRangeException);

        list.removeAt(1);
        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[1], 3);
        EXPECT_EQ(list[2], 4);

        list.removeAt(2);
        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list[1], 3);

        EXPECT_THROW({ list.removeAt(5); }, OutOfRangeException);
    }

    TEST(ArrayListTest, AccessOutOfBounds) {
        Cedar::Core::Container::ArrayList<int> list;
        list.append(1);

        EXPECT_THROW({ list[1]; }, OutOfRangeException);
    }
}
