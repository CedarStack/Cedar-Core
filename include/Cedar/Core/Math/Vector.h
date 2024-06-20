//
// Created by itshu on 18/6/2024.
//

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
