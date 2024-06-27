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

#include <Cedar/Core/Memory.h>

#include <memory>
#include <utility>

using namespace std;

namespace Cedar::Core {

    template<typename ReturnType, typename... ArgTypes>
    class FunctionBase {
    public:
        virtual ~FunctionBase() {}
        virtual ReturnType invoke(ArgTypes... args) const = 0;
    };

    template<typename Func, typename ReturnType, typename... ArgTypes>
    class FunctionHolder : public FunctionBase<ReturnType, ArgTypes...> {
    private:
        Func func;
    public:
        FunctionHolder(Func f) : func(std::move(f)) {}

        ReturnType invoke(ArgTypes... args) const override {
            return func(args...);
        }
    };

    template<typename ReturnType, typename... ArgTypes>
    class Function {
    private:
        Memory::SharedPointer<FunctionBase<ReturnType, ArgTypes...>> func;
    public:
        template<typename Func>
        Function(Func&& f) {
            func = Memory::makeShared<FunctionHolder<Func, ReturnType, ArgTypes...>>(TypeTraits::forward<Func>(f));
        }

        Function(const Function& other) = default;

        Function& operator=(const Function& other) = default;

        Function(Function&& other) noexcept = default;

        Function& operator=(Function&& other) noexcept = default;

        ReturnType operator()(ArgTypes... args) const {
            return func->invoke(args...);
        }
    };


    class Defer {
    private:
        Function<void> func;

    public:
        Defer(const Function<void>& f) : func(f) {}

        ~Defer() {
            func();
        }

        Defer(const Defer&) = delete;
        Defer& operator=(const Defer&) = delete;
    };
}
