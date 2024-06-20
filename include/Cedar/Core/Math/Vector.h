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
#include <cmath>

namespace Cedar::Core::Math {

    template<typename T, Size S>
    class Vector {
    private:
        T Arr[S]{};

        UInt32 Counter = 0;

    public:
        Vector() = default;

        explicit Vector(T value) {
            for (Index i = 0; i < S; ++i) {
                Arr[i] = value;
            }
        }

        template<typename... T1>
        explicit Vector(T1... values) {
            auto InitFun = [this](T &v) {
                Arr[Counter] = v;
                Counter++;
            };
            ((InitFun(values)), ...);
            Counter = 0;
        }

        T operator[](Index index) {
            if (index >= S) {
                throw OutOfRangeException("Index out of range");
            }
            return Arr[index];
        }

        Vector operator+() {
            for (Index i = 0; i < S; ++i) {
                Arr[i] = abs(Arr[i]);
            }
            return *this;
        }

        Vector operator-() {
            for (Index i = 0; i < S; ++i) {
                Arr[i] = -abs(Arr[i]);
            }
            return *this;
        }

        Vector operator+(const Vector &v) {
            Vector copy = *this;
            for (Index i = 0; i < S; ++i) {
                copy.Arr[i] += v.Arr[i];
            }
            return copy;
        }

        Vector operator-(const Vector &v) {
            for (Index i = 0; i < S; ++i) {
                this->Arr[i] -= v.Arr[i];
            }
            return *this;
        }

        Vector operator*(const Vector &v) {
            for (Index i = 0; i < S; ++i) {
                this->Arr[i] *= v.Arr[i];
            }
            return *this;
        }

        Vector operator/(const Vector &v) {
            for (Index i = 0; i < S; ++i) {
                this->Arr[i] /= v.Arr[i];
            }
            return *this;
        }

        Vector operator+=(const Vector &v) {
            return this + v;
        }

        Vector operator-=(const Vector &v) {
            return this - v;
        }

        Vector operator*=(const Vector &v) {
            return this * v;
        }

        Vector operator/=(const Vector &v) {
            return this / v;
        }

        Boolean operator==(const Vector &v) {
            Boolean isEqual = true;
            for (Index i = 0; i < S; ++i) {
                if (this->Arr[i] != v.Arr[i]) { isEqual = false; } else { isEqual = true; }
            }
            return isEqual;
        }

        Boolean operator!=(const Vector &v) {
            Boolean isEqual = true;
            for (Index i = 0; i < S; ++i) {
                if (this->Arr[i] != v.Arr[i]) { isEqual = false; } else { isEqual = true; }
            }
            return !isEqual;
        }

    };

    using Vec2f = Vector<Float32, 2>;
    using Vec3d = Vector<Float64, 3>;
}
