//
// Created by itshu on 18/6/2024.
//

#include <Cedar/Core/Memory.h>

// NOLINTNEXTLINE
#include <string.h>

using namespace Cedar::Core;

void Memory::copy(Cedar::Core::Pointer target, Cedar::Core::Pointer source, Cedar::Core::Size size) {
    memcpy(target, source, size);
}

Int32 Memory::compare(Pointer p1, Pointer p2, Size size) {
    return memcmp(p1, p2, size);
}

void Memory::copyCString(Cedar::Core::CString target, Cedar::Core::CString source) {
    strcpy((char*) target, source);
}

Size Memory::calcCStringLength(Cedar::Core::CString string) {
    return strlen(string);
}

Pointer Memory::allocate(Size size) {
    return new char[size]();
}

void Memory::release(Pointer memory) {
    delete[] static_cast<char*>(memory);
}
