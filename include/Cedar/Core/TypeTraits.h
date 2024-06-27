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

#pragma once

#include <Cedar/Core/BasicTypes.h>

namespace Cedar::Core::TypeTraits {

    template<typename T, T v>
    struct IntegralConstant {
        static constexpr T value = v;
        typedef T ValueType;
        typedef IntegralConstant<T, v> Type;
        constexpr operator T() const noexcept { return v; }
    };

    typedef IntegralConstant<Boolean, true> TrueType;
    typedef IntegralConstant<Boolean, false> FalseType;

    template<Boolean B, typename T, typename F>
    struct Conditional { typedef T Type; };

    template<typename T, typename F>
    struct Conditional<false, T, F> { typedef F Type; };

    template<typename T>
    struct IsReference : FalseType {};

    template<typename T>
    struct IsReference<T&> : TrueType {};

    template<typename T>
    struct IsReference<T&&> : TrueType {};

    template<typename T>
    struct RemoveReference { typedef T Type; };

    template<typename T>
    struct RemoveReference<T&> { typedef T Type; };

    template<typename T>
    struct RemoveReference<T&&> { typedef T Type; };

    template<typename T>
    struct IsClassHelper {
        template<typename C> static CChar test(Int32 C::*);
        template<typename C> static Int64 test(...);
    };

    template<typename T>
    struct IsClass : IntegralConstant<Boolean, sizeof(IsClassHelper<T>::template test<T>(0)) == sizeof(Byte)> {};

    template<typename T>
    struct IsUnionHelper {
        template<typename C> static CChar test(C*);
        template<typename C> static Int64 test(Int32 C::*);
    };

    template<typename T>
    struct IsUnion : IntegralConstant<Boolean, sizeof(IsUnionHelper<T>::template test<T>(nullptr)) == sizeof(Byte) && !IsClass<T>::value> {};

    template<typename T>
    struct RemoveConst {
        typedef T type;
    };

    template<typename T>
    struct RemoveConst<const T> {
        typedef T type;
    };

    template<typename T>
    struct RemoveVolatile {
        typedef T type;
    };

    template<typename T>
    struct RemoveVolatile<volatile T> {
        typedef T type;
    };

    template<typename T>
    struct RemoveCV {
        typedef typename RemoveConst<typename RemoveVolatile<T>::type>::type type;
    };

    template<typename T>
    struct RemoveExtent {
        typedef T type;
    };

    template<typename T>
    struct RemoveExtent<T[]> {
        typedef T type;
    };

    template<typename T, Size N>
    struct RemoveExtent<T[N]> {
        typedef T type;
    };

    template<typename T>
    struct IsArray : FalseType {};

    template<typename T>
    struct IsArray<T[]> : TrueType {};

    template<typename T, Size N>
    struct IsArray<T[N]> : TrueType {};

    template<typename T>
    struct IsFunction : IntegralConstant<Boolean, !IsReference<T>::value && !IsClass<T>::value && !IsUnion<T>::value> {};

    template<typename T>
    struct AddPointer {
        typedef typename Conditional<IsFunction<T>::value || IsArray<T>::value, T, T*>::Type Type;
    };

    template<typename T>
    struct Decay {
    private:
        typedef typename RemoveReference<T>::Type NoRef;
    public:
        typedef typename Conditional<
                IsArray<NoRef>::value,
                typename RemoveExtent<NoRef>::Type*,
                typename Conditional<
                        IsFunction<NoRef>::value,
                        NoRef*,
                        typename RemoveCV<NoRef>::Type
                >::Type
        >::Type Type;
    };

    template<typename T>
    using ToDecay = typename Decay<T>::Type;

    template<typename T>
    typename RemoveReference<T>::Type&& move(T&& arg) {
        return static_cast<typename RemoveReference<T>::Type&&>(arg);
    }

    template<typename T>
    T&& forward(typename RemoveReference<T>::Type& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    T&& forward(typename RemoveReference<T>::Type&& arg) noexcept {
        return static_cast<T&&>(arg);
    }
}
