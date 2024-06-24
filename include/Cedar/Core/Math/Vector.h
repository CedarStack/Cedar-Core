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
#include <Cedar/Core/Math/MathFunctions.h>

namespace Cedar::Core::Math {
    template<typename T, Size S>
    class Vector {
    public:
        Vector() = default;

        explicit Vector(T value) {
            for (Index i = 0; i < S; ++i) {
                m_array[i] = value;
            }
        }

        template<typename... T1>
        explicit Vector(T1... values) {
            static_assert(sizeof...(values) == S, "Number of arguments must match the vector size");
            T initList[] = { values... };
            for (Index i = 0; i < S; ++i) {
                m_array[i] = initList[i];
            }
        }

        T operator[](Index index) const {
            if (index >= S) {
                throw OutOfRangeException("Index out of range");
            }
            return m_array[index];
        }

        T& operator[](Index index) {
            if (index >= S) {
                throw OutOfRangeException("Index out of range");
            }
            return m_array[index];
        }

        Vector operator+() const {
            return *this;
        }

        Vector operator-() const {
            Vector result;
            for (Index i = 0; i < S; ++i) {
                result.m_array[i] = -m_array[i];
            }
            return result;
        }

        Vector operator+(const Vector &v) const {
            Vector result = *this;
            for (Index i = 0; i < S; ++i) {
                result.m_array[i] += v.m_array[i];
            }
            return result;
        }

        Vector operator-(const Vector &v) const {
            Vector result = *this;
            for (Index i = 0; i < S; ++i) {
                result.m_array[i] -= v.m_array[i];
            }
            return result;
        }

        Vector operator*(const Vector &v) const {
            Vector result = *this;
            for (Index i = 0; i < S; ++i) {
                result.m_array[i] *= v.m_array[i];
            }
            return result;
        }

        Vector operator/(const Vector &v) const {
            Vector result = *this;
            for (Index i = 0; i < S; ++i) {
                result.m_array[i] /= v.m_array[i];
            }
            return result;
        }

        Vector& operator+=(const Vector &v) {
            *this = *this + v;
            return *this;
        }

        Vector& operator-=(const Vector &v) {
            *this = *this - v;
            return *this;
        }

        Vector& operator*=(const Vector &v) {
            *this = *this * v;
            return *this;
        }

        Vector& operator/=(const Vector &v) {
            *this = *this / v;
            return *this;
        }

        bool operator==(const Vector &v) const {
            for (Index i = 0; i < S; ++i) {
                if (m_array[i] != v.m_array[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Vector &v) const {
            // NOLINTNEXTLINE
            return !(*this == v);
        }

        T length() const {
            T sum = 0;
            for (Index i = 0; i < S; ++i) {
                sum += m_array[i] * m_array[i];
            }
            return Math::sqrt(sum);
        }

        static T distance(const Vector& v1, const Vector& v2) {
            return (v1 - v2).length();
        }
    private:
        T m_array[S]{};
    };

    using Vec2f = Vector<Float32, 2>;
    using Vec3d = Vector<Float64, 3>;
}
