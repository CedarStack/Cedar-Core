//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_BASIC_TYPES_H
#define CEDAR_CORE_BASIC_TYPES_H

namespace Cedar::Core {
    using Int8 = signed char;
    using Int16 = short;
    using Int32 = int;
    using Int64 = long long;

    using UInt8 = unsigned char;
    using UInt16 = unsigned short;
    using UInt32 = unsigned int;
    using UInt64 = unsigned long long;

    using Float32 = float;
    using Float64 = double;
    using FloatExt = long double;

    using Boolean = bool;

    using Byte = unsigned char;
    using Rune = unsigned int;

    using CString = const char*;
    using Pointer = void*;

//    using Char = char;

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
    using SizeType = unsigned long long;
#else
    using SizeType = unsigned int;
#endif

    using Size = SizeType;
    using Index = SizeType;
}

#endif //CEDAR_CORE_BASIC_TYPES_H
