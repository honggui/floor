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

#ifndef __FLOOR_COMPUTE_DEVICE_METAL_IMAGE_HPP__
#define __FLOOR_COMPUTE_DEVICE_METAL_IMAGE_HPP__

#if defined(FLOOR_COMPUTE_METAL)

// note that this is identical to sampler_t (when compiling in metal mode), but also necessary here, b/c of conversion requirements
typedef constant struct _sampler_t* metal_sampler_t;

//
namespace metal_image {
	//////////////////////////////////////////
	// sampler type, as defined by apple
	// NOTE: only the constexpr version is supported right now
	struct sampler {
		enum ADDRESS_MODE {
			CLAMP_TO_ZERO	= 0,
			CLAMP_TO_EDGE	= 1,
			REPEAT			= 2,
			MIRRORED_REPEAT	= 3
		};
		enum FILTER_MODE {
			NEAREST			= 0,
			LINEAR			= 1,
			// metal 1.2+
			BICUBIC			= 2,
		};
		enum MIP_FILTER_MODE {
			MIP_NONE		= 0,
			MIP_NEAREST		= 1,
			MIP_LINEAR		= 2
		};
		enum COORD_MODE {
			NORMALIZED		= 0,
			PIXEL			= 1
		};
		enum COMPARE_FUNCTION {
			NONE			= 0,
			LESS			= 1,
			LESS_EQUAL		= 2,
			GREATER			= 3,
			GREATER_EQUAL	= 4,
			EQUAL			= 5,
			NOT_EQUAL		= 6,
			ALWAYS			= 7,
			NEVER			= 8
		};
		
		union {
			struct {
				// address modes
				uint64_t s_address : 3;
				uint64_t t_address : 3;
				uint64_t r_address : 3;
				
				// filter modes
				uint64_t mag_filter : 2;
				uint64_t min_filter : 2;
				uint64_t mip_filter : 2;
				
				// coord mode
				uint64_t coord_mode : 1;
				
				// compare function
				uint64_t compare_function : 4;
				
				// metal 1.2+: anisotropic filtering (N - 1)
				uint64_t anisotropy : 4;
				
				// metal 1.2+: lod min/max clamping (half floats)
				uint64_t lod_clamp_min : 16;
				uint64_t lod_clamp_max : 16;
				
				// currently unused/reserved
				uint64_t _unused : 7;
				
				// constant sampler flag
				uint64_t is_constant : 1;
			};
			uint64_t value;
		};
		
		// must be known at compile-time for now
		constexpr sampler(const ADDRESS_MODE address_mode = CLAMP_TO_EDGE,
						  const COORD_MODE coord_mode_ = PIXEL,
						  const FILTER_MODE filter_mode = NEAREST,
						  const MIP_FILTER_MODE mip_filter_mode = MIP_NONE,
						  const COMPARE_FUNCTION compare_function_ = NONE) :
		s_address(address_mode), t_address(address_mode), r_address(address_mode),
		coord_mode(coord_mode_),
		mag_filter(filter_mode), min_filter(filter_mode),
		mip_filter(mip_filter_mode),
		compare_function(compare_function_),
		anisotropy(0),
		lod_clamp_min(0), lod_clamp_max(0x7BFF /* __HALF_MAX__ */),
		_unused(0u), is_constant(1u) {}
		
		constexpr sampler(const sampler& s) :
		s_address(s.s_address), t_address(s.t_address), r_address(s.r_address),
		mag_filter(s.mag_filter), min_filter(s.min_filter), mip_filter(s.mip_filter),
		coord_mode(s.coord_mode),
		compare_function(s.compare_function),
		anisotropy(0),
		lod_clamp_min(0), lod_clamp_max(0x7BFF /* __HALF_MAX__ */),
		_unused(0u), is_constant(1u) {}
		
		// provide metal_sampler_t conversion, so the builtin sampler_t can be initialized
		constexpr operator metal_sampler_t() const { return (metal_sampler_t)value; }
		
		// unavailable
		void operator=(const sampler&) = delete;
		void operator&() = delete;
	};
	static_assert(sizeof(sampler) == 8, "invalid sampler size");
	
}

#endif

#endif
