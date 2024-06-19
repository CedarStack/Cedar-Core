//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_MEMORY_H
#define CEDAR_CORE_MEMORY_H

#include <Cedar/Core/BasicTypes.h>

namespace Cedar::Core::Memory {
    void copy(Pointer target, Pointer source, Size size);
    void copyCString(CString target, CString source);
    Size calcCStringLength(CString string);
    Int32 compare(Pointer p1, Pointer p2, Size size);
    Pointer allocate(Size size);
    void release(Pointer pointer);

    template<typename T>
    struct RemoveReference {
        typedef T type;
    };

    template<typename T>
    struct RemoveReference<T&> {
        typedef T type;
    };

    template<typename T>
    struct RemoveReference<T&&> {
        typedef T type;
    };


    template<typename T>
    typename Memory::RemoveReference<T>::type&& move(T&& arg) {
        return static_cast<typename Memory::RemoveReference<T>::type&&>(arg);
    }
}

#endif //CEDAR_CORE_MEMORY_H
