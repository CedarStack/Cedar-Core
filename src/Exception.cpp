//
// Created by itshu on 18/6/2024.
//

#include <Cedar/Core/Exception.h>
#include <Cedar/Core/Memory.h>

using namespace Cedar::Core;

class Exception::Impl {
public:
    String message;
    explicit Impl(String msg) : message(Memory::move(msg)) {}
};

Exception::Exception(const String& message)
        : pImpl(new Impl(message)) {}

Exception::~Exception() {
    delete pImpl;
}

String Exception::what() const noexcept {
    return pImpl->message;
}
