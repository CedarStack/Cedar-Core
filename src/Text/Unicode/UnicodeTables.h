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

#include <Cedar/Core/Container/Array.h>
#include <Cedar/Core/Container/HashMap.h>

namespace Cedar::Core::Text {
    using Core::Container::Array;
    using Core::Container::HashMap;

    struct UnicodeRange {
        UInt32 start;
        UInt32 end;
    };

    extern const Array<UnicodeRange> CcRanges;
    extern const Array<UnicodeRange> ZsRanges;
    extern const Array<UnicodeRange> PoRanges;
    extern const Array<UnicodeRange> ScRanges;
    extern const Array<UnicodeRange> PsRanges;
    extern const Array<UnicodeRange> PeRanges;
    extern const Array<UnicodeRange> SmRanges;
    extern const Array<UnicodeRange> PdRanges;
    extern const Array<UnicodeRange> NdRanges;
    extern const Array<UnicodeRange> LuRanges;
    extern const Array<UnicodeRange> SkRanges;
    extern const Array<UnicodeRange> PcRanges;
    extern const Array<UnicodeRange> LlRanges;
    extern const Array<UnicodeRange> SoRanges;
    extern const Array<UnicodeRange> LoRanges;
    extern const Array<UnicodeRange> PiRanges;
    extern const Array<UnicodeRange> CfRanges;
    extern const Array<UnicodeRange> NoRanges;
    extern const Array<UnicodeRange> PfRanges;
    extern const Array<UnicodeRange> LtRanges;
    extern const Array<UnicodeRange> LmRanges;
    extern const Array<UnicodeRange> MnRanges;
    extern const Array<UnicodeRange> MeRanges;
    extern const Array<UnicodeRange> McRanges;
    extern const Array<UnicodeRange> NlRanges;
    extern const Array<UnicodeRange> ZlRanges;
    extern const Array<UnicodeRange> ZpRanges;
    extern const Array<UnicodeRange> CsRanges;
    extern const Array<UnicodeRange> CoRanges;

    extern const HashMap<Rune, Rune> ToUpperMap;
    extern const HashMap<Rune, Rune> ToLowerMap;
}
