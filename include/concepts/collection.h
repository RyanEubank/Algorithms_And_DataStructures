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

#include "../archetypes/iterator_archetypes.h"
#include "../archetypes/range_archetypes.h"
#include "../concepts/searchable.h"
#include "../concepts/streamable.h"
#include "../util/NamedType.h"

namespace collections {

	using Reserve = NamedType<size_t, struct ReserveType>;
	using Size = NamedType<size_t, struct SizeType>;


	struct from_range_t { explicit from_range_t() = default; };
	inline constexpr from_range_t from_range{};

	// ------------------------------------------------------------------------
	/// <summary>
	/// The concept collection_type_traits defines the minimal type aliases
	/// required of collection types.
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The collection has aliases for value_type, size_type,
	///           reference, const_reference, pointer, and const_pointer.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept collection_type_traits = requires {
		typename T::value_type;
		typename T::size_type;
		typename T::reference;
		typename T::const_reference;
		typename T::pointer;
		typename T::const_pointer;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// The concept basic_collection_interface defines the most basic methods 
	/// required for collection types.
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The collection has methods for clear(), size() and isEmpty().
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept basic_collection_interface = requires(T & c1, const T & c2) {
		{ c1.clear() };
		{ c2.size() } -> std::convertible_to<size_t>;
		{ c2.isEmpty() } -> std::convertible_to<bool>;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// The concept conditionally_three_way_comparable defines a requirement
	/// for collection types that they must implement the comparison (spaceship)
	/// operator if the elements they contain are also three way comparable.
	/// </summary> ------------------------------------------------------------
	template <class collection_t, class element_t>
	concept conditionally_three_way_comparable =
		std::three_way_comparable<
			collection_t, 
			decltype(element_t{} <=> element_t{}) 
		> ||
		!std::three_way_comparable<element_t>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// The concept collection_constructible defines a requirement for the
	/// constructors of collection types. The required constructors are as 
	/// follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type is default constructible.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible from an initializer list.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible from an iterator pair.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is constructible from a range.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <
		class collection,
		class iterator = input_iterator_archetype<typename collection::value_type>,
		class sentinel = iterator,
		class range = input_range_archetype<typename collection::value_type>,
		class range_tag = from_range_t
	>
	concept collection_constructible =
		std::input_iterator<iterator> &&
		std::sentinel_for<sentinel, iterator> &&
		std::ranges::input_range<range> &&
		std::is_same_v<range_tag, from_range_t>&&
		std::is_default_constructible_v<collection> &&
		std::is_constructible_v<
			collection,
			std::initializer_list<typename collection::value_type>
		> &&
		std::is_constructible_v<collection, iterator, sentinel> &&
		std::is_constructible_v<collection, range_tag, range&&> &&
		std::is_constructible_v<
			collection, 
			Size, 
			typename collection::value_type
		>;

	// ------------------------------------------------------------------------
	/// <summary> 
	/// The concept 'container' defines the behaviour and interface for 
	/// enforcing a collection-like interface on the given type. Collections 
	/// must meet the following requirements:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type has constructors defined in collection_constructible.
	///		</term></item></para>
	///		<para><item><term>
	///			- The element type is equality comparable.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type conforms to copy and move semantics.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type implements three way comparison if and only if its 
	///			  elements can be compared.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type implements stream operators.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines collection type traits.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type implements basic collection methods.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept collection =
		collection_constructible<T> &&
		std::equality_comparable<T> &&
		std::copyable<T> &&
		conditionally_three_way_comparable<T, typename T::value_type> &&
		streamable<T, uint8_t> &&
		collection_type_traits<T> &&
		basic_collection_interface<T>;
}