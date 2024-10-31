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

#include <memory>

#include "../archetypes/iterator_archetypes.h"
#include "../archetypes/range_archetypes.h"
#include "../concepts/finite.h"
#include "../concepts/printable.h"
#include "../concepts/streamable.h"
#include "../util/NamedType.h"

namespace collections {

	struct from_range_t { explicit from_range_t() = default; };
	inline constexpr from_range_t from_range{};

	template <class T, class U>
	using rebind = std::allocator_traits<T>::template rebind_alloc<U>;

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines the minimal set of requiredc type aliases for collections.
	/// </para>
	/// 
	/// Required aliases:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- value_type
	///		</term></item></para>
	///		<para><item><term>
	///			- size_type
	///		</term></item></para>
	///		<para><item><term>
	///			- difference_type
	///		</term></item></para>
	///		<para><item><term>
	///			- reference
	///		</term></item></para>
	///		<para><item><term>
	///			- const_reference
	///		</term></item></para>
	///		<para><item><term>
	///			- pointer
	///		</term></item></para>
	///		<para><item><term>
	///			- const_pointer
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> --------------------------------------------------------
	template <class T>
	concept collection_type_traits = requires {
		typename T::value_type;
		typename T::size_type;
		typename T::difference_type;
		typename T::reference;
		typename T::const_reference;
		typename T::pointer;
		typename T::const_pointer;
	};

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines the constructors required for collection types.
	/// </para>
	/// 
	/// Required constructors:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type is default constructible.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible from an initializer list of
	///		      value_types.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible from an iterator/sentinel over a set
	///			  of value_types.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible from a range over a set of 
	///			  value_types.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> -------------------------------------------------------------
	template <
		class T,
		class iterator	= input_iterator_archetype<typename T::value_type>,
		class sentinel	= iterator,
		class range		= input_range_archetype<typename T::value_type>,
		class ilist		= std::initializer_list<typename T::value_type>
	>
	concept collection_constructible =
		std::input_iterator<iterator> &&
		std::sentinel_for<sentinel, iterator> &&
		std::ranges::input_range<range> &&
		std::is_default_constructible_v<T> &&
		std::is_constructible_v<T, ilist> &&
		std::is_constructible_v<T, iterator, sentinel> &&
		std::is_constructible_v<T, from_range_t, range&&>;

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines the basic requirement for a collection type.
	/// </para>
	/// 
	/// Collections must meet the following requirements:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type is regular (defined by std::regular<T>)
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is  streamable.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type can be printed in human readble format.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is finite in size.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type has at minimum defined collections type traits.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible as defined by 
	///		      collection_constructible<T>.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> -------------------------------------------------------------
	template <class T>
	concept collection =
		collection_type_traits<T> &&
		collection_constructible<T> &&
		finite<T> &&
		//printable<T, uint8_t> && //TODO decide if this requirement is really neccessary or if a collections::print() method will cover all cases
		streamable<T, uint8_t> &&
		std::regular<T>;
}