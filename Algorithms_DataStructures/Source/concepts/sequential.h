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

#include "../concepts/collection.h"
#include "../concepts/indexable.h"
#include "../util/NamedType.h"

namespace collections {

	using Index = NamedType<size_t, struct IndexType>;

	struct IndexRange {
		size_t begin;
		size_t end;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// The is_list concept defines behaviour and an interfacce for enforcing a 
	/// list-like interface on the given type. Lists must meet the following 
	/// requirements:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///		- The type is a collection template type.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is a standard forward range.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is indexable.
	///		</term></item></para>
	///		<para><item><term>
	///		- The type is associative.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <
		class T, 
		class begin_it = typename T::iterator, 
		class end_it = typename T::iterator,
		class ...Args
	>
	concept sequential = 
		collection<T> &&
		std::ranges::forward_range<T> &&
		indexable<T, typename T::value_type, typename T::size_type> &&
		std::input_iterator<begin_it> &&
		std::sentinel_for<end_it, begin_it> &&
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
			{ c1.insert(index, lval_element) };
			{ c1.insert(index, rval_element) };
			{ c1.insert(position, lval_element) };
			{ c1.insert(position, rval_element) };
			{ c1.insert(position, begin, end) };
			{ c1.insert(index, begin, end) };
			{ c1.removeFront() };
			{ c1.removeBack() };
			{ c1.remove(index) };
			{ c1.remove(position) };
			{ c1.remove(index_range) };
			{ c1.remove(position, position) };
			{ c1.emplaceFront(args...) };
			{ c1.emplaceBack(args...) };
			{ c1.emplace(index, args...) };
			{ c1.emplace(position, args...) };
		};
}