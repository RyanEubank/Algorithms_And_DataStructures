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
#include "../util/NamedType.h"

namespace collections {

	using Size = NamedType<size_t, struct SizeType>;
	using Index = NamedType<size_t, struct IndexType>;

	struct IndexRange {
		size_t begin;
		size_t end;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// The sequential concept defines behaviour and an interfacce for 
	/// enforcing a list-like interface on the given type. Lists must meet the 
	/// following requirements:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///		- The type is a ranged collection.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is indexable by integral-type indexes.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is constructible by a size and (optional) value.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type declares sequential access, insertion, and removal
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
	concept sequential = 
		std::input_iterator<begin_it> &&
		std::sentinel_for<end_it, begin_it> &&
		bidirectional_ranged_collection<T> &&
		indexable<T, typename T::value_type, typename T::size_type> &&
		std::is_constructible_v<T, Size, typename T::value_type> &&
		requires(
			T& c1,
			const T& c2,
			typename T::const_reference lval_element,
			typename T::value_type&& rval_element,
			Index index,
			IndexRange index_range,
			typename T::const_iterator position,
			begin_it begin,
			end_it end,
			Args&&... args
		) {
			{ c1.front() } -> std::same_as<typename T::reference>;
			{ c2.front() } -> std::same_as<typename T::const_reference>;
			{ c1.back() } -> std::same_as<typename T::reference>;
			{ c2.back() } -> std::same_as<typename T::const_reference>;
			{ c1.insertFront(lval_element) };
			{ c1.insertFront(rval_element) };
			{ c1.insertBack(lval_element) };
			{ c1.insertBack(rval_element) };
			{ c1.insert(index, lval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(index, rval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(position, lval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(position, rval_element) } -> std::same_as<typename T::iterator>;
			{ c1.insert(position, begin, end) } -> std::same_as<typename T::iterator>;
			{ c1.insert(index, begin, end) } -> std::same_as<typename T::iterator>;
			{ c1.removeFront() };
			{ c1.removeBack() };
			{ c1.remove(index) } -> std::same_as<typename T::iterator>;
			{ c1.remove(position) } -> std::same_as<typename T::iterator>;
			{ c1.remove(index_range) } -> std::same_as<typename T::iterator>;
			{ c1.remove(position, position) } -> std::same_as<typename T::iterator>;
			{ c1.emplaceFront(args...) };
			{ c1.emplaceBack(args...) };
			{ c1.emplace(index, args...) } -> std::same_as<typename T::iterator>;
			{ c1.emplace(position, args...) } -> std::same_as<typename T::iterator>;
		};
}