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
#include <iterator>
#include <ranges>
#include <type_traits>

#include "collection.h"
#include "../archetypes/iterator_archetypes.h"
#include "../util/NamedType.h"

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines requirements for collections, containers, and adapters that can
	/// be accessed and modified via an index.
	/// </para>
	/// 
	/// Required methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- reference operator[](index)
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reference operator[](index) const
	///		</term></item></para>
	///		<para><item><term>
	///			- reference at(index)
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reference at(index) const
	///		</term></item></para>
	///		<para><item><term>
	///			- iterator insert(strong_index, lvalue)
	///		</term></item></para>
	///		<para><item><term>
	///			- iterator insert(strong_index, rvalue)
	///		</term></item></para>
	///		<para><item><term>
	///			- iterator insert(strong_index, begin, end)
	///		</term></item></para>
	///		<para><item><term>
	///			- iterator emplace(strong_index, Args...)
	///		</term></item></para>
	/// 	<para><item><term>
	///			- iterator remove(strong_index)
	///		</term></item></para>
	///	</list>
	///
	/// </summary> -------------------------------------------------------------
	template <
		class T, 
		class index_t,
		class strong_index_t = NamedType<index_t, struct IndexType>,
		class input_iterator = input_iterator_archetype<typename T::value_type>, 
		class ...Args
	>
	concept indexable = 
		collection_type_traits<T> &&
		std::ranges::input_range<T> && 
	requires (
		T& obj1,
		const T& obj2,
		index_t index,
		strong_index_t str_index,
		typename T::const_reference lval,
		typename T::value_type&& rval,
		input_iterator iter,
		Args&&... args
	) {
		{ obj1[index] }
			-> std::convertible_to<typename T::reference>;

		{ obj2[index] }
			-> std::convertible_to<typename T::const_reference>;

		{ obj1.at(index) }
			-> std::convertible_to<typename T::reference>;

		{ obj2.at(index) }
			-> std::convertible_to<typename T::const_reference>;

		{ obj1.insert(str_index, lval) }
			-> std::convertible_to<typename T::iterator>;

		{ obj1.insert(str_index, rval) }
			-> std::convertible_to<typename T::iterator>;

		{ obj1.insert(str_index, iter, iter) }
			-> std::convertible_to<typename T::iterator>;

		{ obj1.emplace(str_index, args...) }
			-> std::convertible_to<typename T::iterator>;

		{ obj1.remove(str_index) }
			-> std::convertible_to<typename T::iterator>;
	};
}