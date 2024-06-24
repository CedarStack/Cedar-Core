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
#include <Cedar/Core/IO/Path.h>

namespace Cedar::Core::IO {
    TEST(PathTest, DefaultConstructor) {
        Path p;
        EXPECT_EQ(p.toString(), "");
    }

    TEST(PathTest, ParameterConstructorAndNormalize) {
        Path p("C:\\Users\\Test\\File.txt");
        EXPECT_EQ(p.toString(), "C:/Users/Test/File.txt");
    }

    TEST(PathTest, GetFileName) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getFileName(), "File.txt");
    }

    TEST(PathTest, GetFileType) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getFileType(), ".txt");
    }

    TEST(PathTest, GetParent) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getParent().toString(), "C:/Users/Test");
    }

    TEST(PathTest, GetRoot) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getRoot(), "C:");
    }

    TEST(PathTest, IsAbsolute) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_TRUE(p.isAbsolute());

        Path relativePath("Users/Test/File.txt");
        EXPECT_FALSE(relativePath.isAbsolute());
    }

    TEST(PathTest, CacuDepth) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.calculateDepth(), 3);
    }

    TEST(PathTest, DecomposeList) {
        Path p("C:/Users/Test/File.txt");
        auto parts = p.decomposeList();
        ASSERT_EQ(parts.size(), 4);
        EXPECT_EQ(parts[0].toString(), "C:");
        EXPECT_EQ(parts[1].toString(), "C:/Users");
        EXPECT_EQ(parts[2].toString(), "C:/Users/Test");
        EXPECT_EQ(parts[3].toString(), "C:/Users/Test/File.txt");
    }

    TEST(PathTest, PathSegment) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p[0].toString(), "C:");
        EXPECT_EQ(p[1].toString(), "C:/Users");
        EXPECT_EQ(p[2].toString(), "C:/Users/Test");
        EXPECT_EQ(p[3].toString(), "C:/Users/Test/File.txt");
    }
}
