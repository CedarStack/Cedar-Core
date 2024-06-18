//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_STRING_H
#define CEDAR_CORE_STRING_H

#include <Cedar/Core/BasicTypes.h>

namespace Cedar::Core {
    class String {
    public:
        String();
        explicit String(CString string);
        String(const String& other);
        String(String&& other) noexcept;
        ~String();

        Rune at(Index index) const;

        Rune operator[](Index index) const;
        String& operator=(const String& other);
//        String& operator=(String&& other) noexcept;
    private:
        struct Impl;
        Impl* pImpl;
    };
}  // namespace Cedar::Core


#endif //CEDAR_CORE_STRING_H
