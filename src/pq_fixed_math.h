/*
 * pq_fixed_math.h
 *
 * Fixed-point mathfunctions.
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * 
 * Code imported from Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PQ_FIXED_MATH_H
#define PQ_FIXED_MATH_H

#include "pq_globals.h"
#include <stdint.h>

namespace pq {

// computes limit((val >> rshift), 2**bits)
static inline int32_t signed_saturate_rshift(int32_t val, int bits, int rshift) __attribute__((always_inline, unused));
static inline int32_t signed_saturate_rshift(int32_t val, int bits, int rshift)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("ssat %0, %1, %2, asr %3" : "=r" (out) : "I" (bits), "r" (val), "I" (rshift));
	return out;
#else
	int32_t out, max;
	out = val >> rshift;
	max = 1 << (bits - 1);
	if (out >= 0) {
		if (out > max - 1) out = max - 1;
	} else {
		if (out < -max) out = -max;
	}
	return out;
#endif
}

// computes limit(val, 2**bits)
static inline int16_t saturate16(int32_t val) __attribute__((always_inline, unused));
static inline int16_t saturate16(int32_t val)
{
#if defined (__ARM_ARCH_7EM__)
	int16_t out;
	int32_t tmp;
	asm volatile("ssat %0, %1, %2" : "=r" (tmp) : "I" (16), "r" (val) );
	out = (int16_t) (tmp);
	return out;
#else
    if (val > 32767) val = 32767;
    else if (val < -32768) val = -32768;
    return val;
#endif
}

// computes ((a[31:0] * b[15:0]) >> 16)
static inline int32_t signed_multiply_32x16b(int32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_multiply_32x16b(int32_t a, uint32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("smulwb %0, %1, %2" : "=r" (out) : "r" (a), "r" (b));
	return out;
#else
	return ((int64_t)a * (int16_t)(b & 0xFFFF)) >> 16;
#endif
}

// computes ((a[31:0] * b[31:16]) >> 16)
static inline int32_t signed_multiply_32x16t(int32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline int32_t signed_multiply_32x16t(int32_t a, uint32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("smulwt %0, %1, %2" : "=r" (out) : "r" (a), "r" (b));
	return out;
#else
	return ((int64_t)a * (int16_t)(b >> 16)) >> 16;
#endif
}

// computes (((int64_t)a[31:0] * (int64_t)b[31:0]) >> 32)
static inline int32_t multiply_32x32_rshift32(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_32x32_rshift32(int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("smmul %0, %1, %2" : "=r" (out) : "r" (a), "r" (b));
	return out;
#else
	return ((int64_t)a * (int64_t)b) >> 32;
#endif
}

// computes (((int64_t)a[31:0] * (int64_t)b[31:0] + 0x80000000) >> 32)
static inline int32_t multiply_32x32_rshift32_rounded(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_32x32_rshift32_rounded(int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("smmulr %0, %1, %2" : "=r" (out) : "r" (a), "r" (b));
	return out;
#else
    return (((int64_t)a * (int64_t)b) + 0x80000000) >> 32;
#endif
}

// computes sum + (((int64_t)a[31:0] * (int64_t)b[31:0] + 0x80000000) >> 32)
static inline int32_t multiply_accumulate_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_accumulate_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("smmlar %0, %2, %3, %1" : "=r" (out) : "r" (sum), "r" (a), "r" (b));
	return out;
#else
	return sum + ((((int64_t)a * (int64_t)b) + 0x80000000) >> 32);
#endif
}

// computes sum - (((int64_t)a[31:0] * (int64_t)b[31:0] + 0x80000000) >> 32)
static inline int32_t multiply_subtract_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline int32_t multiply_subtract_32x32_rshift32_rounded(int32_t sum, int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("smmlsr %0, %2, %3, %1" : "=r" (out) : "r" (sum), "r" (a), "r" (b));
	return out;
#else
	return sum - ((((int64_t)a * (int64_t)b) + 0x80000000) >> 32);
#endif
}


// computes (a[31:16] | (b[31:16] >> 16))
static inline uint32_t pack_16t_16t(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline uint32_t pack_16t_16t(int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("pkhtb %0, %1, %2, asr #16" : "=r" (out) : "r" (a), "r" (b));
	return out;
#else
	return (a & 0xFFFF0000) | ((uint32_t)b >> 16);
#endif
}

// computes (a[31:16] | b[15:0])
static inline uint32_t pack_16t_16b(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline uint32_t pack_16t_16b(int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("pkhtb %0, %1, %2" : "=r" (out) : "r" (a), "r" (b));
	return out;
#else
	return (a & 0xFFFF0000) | (b & 0x0000FFFF);
#endif
}

// computes ((a[15:0] << 16) | b[15:0])
static inline uint32_t pack_16b_16b(int32_t a, int32_t b) __attribute__((always_inline, unused));
static inline uint32_t pack_16b_16b(int32_t a, int32_t b)
{
#if defined (__ARM_ARCH_7EM__)
	int32_t out;
	asm volatile("pkhbt %0, %1, %2, lsl #16" : "=r" (out) : "r" (b), "r" (a));
	return out;
#else
	return (a << 16) | (b & 0x0000FFFF);
#endif
}

// computes ((a[31:0] << 32) / b[31:0])
static inline uint32_t divide_32div32(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline uint32_t divide_32div32(uint32_t a, uint32_t b) {
	return b ? (uint64_t(a) << 32) / b : 0xFFFFFFFF /* 2^32-1 UINT32_MAX */;
}

} // namespace pq


#endif
