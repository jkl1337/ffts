/*

This file is part of FFTS -- The Fastest Fourier Transform in the South

Copyright (c) 2018, Jukka Ojanen <jukka.ojanen@kolumbus.fi>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the organization nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANTHONY M. BLAKE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef FFTS_MACROS_SSE_H
#define FFTS_MACROS_SSE_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <immintrin.h>

typedef __m128 V4SF;

#define V4SF_ADD  _mm_add_ps
#define V4SF_SUB  _mm_sub_ps
#define V4SF_MUL  _mm_mul_ps
#define V4SF_LIT4 _mm_set_ps
#define V4SF_XOR  _mm_xor_ps
#define V4SF_ST   _mm_store_ps
#define V4SF_LD   _mm_load_ps

#define V4SF_SWAP_PAIRS(x) \
    (_mm_shuffle_ps(x, x, _MM_SHUFFLE(2,3,0,1)))

/* note: order is swapped */
#define V4SF_UNPACK_HI(x,y) \
    (_mm_movehl_ps(y, x))

#define V4SF_UNPACK_LO(x,y) \
    (_mm_movelh_ps(x, y))

#define V4SF_BLEND(x, y) \
    (_mm_shuffle_ps(x, y, _MM_SHUFFLE(3,2,1,0)))

#define V4SF_DUPLICATE_RE(r) \
    (_mm_shuffle_ps(r, r, _MM_SHUFFLE(2,2,0,0)))

#define V4SF_DUPLICATE_IM(r) \
    (_mm_shuffle_ps(r, r, _MM_SHUFFLE(3,3,1,1)))

static FFTS_ALWAYS_INLINE V4SF
V4SF_IMULI(int inv, V4SF a)
{
    if (inv) {
        return V4SF_SWAP_PAIRS(V4SF_XOR(a, V4SF_LIT4(0.0f, -0.0f, 0.0f, -0.0f)));
    } else {
        return V4SF_SWAP_PAIRS(V4SF_XOR(a, V4SF_LIT4(-0.0f, 0.0f, -0.0f, 0.0f)));
    }
}

static FFTS_ALWAYS_INLINE V4SF
V4SF_IMUL(V4SF d, V4SF re, V4SF im)
{
    re = V4SF_MUL(re, d);
    im = V4SF_MUL(im, V4SF_SWAP_PAIRS(d));
    return V4SF_SUB(re, im);
}

static FFTS_ALWAYS_INLINE V4SF
V4SF_IMULJ(V4SF d, V4SF re, V4SF im)
{
    re = V4SF_MUL(re, d);
    im = V4SF_MUL(im, V4SF_SWAP_PAIRS(d));
    return V4SF_ADD(re, im);
}

#ifdef FFTS_DOUBLE
typedef __m256d V4DF;

#define V4DF_ADD  _mm256_add_pd
#define V4DF_SUB  _mm256_sub_pd
#define V4DF_MUL  _mm256_mul_pd
#define V4DF_LIT4 _mm256_set_pd
#define V4DF_XOR  _mm256_xor_pd
#define V4DF_LD   _mm256_loadu_pd
#define V4DF_ST   _mm256_storeu_pd

#define V4DF_SWAP_PAIRS(x) \
    (_mm256_permute_pd(x, (1 << 2) | (1 << 0)))

#define V4DF_BLEND(x, y) \
    (_mm256_blend_pd(x, y, (1 << 3) | (1 << 2)))

#define V4DF_UNPACK_HI(x, y) \
    (_mm256_permute2f128_pd(x, y, (1 << 5) | (1 << 4) | (1 << 0)))

#define V4DF_UNPACK_LO(x, y) \
    (_mm256_permute2f128_pd(x, y, (1 << 5)))

#define V4DF_DUPLICATE_RE(x) \
    (_mm256_movedup_pd(x))

#define V4DF_DUPLICATE_IM(x) \
    (_mm256_permute_pd(x, (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))

static FFTS_ALWAYS_INLINE V4DF
V4DF_IMUL(V4DF d, V4DF re, V4DF im)
{
    re = V4DF_MUL(re, d);
    im = V4DF_MUL(im, V4DF_SWAP_PAIRS(d));
    return V4DF_SUB(re, im);
}

static FFTS_ALWAYS_INLINE V4DF
V4DF_IMULJ(V4DF d, V4DF re, V4DF im)
{
    re = V4DF_MUL(re, d);
    im = V4DF_MUL(im, V4DF_SWAP_PAIRS(d));
    return V4DF_ADD(re, im);
}

static FFTS_ALWAYS_INLINE V4DF
V4DF_IMULI(int inv, V4DF a)
{
    if (inv) {
        return V4DF_SWAP_PAIRS(V4DF_XOR(a, V4DF_LIT4(0.0, -0.0, 0.0, -0.0)));
    } else {
        return V4DF_SWAP_PAIRS(V4DF_XOR(a, V4DF_LIT4(-0.0, 0.0, -0.0, 0.0)));
    }
}
#endif

#endif /* FFTS_MACROS_AVX_H */
