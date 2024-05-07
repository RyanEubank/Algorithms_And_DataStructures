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

#include "collection.h"
#include "indexable.h"
#include "searchable.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// The concept associative defines an interface for template container 
	/// types that provide associative access. Associative types must meet the 
	/// following requirements:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///		- The type is a ranged collection template type.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is a standard forward range.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type overloads methods for efficient search by element type.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is declares associative access, insertion, and removal
	///       methods.
	///		</term></item></para>
	/// </list>
	///  
	/// </summary> ------------------------------------------------------------
	template <
		class T, 
		class begin_it = input_iterator_archetype<typename T::value_type>, 
		class end_it = input_iterator_archetype<typename T::value_type>,
		class ...Args
	>
	concept associative = 
		std::ranges::forward_range<T> &&
		ranged_collection<T> &&
		searchable<T> && 
		std::input_iterator<begin_it> &&
		std::sentinel_for<end_it, begin_it> &&
		requires(
			T& c1,
			begin_it begin,
			end_it end,
			typename T::const_reference lval_element,
			typename T::value_type&& rval_element,
			typename T::const_iterator position,
			Args&&... args
		) {
			{ c1.insert(lval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(rval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(begin, end) } -> std::same_as<typename T::iterator>;
			{ c1.remove(position) } -> std::same_as<typename T::iterator>;
			{ c1.remove(position, position) } -> std::same_as<typename T::iterator>;
			{ c1.emplace(args...) } -> std::same_as<typename T::iterator>;
			{ c1.insert(position, lval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(position, rval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(position, begin, end) } -> std::same_as<typename T::iterator>;
			{ c1.emplace(position, args...) } -> std::same_as<typename T::iterator>;
		};
}