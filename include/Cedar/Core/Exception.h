//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_EXCEPTION_H
#define CEDAR_CORE_EXCEPTION_H

#include <exception>

namespace Cedar::Core {
    class String;

    class Exception : public std::exception {
    public:
        explicit Exception(const String &message);

        ~Exception();

        String getMessage() const noexcept;

        const char *what() const noexcept override;

    private:
        class Impl;

        Impl *pImpl;
    };
}

#endif //CEDAR_CORE_EXCEPTION_H
