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
#include <functional>
#include <type_traits>

#include "collection.h"

namespace collections {

	// --------------------------------------------------------------------
	/// <summary><para>
	/// Defines the miniimal set of methods and operators for sequential
	/// collection types.
	/// </para>
	/// 
	/// Required methods:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///			iterator find(const lvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator find_if(predicate) -> where predicate is a 
	///			callable taking an element and returning a bool.
	///		</term></item></para>
	///		<para><item><term>
	///			const_iterator find(const lvalue reference) const
	///		</term></item></para>
	///		<para><item><term>
	///			const_iterator find_if(predicate) const -> where predicate 
	///			is a callable taking an element and returning a bool.
	///		</term></item></para>
	///		<para><item><term>
	///			iterator insert(const lvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator insert(rvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator insert(input_iterator&lt;T&gt;, sentinel&lt;T&gt;)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator remove(const_iterator)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator remove(const_iterator, const_iterator)
	///		</term></item></para>
	///		<para><item><term>
	///			iterator emplace(Args&lt;T&gt;...)
	///		</term></item></para>
	/// </list>
	///  
	/// </summary> --------------------------------------------------------
	template <
		class T, 
		class input_iter = input_iterator_archetype<typename T::value_type>, 
		class U = typename T::value_type,
		class ...Args
	>
	concept associative = 
		!std::convertible_to<U, typename T::const_iterator> && requires(
			T& c1,
			const T& c2,
			input_iter iterator,
			typename T::const_reference lval,
			typename T::value_type&& rval,
			typename T::const_iterator position,
			typename T::key_type key,
			U&& arg1,
			Args&&... args
		) {
			{ c1.find(key) }					-> std::convertible_to<typename T::iterator>;
			{ c1.contains(key) }				-> std::convertible_to<bool>;
			{ c2.find(key) }					-> std::convertible_to<typename T::const_iterator>;
			{ c2.contains(key) }				-> std::convertible_to<bool>;
			//TODO - require find_if
			{ c1.insert(lval) }					-> std::convertible_to<typename T::iterator>;
			{ c1.insert(rval) }					-> std::convertible_to<typename T::iterator>;
			{ c1.insert(iterator, iterator) }	-> std::convertible_to<typename T::iterator>;
			{ c1.remove(position) }				-> std::convertible_to<typename T::iterator>;
			{ c1.remove(position, position) }	-> std::convertible_to<typename T::iterator>;
			{ c1.emplace(arg1, args...) }		-> std::convertible_to<typename T::iterator>;
		};
}