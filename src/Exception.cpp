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

#include <Cedar/Core/Exception.h>
#include <Cedar/Core/Memory.h>
#include <Cedar/Core/String.h>

using namespace Cedar::Core;

class Exception::Impl {
public:
    String message;
    explicit Impl(String msg) : message(Memory::move(msg)) {}
};

Exception::Exception(const String& message)
        : pImpl(new Impl(message)) {}

Exception::Exception(CString message)
        : pImpl(new Impl(message)) {}

Exception::~Exception() {
    delete pImpl;
}

String Exception::getMessage() const noexcept {
    return pImpl->message;
}

const char* Exception::what() const noexcept {
    return pImpl->message.rawString();
}