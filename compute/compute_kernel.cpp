/*
 *  Flo's Open libRary (floor)
 *  Copyright (C) 2004 - 2016 Florian Ziesche
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

#include <floor/compute/compute_kernel.hpp>

compute_kernel::~compute_kernel() {}

uint3 compute_kernel::check_local_work_size(const compute_kernel::kernel_entry& entry, const uint3& local_work_size) {
	// make sure all elements are always at least 1
	uint3 ret = local_work_size.maxed(1u);
	
	const auto work_group_size = ret.x * ret.y * ret.z;
	if(entry.max_total_local_size > 0 &&
	   work_group_size > entry.max_total_local_size) {
		// only warn/error once about this, don't want to spam the console/log unnecessarily
		bool do_warn = false;
		{
			GUARD(warn_map_lock);
			do_warn = (warn_map.count(&entry) == 0);
			warn_map.insert(&entry, true);
		}
		
		// if local work size y-dim is > 1, max work-size is > 1 and device work-group item sizes y-dim is > 2, set it at least to 2
		// note that this is usually a good idea for image accesses / cache use
		if(local_work_size.y > 1 && entry.max_total_local_size > 1 && entry.max_local_size.y > 1) {
			ret = { (uint32_t)(entry.max_total_local_size / 2u), 2, 1 };
			// TODO: might want to have/keep a specific shape
		}
		// just return max possible local work size "{ max, 1, 1 }"
		else ret = { (uint32_t)entry.max_total_local_size, 1, 1 };
		
		if(do_warn) {
			log_error("specified work-group size (%u) too large for this device (max: %u) - using %v now!",
					  work_group_size, entry.max_total_local_size, ret);
		}
	}
	return ret;
}
