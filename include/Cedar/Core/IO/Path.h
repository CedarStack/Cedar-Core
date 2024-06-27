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

#pragma once

#include <Cedar/Core/String.h>
#include <Cedar/Core/Container/List.h>

namespace Cedar::Core::IO {
    class Path {
    public:
        Path();
        Path(const String& path);
        Path(const Path& other);
        Path(Path&& other) noexcept;
        ~Path();

        // Basic operations
        [[nodiscard]] String getFileName() const;
        [[nodiscard]] String getFileType() const;
        [[nodiscard]] Path getParent() const;
        [[nodiscard]] String getRoot() const;
        [[nodiscard]] Boolean isAbsolute() const;
        [[nodiscard]] Size calculateDepth() const;
        [[nodiscard]] Container::List<Path> decomposeList() const;

        // Operator overloads
        Path& operator=(const Path& other);
        Path& operator=(Path&& other) noexcept;
        [[nodiscard]] Path operator[](Size index) const;

        // Conversion to string for easy output
        [[nodiscard]] String toString() const;
    private:
        struct Impl;
        Impl* pImpl;
    };
}
