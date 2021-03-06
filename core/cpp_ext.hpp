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

#ifndef __FLOOR_CPP_EXT_HPP__
#define __FLOOR_CPP_EXT_HPP__

// NOTE: this header adds misc base c++ functionality that either will be part of a future c++ standard, or should be part of it
// NOTE: also don't include this header on its own, this is either included through core/cpp_headers.hpp or device/common.hpp

// non-member size (N4280, also provided by libc++ 3.6+ in c++1z mode and VS2015)
#if ((__cplusplus <= 201402L) || (_LIBCPP_STD_VER <= 14)) && !defined(_MSC_VER)
template <class C> floor_inline_always constexpr auto size(const C& c) noexcept -> decltype(c.size()) {
	return c.size();
}
template <class T, size_t N> floor_inline_always constexpr size_t size(const T (&)[N]) noexcept {
	return N;
}
#endif

// string is not supported on compute
#if !defined(FLOOR_COMPUTE) || defined(FLOOR_COMPUTE_HOST)

// for whatever reason there is no "string to 32-bit uint" conversion function in the standard
#if !defined(FLOOR_NO_STOU)
floor_inline_always static uint32_t stou(const string& str, size_t* pos = nullptr, int base = 10) {
	const auto ret = stoull(str, pos, base);
	if(ret > 0xFFFFFFFFull) {
		return numeric_limits<uint32_t>::max();
	}
	return (uint32_t)ret;
}
#endif
// same for size_t
#if !defined(FLOOR_NO_STOSIZE)
#if defined(PLATFORM_X32)
floor_inline_always static size_t stosize(const string& str, size_t* pos = nullptr, int base = 10) {
	const auto ret = stoull(str, pos, base);
	if(ret > 0xFFFFFFFFull) {
		return (size_t)numeric_limits<uint32_t>::max();
	}
	return (unsigned int)ret;
}
#elif defined(PLATFORM_X64)
floor_inline_always static size_t stosize(const string& str, size_t* pos = nullptr, int base = 10) {
	return (size_t)stoull(str, pos, base);
}
#endif
#endif
// and for bool
#if !defined(FLOOR_NO_STOB)
floor_inline_always static bool stob(const string& str) {
	return (str == "1" || str == "true" || str == "TRUE" || str == "YES");
}
#endif

#endif // !FLOOR_COMPUTE || FLOOR_COMPUTE_HOST

// apply(function, tuple): calls a function with the unpacked tuple elements used as the function arguments
#if !defined(FLOOR_NO_APPLY)
// ref N3658: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3658.html
// -> removed return types, since c++14 has auto-deduced return types + added constexpr
template<typename F, typename Tuple, size_t... I>
constexpr auto apply_impl(F&& f, Tuple&& args, index_sequence<I...>) {
	return forward<F>(f)(get<I>(forward<Tuple>(args))...);
}

template<typename F, typename Tuple, typename Indices = make_index_sequence<tuple_size<Tuple>::value>>
constexpr auto apply(F&& f, Tuple&& args) {
	return apply_impl(forward<F>(f), forward<Tuple>(args), Indices());
}
#endif

#endif
