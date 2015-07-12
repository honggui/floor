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

// TODO: other atomic functions
floor_inline_always uint32_t atomic_cmpxchg(volatile uint32_t* p, uint32_t cmp, uint32_t val) {
	return __c11_atomic_compare_exchange_weak((_Atomic(uint32_t)*)p, &cmp, val, memory_order_relaxed, memory_order_relaxed);
}

#endif

#endif