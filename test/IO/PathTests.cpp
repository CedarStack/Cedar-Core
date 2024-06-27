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
    // Test the default constructor to ensure it initializes an empty path correctly.
    TEST(PathTest, DefaultConstructor) {
        Path p;
        EXPECT_EQ(p.toString(), "");  // The path should be empty initially.
    }

    // Test the parameterized constructor to check path normalization from backslashes to slashes.
    TEST(PathTest, ParameterConstructorAndNormalize) {
        Path p("C:\\Users\\Test\\File.txt");
        EXPECT_EQ(p.toString(), "C:/Users/Test/File.txt");  // Path should be normalized to forward slashes.
    }

    // Test extraction of the file name from a full path.
    TEST(PathTest, GetFileName) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getFileName(), "File.txt");  // Only the file name should be returned.
    }

    // Test extraction of the file extension from a full path.
    TEST(PathTest, GetFileType) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getFileType(), ".txt");  // Only the file extension should be returned.
    }

    // Test getting the parent directory of a file path.
    TEST(PathTest, GetParent) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getParent().toString(), "C:/Users/Test");  // Parent path should be everything except the file.
    }

    // Test extraction of the root component from a path.
    TEST(PathTest, GetRoot) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.getRoot(), "C:");  // The root of the path should be "C:".
    }

    // Test if the path is recognized as absolute.
    TEST(PathTest, IsAbsolute) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_TRUE(p.isAbsolute());  // This should be recognized as an absolute path.

        Path relativePath("Users/Test/File.txt");
        EXPECT_FALSE(relativePath.isAbsolute());  // This should be recognized as a relative path.
    }

    // Test calculating the depth of a path, based on its components.
    TEST(PathTest, CacuDepth) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p.calculateDepth(), 3);  // Depth is the number of elements in the path.
    }

    // Test decomposing a path into a list of all its hierarchical components.
    TEST(PathTest, DecomposeList) {
        Path p("C:/Users/Test/File.txt");
        auto parts = p.decomposeList();
        ASSERT_EQ(parts.size(), 4);
        EXPECT_EQ(parts[0].toString(), "C:");
        EXPECT_EQ(parts[1].toString(), "C:/Users");
        EXPECT_EQ(parts[2].toString(), "C:/Users/Test");
        EXPECT_EQ(parts[3].toString(), "C:/Users/Test/File.txt");
    }

    // Test accessing individual path segments.
    TEST(PathTest, PathSegment) {
        Path p("C:/Users/Test/File.txt");
        EXPECT_EQ(p[0].toString(), "C:");
        EXPECT_EQ(p[1].toString(), "C:/Users");
        EXPECT_EQ(p[2].toString(), "C:/Users/Test");
        EXPECT_EQ(p[3].toString(), "C:/Users/Test/File.txt");
    }
}
