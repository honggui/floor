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

#ifndef __FLOOR_VULKAN_MEMORY_HPP__
#define __FLOOR_VULKAN_MEMORY_HPP__

#include <floor/compute/vulkan/vulkan_common.hpp>

#if !defined(FLOOR_NO_VULKAN)

#include <floor/compute/compute_memory.hpp>

class vulkan_device;
class vulkan_queue;
class compute_queue;

//! helper class for common code between vulkan_buffer and vulkan_image
class vulkan_memory {
public:
	vulkan_memory(const vulkan_device* device,
				  const uint64_t* object,
				  // if false: is buffer
				  const bool is_image) noexcept;

	vulkan_memory(const vulkan_device* device_,
				  const VkBuffer* buffer) noexcept :
	vulkan_memory(device_, (const uint64_t*)buffer, false) {}

	vulkan_memory(const vulkan_device* device_,
				  const VkImage* image) noexcept :
	vulkan_memory(device_, (const uint64_t*)image, true) {}

	virtual ~vulkan_memory() noexcept;
	
protected:
	const vulkan_device* device;
	const uint64_t* object { nullptr };
	VkDeviceMemory mem { nullptr };
	const bool is_image { false };
	
	struct vulkan_mapping {
		VkBuffer buffer;
		VkDeviceMemory mem;
		const size_t size;
		const size_t offset;
		const COMPUTE_MEMORY_MAP_FLAG flags;
	};
	// stores all mapped pointers and the mapped buffer
	unordered_map<void*, vulkan_mapping> mappings;
	
	//! if cqueue isn't nullptr, returns cqueue, otherwise returns the devices default compute_queue
	shared_ptr<compute_queue> queue_or_default_compute_queue(shared_ptr<compute_queue> cqueue) const;
	
	//! overwrites memory data with the host data pointed to by data, with the specified size/offset
	bool write_memory_data(shared_ptr<compute_queue> cqueue,
						   void* data, const size_t& size, const size_t& offset,
						   const char* error_msg_on_failure = nullptr);
	
	void* __attribute__((aligned(128))) map(shared_ptr<compute_queue> cqueue,
											const COMPUTE_MEMORY_MAP_FLAG flags,
											const size_t size = 0, const size_t offset = 0);
	
	void unmap(shared_ptr<compute_queue> cqueue, void* __attribute__((aligned(128))) mapped_ptr);
	
	virtual void image_copy_dev_to_host(VkCommandBuffer, VkBuffer) {}
	virtual void image_copy_host_to_dev(VkCommandBuffer, VkBuffer) {}
	
};

#endif

#endif
