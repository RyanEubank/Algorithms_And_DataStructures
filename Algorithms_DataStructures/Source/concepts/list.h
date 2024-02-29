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
		class end_it = typename T::iterator
	>
	concept list = 
		collection<T> &&
		std::ranges::forward_range<T> &&
		indexable<T, typename T::value_type, typename T::size_type> &&
		std::input_iterator<begin_it> &&
		std::sentinel_for<end_it, begin_it> &&
		requires(
			T& c1,
			const T& c2,
			typename T::const_reference element,
			typename T::value_type&& rval_element,
			const Index& index,
			IndexRange& index_range,
			typename T::const_iterator pos,
			begin_it begin,
			end_it end
		) {
			{ c1.front() } -> std::same_as<typename T::reference>;
			{ c2.front() } -> std::same_as<typename T::const_reference>;
			{ c1.back() } -> std::same_as<typename T::reference>;
			{ c2.back() } -> std::same_as<typename T::const_reference>;
			{ c1.insertFront(element) };
			{ c1.insertBack(element) };
			{ c1.insert(index, element) };
			{ c1.insert(pos, element) };
			{ c1.insert(pos, rval_element) };
			{ c1.insert(pos, begin, end) };
			{ c1.removeFront() };
			{ c1.removeBack() };
			{ c1.remove(index) };
			{ c1.remove(pos) };
			{ c1.remove(index_range) };
			{ c1.remove(pos, pos) };
		};
}