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

#include "collection.h"

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines requirements for collections, containers, and adpaters with
	/// positional access via its iterators.
	/// </para>
	/// 
	/// Required methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			iterator insertAt(const_iterator, const lvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator insertAt(const_iterator, rvalue reference)
	///		</term></item></para> 
	///		<para><item><term>
	///			iterator insertAt(const_iterator, input_iterator&lt;T&gt;, sentinel&lt;T&gt;)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator emplaceAt(const_iterator, Args&lt;T&gt;...)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator removeAt(const_iterator)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator removeAt(const_iterator, const_iterator)
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> --------------------------------------------------------
	template <
		class T,
		class iterator = input_iterator_archetype<typename T::value_type>,
		class sentinel = iterator,
		class ...Args
	>
	concept positional = 
		std::ranges::input_range<T> && 
		collection_type_traits<T> &&
	requires (
		T& obj,
		iterator begin,
		sentinel end,
		typename T::const_reference lval,
		typename T::value_type&& rval,
		typename T::const_iterator position,
		Args&&... args
	) {
		{ obj.insert(position, lval) } 
			-> std::convertible_to<typename T::iterator>;

		{ obj.insert(position, rval) } 
			-> std::convertible_to<typename T::iterator>;

		{ obj.insert(position, begin, end) } 
			-> std::convertible_to<typename T::iterator>;

		{ obj.emplace(position, args...) } 
			-> std::convertible_to<typename T::iterator>;

		{ obj.remove(position) } 
			-> std::convertible_to<typename T::iterator>;

		{ obj.remove(position, position) } 
			-> std::convertible_to<typename T::iterator>;
	};
}