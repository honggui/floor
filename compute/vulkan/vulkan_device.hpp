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

#ifndef __FLOOR_VULKAN_DEVICE_HPP__
#define __FLOOR_VULKAN_DEVICE_HPP__

#include <floor/compute/vulkan/vulkan_common.hpp>
#include <floor/compute/compute_device.hpp>
#include <memory>

FLOOR_PUSH_WARNINGS()
FLOOR_IGNORE_WARNING(weak-vtables)

#if !defined(FLOOR_NO_VULKAN)
class vulkan_compute;
#endif

class vulkan_device final : public compute_device {
public:
	vulkan_device();
	~vulkan_device() override {}
	
#if !defined(FLOOR_NO_VULKAN)
	//! physical vulkan device
	VkPhysicalDevice physical_device { nullptr };
	
	//! logical vulkan device
	VkDevice device { nullptr };
	
	//! memory properties of the device/implementation/host
	shared_ptr<VkPhysicalDeviceMemoryProperties> mem_props;
#else
	void* _physical_device { nullptr };
	void* _device { nullptr };
	shared_ptr<void*> _mem_props;
#endif
	
	//! queue count per queue family
	//! (obviously also stores the queue family count)
	vector<uint32_t> queue_counts {};
	
	//! for internal purposes, do not change this
	uint32_t cur_queue_idx { 0 };
	
	//! max push constants size
	uint32_t max_push_constants_size { 0u };
	
	//! memory type index for device memory allocation
	uint32_t device_mem_index { ~0u };
	
	//! memory type index for (potentially cached) host + device-visible memory allocation
	uint32_t host_mem_cached_index { ~0u };
	
	//! memory type index for (potentially uncached) host + device-visible memory allocation
	uint32_t host_mem_uncached_index { ~0u };
	
	// put these at the end, b/c they are rather large
#if !defined(FLOOR_NO_VULKAN)
	//! fixed sampler descriptor set
	//! NOTE: this is allocated once at context creation
	VkDescriptorSetLayout fixed_sampler_desc_set_layout { nullptr };
	VkDescriptorPool fixed_sampler_desc_pool { nullptr };
	VkDescriptorSet fixed_sampler_desc_set { nullptr };
	
	//! fixed sampler set
	//! NOTE: this is allocated once at context creation
	vector<VkSampler> fixed_sampler_set;
	
	//! fixed sampler descriptor image infos, used to to update + bind the descriptor set
	//! NOTE: this solely consists of { nullptr, nullptr, 0 } objects, but is sadly necessary when updating/setting
	//!       the descriptor set (.sampler is ignored if immutable samplers are used, others are ignored anyways)
	vector<VkDescriptorImageInfo> fixed_sampler_image_info;
#else
	uint64_t _fixed_sampler_desc_set_layout;
	uint64_t _fixed_sampler_desc_pool;
	uint64_t _fixed_sampler_desc_set;
	vector<uint64_t> _fixed_sampler_set;
	struct _dummy_desc_img_info { void* _a; void* _b; uint32_t _c; };
	vector<_dummy_desc_img_info> _fixed_sampler_image_info;
#endif
	
};

FLOOR_POP_WARNINGS()

#endif
