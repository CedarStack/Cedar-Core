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
#include <Cedar/Core/Container/HashMap.h>

namespace Cedar::Core::Container {
    TEST(HashMapTest, InsertAndFind) {
        HashMap<int, std::string, 10> map;

        map.insert(1, "one");
        map.insert(2, "two");
        map.insert(3, "three");

        EXPECT_EQ(*map.find(1), "one");
        EXPECT_EQ(*map.find(2), "two");
        EXPECT_EQ(*map.find(3), "three");
        EXPECT_EQ(map.find(4), nullptr);
    }

    TEST(HashMapTest, Delete) {
        HashMap<int, std::string, 10> map;

        map.insert(1, "one");
        map.insert(2, "two");

        EXPECT_TRUE(map.remove(1));
        EXPECT_EQ(map.find(1), nullptr);
        EXPECT_NE(map.find(2), nullptr);
        EXPECT_FALSE(map.remove(1));
    }

    TEST(HashMapTest, Clear) {
        HashMap<int, std::string, 10> map;

        map.insert(1, "one");
        map.insert(2, "two");
        map.clear();

        EXPECT_EQ(map.find(1), nullptr);
        EXPECT_EQ(map.find(2), nullptr);
    }
}
