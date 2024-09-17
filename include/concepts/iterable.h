/* ============================================================================
* Copyright (C) 2023 Ryan Eubank
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
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
* ========================================================================= */

#pragma once

#include <concepts>
#include <ranges>
#include <type_traits>

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines requirements for iterable collection types. Iterable types are 
	/// suitable for single pass algorithms.
	/// </para>
	/// 
	/// Required methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- const_iterator cbegin() const
	///		</term></item></para>
	///		<para><item><term>
	///			- const_iterator cend() const
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> -------------------------------------------------------------
	template <class T>
	concept iterable = std::ranges::input_range<T> &&
	requires(const T& obj) {
		{ obj.begin() } -> std::convertible_to<typename T::const_iterator>;
		{ obj.end()	}	-> std::sentinel_for<typename T::const_iterator>;
		{ obj.cbegin() } -> std::convertible_to<typename T::const_iterator>;
		{ obj.cend() }	-> std::sentinel_for<typename T::const_iterator>;
	};

	// -------------------------------------------------------------------------
	/// <summary>
	/// Defines requirements for forward-iterable collection types. 
	/// Forward-iterable types are suitable for multiple pass algorithms.
	/// </summary> -------------------------------------------------------------
	template <class T>
	concept forward_iterable = std::ranges::forward_range<T> && iterable<T>;

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines requirements for bidirectional-iterable collection types. 
	/// Bidirectionally-iterable types are suitable for forward and backwards
	/// multiple pass algorithms.
	/// </para>
	/// 
	/// Required methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- reverse_iterator rbegin()
	///		</term></item></para>
	///		<para><item><term>
	///			- reverse_iterator rbegin() rend() 
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reverse_iterator rbegin() const
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reverse_iterator rbegin() rend() const
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reverse_iterator crbegin() const
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reverse_iterator crbegin() rend() const
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> -------------------------------------------------------------
	template <class T>
	concept bidirectionally_iterable = 
		std::ranges::bidirectional_range<T> && 
		iterable<T> &&
	requires(T& obj1, const T& obj2) {
		typename T::reverse_iterator;
		typename T::const_reverse_iterator;
		{ obj1.rbegin() }	-> std::convertible_to<typename T::reverse_iterator>;
		{ obj1.rend() }		-> std::sentinel_for<typename T::reverse_iterator>;
		{ obj2.rbegin() }	-> std::convertible_to<typename T::const_reverse_iterator>;
		{ obj2.rend() }		-> std::sentinel_for<typename T::const_reverse_iterator>;
		{ obj2.crbegin() }	-> std::convertible_to<typename T::const_reverse_iterator>;
		{ obj2.crend()	}	-> std::sentinel_for<typename T::const_reverse_iterator>;
	};

	// -------------------------------------------------------------------------
	/// <summary>
	/// Defines requirements for random access iterable collection types.
	/// Random access types are suitable for constant time access to elements.
	/// </summary> -------------------------------------------------------------
	template <class T>
	concept random_access_iterable = 
		std::ranges::random_access_range<T> &&
		bidirectionally_iterable<T>;

}
