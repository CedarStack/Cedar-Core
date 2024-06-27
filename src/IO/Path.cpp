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

#include <Cedar/Core/IO/Path.h>

using namespace Cedar::Core;
using namespace Cedar::Core::Container;
using namespace Cedar::Core::IO;

const String pathSeparator = "/";

static String normalize(const String& path) {
    String normalizedPath = path.replace("\\", "/");

    Size pos = 0;
    while ((pos = normalizedPath.find("//", pos + 2)) != String::NPos) {
        normalizedPath = normalizedPath.substring(0, pos) + normalizedPath.substring(pos + 1);
    }

    return normalizedPath;
}

struct Path::Impl {
    String path;

    Impl(String path): path(normalize(path)) {}
};

Path::Path(): pImpl(new Impl("")) {}

Path::Path(const String& path) : pImpl(new Impl(path)) {}

Path::Path(const Path& other) : pImpl(new Impl(*other.pImpl)) {}

Path::Path(Path&& other) noexcept : pImpl(other.pImpl) {
    other.pImpl = nullptr;
}

Path::~Path() {
    delete pImpl;
}

String Path::getFileName() const {
    Size pos = pImpl->path.find("/", -1);
    return pos != String::NPos ? pImpl->path.substring(pos + 1) : pImpl->path;
}

String Path::getFileType() const {
    Size pos = pImpl->path.find('.', -1);
    return pos != String::NPos ? pImpl->path.substring(pos) : String();
}

Path Path::getParent() const {
    Size pos = pImpl->path.find(pathSeparator, -1);
    return pos != String::NPos ? Path(pImpl->path.substring(0, pos)) : Path();
}

String Path::getRoot() const {
    if (isAbsolute()) {
        Size pos = pImpl->path.find(pathSeparator);
        if (pos != String::NPos) {
            return pImpl->path.substring(0, pos);
        }
    }
    return String();
}

Boolean Path::isAbsolute() const {
    return  pImpl->path.length() > 0 && (pImpl->path[0] == pathSeparator[0] || ( pImpl->path.length() > 1 &&  pImpl->path[1] == ':'));
}

Size Path::calculateDepth() const {
    Size depth = 0;
    Size start = 0;
    while ((start = pImpl->path.find(pathSeparator, start + 1)) != String::NPos) {
        depth++;
    }
    return depth;
}

List<Path> Path::decomposeList() const {
    List<Path> parts;
    Size start = 0, end;

    while ((end = pImpl->path.find(pathSeparator, start)) != String::NPos) {
        if (end != start) {
            String part = pImpl->path.substring(0, end);
            parts.append(Path(part));
        }
        start = end + 1;
    }

    if (start < pImpl->path.length()) {
        String part = pImpl->path.substring(0, pImpl->path.length());
        parts.append(Path(part));
    }

    return parts;
}

Path& Path::operator=(const Path& other) {
    if (this != &other) {
        delete pImpl;
        pImpl = new Impl(*other.pImpl);
    }
    return *this;
}

Path& Path::operator=(Path&& other) noexcept {
    if (this != &other) {
        delete pImpl;
        pImpl = other.pImpl;
        other.pImpl = nullptr;
    }
    return *this;
}

Path Path::operator[](Size index) const {
    return this->decomposeList()[index];
}

String Path::toString() const {
    return pImpl->path;
}
