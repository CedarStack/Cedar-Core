//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_EXCEPTION_H
#define CEDAR_CORE_EXCEPTION_H

#include <Cedar/Core/String.h>

namespace Cedar::Core {
    class Exception {
    public:
        explicit Exception(const String& message);
        ~Exception();

        String what() const noexcept;
    private:
        class Impl;
        Impl* pImpl;
    };
}

#endif //CEDAR_CORE_EXCEPTION_H
