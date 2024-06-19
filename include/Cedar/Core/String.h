//
// Created by itshu on 18/6/2024.
//

#ifndef CEDAR_CORE_STRING_H
#define CEDAR_CORE_STRING_H

#include <Cedar/Core/BasicTypes.h>

namespace Cedar::Core {
    namespace Container {
        template<typename T>
        class List;
    }

    class String {
    public:
        String();

        //NOLINTNEXTLINE
        String(CString string);
        String(CString str, Size len);
        String(const String& other);
        String(String&& other) noexcept;
        ~String();

        Rune at(Index index) const;
        CString rawString() const noexcept;

        Container::List<String> split(const String& delimiter) const;

        Rune operator[](Index index) const;
        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;
        String operator+(const String& other) const;
        Boolean operator==(const String& other) const;
        Boolean operator!=(const String& other) const;
    private:
        struct Impl;
        Impl* pImpl;
    };
}  // namespace Cedar::Core


#endif //CEDAR_CORE_STRING_H
