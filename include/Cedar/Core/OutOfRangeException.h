//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_OUT_OF_RANGE_EXCEPTION_H
#define CEDAR_CORE_OUT_OF_RANGE_EXCEPTION_H

#include <Cedar/Core/Exception.h>

namespace Cedar::Core {
    class OutOfRangeException: public Exception {
    public:
        explicit OutOfRangeException(const String& message): Exception(message) {}
    };
}

#endif //CEDAR_CORE_OUT_OF_RANGE_EXCEPTION_H
