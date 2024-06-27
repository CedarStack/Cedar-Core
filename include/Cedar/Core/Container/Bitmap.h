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
#include <Cedar/Core/Memory.h>
#include <Cedar/Core/Threading/Mutex.h>
#include <Cedar/Core/Threading/LockGuard.h>
#include <Cedar/Core/Exceptions/OutOfRangeException.h>

namespace Cedar::Core::Container {
    template<typename PixelType>
    class Bitmap {
    public:
        Bitmap(Size width, Size height): m_width(width), m_height(height), m_pixels(new PixelType[width * height]) {}

        ~Bitmap() = default;

        Bitmap(const Bitmap& other) {
            Threading::LockGuard<Threading::Mutex> lock(other.m_mtx);
            m_width = other.m_width;
            m_height = other.m_height;
            m_pixels.reset(new PixelType[m_width * m_height]);
            Memory::copy(m_pixels.get(), other.m_pixels.get(), m_width * m_height * sizeof(PixelType));
        }

        Bitmap& operator=(const Bitmap&) = delete;

        [[nodiscard]] Size width() const {
            return m_width;
        }

        [[nodiscard]] Size height() const {
            return m_height;
        }

        PixelType* pixelData() {
            return m_pixels.get();
        }

        const PixelType* pixelData() const {
            return m_pixels.get();
        }

        PixelType& operator()(Size row, Size col) {
            if (row >= m_height || col >= m_width)
                throw OutOfRangeException("Index out of range");
            return m_pixels[row * m_width + col];
        }

        const PixelType& operator()(Size row, Size col) const {
            if (row >= m_height || col >= m_width)
                throw OutOfRangeException("Index out of range");
            return m_pixels[row * m_width + col];
        }
    private:
        mutable Threading::Mutex m_mtx;
        Memory::UniquePointer<PixelType[]> m_pixels;
        Size m_width;
        Size m_height;
    };
}
