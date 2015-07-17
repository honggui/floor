/*
 *  Flo's Open libRary (floor)
 *  Copyright (C) 2004 - 2015 Florian Ziesche
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License only.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __FLOOR_COMPUTE_DEVICE_HOST_ATOMIC_HPP__
#define __FLOOR_COMPUTE_DEVICE_HOST_ATOMIC_HPP__

#if defined(FLOOR_COMPUTE_HOST)

// already include this here, since the host uses a vanilla stl/libc++
#include <atomic>

// cmpxchg (up here, because it's needed by the fallback implementations)
floor_inline_always int32_t atomic_cmpxchg(volatile int32_t* p, int32_t cmp, int32_t val) {
	return __c11_atomic_compare_exchange_weak((_Atomic(int32_t)*)p, &cmp, val,
											  memory_order_relaxed, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_cmpxchg(volatile uint32_t* p, uint32_t cmp, uint32_t val) {
	return __c11_atomic_compare_exchange_weak((_Atomic(uint32_t)*)p, &cmp, val,
											  memory_order_relaxed, memory_order_relaxed);
}
floor_inline_always float atomic_cmpxchg(volatile float* p, float cmp, float val) {
	const auto ret = atomic_cmpxchg((volatile uint32_t*)p, *(uint32_t*)&cmp, *(uint32_t*)&val);
	return *(float*)&ret;
}
floor_inline_always int64_t atomic_cmpxchg(volatile int64_t* p, int64_t cmp, int64_t val) {
	return __c11_atomic_compare_exchange_weak((_Atomic(int64_t)*)p, &cmp, val,
											  memory_order_relaxed, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_cmpxchg(volatile uint64_t* p, uint64_t cmp, uint64_t val) {
	return __c11_atomic_compare_exchange_weak((_Atomic(uint64_t)*)p, &cmp, val,
											  memory_order_relaxed, memory_order_relaxed);
}
floor_inline_always double atomic_cmpxchg(volatile float* p, double cmp, double val) {
	const auto ret = atomic_cmpxchg((volatile uint64_t*)p, *(uint64_t*)&cmp, *(uint64_t*)&val);
	return *(double*)&ret;
}

// add
floor_inline_always int32_t atomic_add(volatile int32_t* p, int32_t val) {
	return __c11_atomic_fetch_add((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_add(volatile uint32_t* p, uint32_t val) {
	return __c11_atomic_fetch_add((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always float atomic_add(volatile float* p, float val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(+, , p, val)
}
floor_inline_always int64_t atomic_add(volatile int64_t* p, int64_t val) {
	return __c11_atomic_fetch_add((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_add(volatile uint64_t* p, uint64_t val) {
	return __c11_atomic_fetch_add((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always float atomic_add(volatile double* p, double val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(+, , p, val)
}

// sub
floor_inline_always int32_t atomic_sub(volatile int32_t* p, int32_t val) {
	return __c11_atomic_fetch_sub((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_sub(volatile uint32_t* p, uint32_t val) {
	return __c11_atomic_fetch_sub((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always float atomic_sub(volatile float* p, float val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(-, , p, val)
}
floor_inline_always int64_t atomic_sub(volatile int64_t* p, int64_t val) {
	return __c11_atomic_fetch_sub((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_sub(volatile uint64_t* p, uint64_t val) {
	return __c11_atomic_fetch_sub((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always float atomic_sub(volatile double* p, double val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(-, , p, val)
}

// inc
floor_inline_always int32_t atomic_inc(volatile int32_t* p) {
	return __c11_atomic_fetch_add((_Atomic(int32_t)*)p, 1, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_inc(volatile uint32_t* p) {
	return __c11_atomic_fetch_add((_Atomic(uint32_t)*)p, 1u, memory_order_relaxed);
}
floor_inline_always float atomic_inc(volatile float* p) {
	return atomic_add(p, 1.0f);
}
floor_inline_always int64_t atomic_inc(volatile int64_t* p) {
	return __c11_atomic_fetch_add((_Atomic(int64_t)*)p, 1ll, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_inc(volatile uint64_t* p) {
	return __c11_atomic_fetch_add((_Atomic(uint64_t)*)p, 1ull, memory_order_relaxed);
}
floor_inline_always double atomic_inc(volatile double* p) {
	return atomic_add(p, 1.0);
}

// dec
floor_inline_always int32_t atomic_dec(volatile int32_t* p) {
	return __c11_atomic_fetch_sub((_Atomic(int32_t)*)p, 1, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_dec(volatile uint32_t* p) {
	return __c11_atomic_fetch_sub((_Atomic(uint32_t)*)p, 1u, memory_order_relaxed);
}
floor_inline_always float atomic_dec(volatile float* p) {
	return atomic_sub(p, 1.0f);
}
floor_inline_always int64_t atomic_dec(volatile int64_t* p) {
	return __c11_atomic_fetch_sub((_Atomic(int64_t)*)p, 1ll, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_dec(volatile uint64_t* p) {
	return __c11_atomic_fetch_sub((_Atomic(uint64_t)*)p, 1ull, memory_order_relaxed);
}
floor_inline_always double atomic_dec(volatile double* p) {
	return atomic_sub(p, 1.0);
}

// xchg
floor_inline_always int32_t atomic_xchg(volatile int32_t* p, int32_t val) {
	return __c11_atomic_exchange((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_xchg(volatile uint32_t* p, uint32_t val) {
	return __c11_atomic_exchange((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always float atomic_xchg(volatile float* p, float val) {
	const uint32_t ret = __c11_atomic_exchange((_Atomic(uint32_t)*)p, *(uint32_t*)&val, memory_order_relaxed);
	return *(float*)&ret;
}
floor_inline_always int64_t atomic_xchg(volatile int64_t* p, int64_t val) {
	return __c11_atomic_exchange((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_xchg(volatile uint64_t* p, uint64_t val) {
	return __c11_atomic_exchange((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always double atomic_xchg(volatile double* p, double val) {
	const uint64_t ret = __c11_atomic_exchange((_Atomic(uint64_t)*)p, *(uint64_t*)&val, memory_order_relaxed);
	return *(double*)&ret;
}

// min (not natively supported)
floor_inline_always int32_t atomic_min(volatile int32_t* p, int32_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(min, , p, val)
}
floor_inline_always uint32_t atomic_min(volatile uint32_t* p, uint32_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(min, , p, val)
}
floor_inline_always float atomic_min(volatile float* p, float val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(min, , p, val)
}
floor_inline_always int64_t atomic_min(volatile int64_t* p, int64_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(min, , p, val)
}
floor_inline_always uint64_t atomic_min(volatile uint64_t* p, uint64_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(min, , p, val)
}
floor_inline_always double atomic_min(volatile double* p, double val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(min, , p, val)
}

// max (not natively supported)
floor_inline_always int32_t atomic_max(volatile int32_t* p, int32_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(max, , p, val)
}
floor_inline_always uint32_t atomic_max(volatile uint32_t* p, uint32_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(max, , p, val)
}
floor_inline_always float atomic_max(volatile float* p, float val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_32(max, , p, val)
}
floor_inline_always int64_t atomic_max(volatile int64_t* p, int64_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(max, , p, val)
}
floor_inline_always uint64_t atomic_max(volatile uint64_t* p, uint64_t val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(max, , p, val)
}
floor_inline_always double atomic_max(volatile double* p, double val) {
	FLOOR_ATOMIC_FALLBACK_FUNC_OP_64(max, , p, val)
}

// and
floor_inline_always int32_t atomic_and(volatile int32_t* p, int32_t val) {
	return __c11_atomic_fetch_and((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_and(volatile uint32_t* p, uint32_t val) {
	return __c11_atomic_fetch_and((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always int64_t atomic_and(volatile int64_t* p, int64_t val) {
	return __c11_atomic_fetch_and((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_and(volatile uint64_t* p, uint64_t val) {
	return __c11_atomic_fetch_and((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}

// or
floor_inline_always int32_t atomic_or(volatile int32_t* p, int32_t val) {
	return __c11_atomic_fetch_or((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_or(volatile uint32_t* p, uint32_t val) {
	return __c11_atomic_fetch_or((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always int64_t atomic_or(volatile int64_t* p, int64_t val) {
	return __c11_atomic_fetch_or((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_or(volatile uint64_t* p, uint64_t val) {
	return __c11_atomic_fetch_or((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}

// xor
floor_inline_always int32_t atomic_xor(volatile int32_t* p, int32_t val) {
	return __c11_atomic_fetch_xor((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_xor(volatile uint32_t* p, uint32_t val) {
	return __c11_atomic_fetch_xor((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always int64_t atomic_xor(volatile int64_t* p, int64_t val) {
	return __c11_atomic_fetch_xor((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_xor(volatile uint64_t* p, uint64_t val) {
	return __c11_atomic_fetch_xor((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}

// store
floor_inline_always void atomic_store(volatile int32_t* p, int32_t val) {
	__c11_atomic_store((_Atomic(int32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always void atomic_store(volatile uint32_t* p, uint32_t val) {
	__c11_atomic_store((_Atomic(uint32_t)*)p, val, memory_order_relaxed);
}
floor_inline_always void atomic_store(volatile float* p, float val) {
	__c11_atomic_store((_Atomic(uint32_t)*)p, *(uint32_t*)&val, memory_order_relaxed);
}
floor_inline_always void atomic_store(volatile int64_t* p, int64_t val) {
	__c11_atomic_store((_Atomic(int64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always void atomic_store(volatile uint64_t* p, uint64_t val) {
	__c11_atomic_store((_Atomic(uint64_t)*)p, val, memory_order_relaxed);
}
floor_inline_always void atomic_store(volatile double* p, double val) {
	__c11_atomic_store((_Atomic(uint64_t)*)p, *(uint64_t*)&val, memory_order_relaxed);
}

// load
floor_inline_always int32_t atomic_load(volatile int32_t* p) {
	return __c11_atomic_load((_Atomic(int32_t)*)p, memory_order_relaxed);
}
floor_inline_always uint32_t atomic_load(volatile uint32_t* p) {
	return __c11_atomic_load((_Atomic(uint32_t)*)p, memory_order_relaxed);
}
floor_inline_always float atomic_load(volatile float* p) {
	const uint32_t ret = __c11_atomic_load((_Atomic(uint32_t)*)p, memory_order_relaxed);
	return *(float*)&ret;
}
floor_inline_always int64_t atomic_load(volatile int64_t* p) {
	return __c11_atomic_load((_Atomic(int64_t)*)p, memory_order_relaxed);
}
floor_inline_always uint64_t atomic_load(volatile uint64_t* p) {
	return __c11_atomic_load((_Atomic(uint64_t)*)p, memory_order_relaxed);
}
floor_inline_always double atomic_load(volatile double* p) {
	const uint64_t ret = __c11_atomic_load((_Atomic(uint64_t)*)p, memory_order_relaxed);
	return *(double*)&ret;
}

#endif

#endif
