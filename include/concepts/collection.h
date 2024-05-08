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

	struct from_range_t { explicit from_range_t() = default; };
	inline constexpr from_range_t from_range{};

	namespace internal {

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of type aliases defined for collection 
		/// types. These aliases include:
		/// 
		/// <list type="bullet">
		///		<para><item><term>
		///			- value_type
		///		</term></item></para>
		///		<para><item><term>
		///			- size_type
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
			typename T::reference;
			typename T::const_reference;
			typename T::pointer;
			typename T::const_pointer;
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods defined for collection types. 
		/// These methods have the following signatures:
		/// 
		/// <list type="bullet">
		///		<para><item><term>
		///			void clear()
		///		</term></item></para>
		///		<para><item><term>
		///			size_type size() const
		///		</term></item></para>
		///		<para><item><term>
		///			bool isEmpty() const
		///		</term></item></para>
		/// </list>
		/// 
		/// </summary> --------------------------------------------------------
		template <class T>
		concept collection_interface = requires(T& c1, const T& c2) {
			{ c1.clear() };
			{ c2.size() } -> std::convertible_to<typename T::size_type>;
			{ c2.isEmpty() } -> std::convertible_to<bool>;
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of constructors for collection types. The 
		/// required constructors are as follows:
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
		///			- The type is constructible from an iterator/sentinel over
		///			  a set of value_types.
		///		</term></item></para>
		///		<para><item><term>
		///			- The type is constructible from a range over a set of 
		///           value_types.
		///		</term></item></para>
		/// </list>
		/// 
		/// </summary> ---------------------------------------------------------
		template <
			class T,
			class iterator = input_iterator_archetype<typename T::value_type>,
			class sentinel = iterator,
			class range = input_range_archetype<typename T::value_type>,
			class ilist = std::initializer_list<typename T::value_type>
		>
		concept collection_constructors =
			std::input_iterator<iterator> &&
			std::sentinel_for<sentinel, iterator> &&
			std::ranges::input_range<range> &&
			std::is_default_constructible_v<T> &&
			std::is_constructible_v<T, ilist> &&
			std::is_constructible_v<T, iterator, sentinel> &&
			std::is_constructible_v<T, from_range_t, range&&>;

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods defined for ranged collection 
		/// types. These methods have the following signatures:
		/// 
		/// <list type="bullet">
		///		<para><item><term>
		///			const_iterator cbegin() const
		///		</term></item></para>
		///		<para><item><term>
		///			const_iterator cend() const
		///		</term></item></para>
		/// </list>
		/// 
		/// </summary> --------------------------------------------------------
		template <class T>
		concept ranged_collection_interface = requires(T & c1, const T& c2) {
			{ c2.cbegin() } -> std::same_as<typename T::const_iterator>;
			{ c2.cend() } -> std::same_as<typename T::const_iterator>;
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of aliases and methods defined for ranged 
		/// collection types. These aliases and methods have the following 
		/// signatures:
		/// 
		/// <list type="bullet">
		///		<para><item><term>
		///			typename reverse_iterator
		///		</term></item></para>
		///		<para><item><term>
		///			typename const_reverse_iterator
		///		</term></item></para>	
		///		<para><item><term>
		///			reverse_iterator rbegin()
		///		</term></item></para>
		///		<para><item><term>
		///			reverse_iterator rend()
		///		</term></item></para>
		///		<para><item><term>
		///			const_reverse_iterator rbegin() const
		///		</term></item></para>
		///		<para><item><term>
		///			const reverse_iterator rend() const
		///		</term></item></para>
		///		<para><item><term>
		///			const_reverse_iterator crbegin() const
		///		</term></item></para>
		///		<para><item><term>
		///			const reverse_iterator crend() const
		///		</term></item></para>
		/// </list>
		/// 
		/// </summary> --------------------------------------------------------
		template <class T>
		concept bidirectional_ranged_collection_interface = 
		requires(T& c1, const T& c2) {
			typename T::reverse_iterator;
			typename T::const_reverse_iterator;
			{ c1.rbegin() } -> std::same_as<typename T::reverse_iterator>;
			{ c1.rend() } -> std::same_as<typename T::reverse_iterator>;
			{ c2.rbegin() } -> std::same_as<typename T::const_reverse_iterator>;
			{ c2.rend() } -> std::same_as<typename T::const_reverse_iterator>;
			{ c2.crbegin() } -> std::same_as<typename T::const_reverse_iterator>;
			{ c2.crend() } -> std::same_as<typename T::const_reverse_iterator>;
		};
	}

	// ------------------------------------------------------------------------
	/// <summary> 
	/// Requires common behaviour and interface for collection types. The
	/// requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type has aliases for types defined in 
	///			  <see cref="collections::internal::collection_type_traits{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type has constructors defined in 
	///           <see cref="collections::internal::collection_constructors{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type has methods defined in 
	///           <see cref="collections::internal::collection_interface{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is <see cref="std::copyable"/>, obeying copy and
	///			  move semantics.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is equality comparable.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is has overloaded stream operators as defined in
	///			  <see cref="collections::streamable{T}"/>.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept collection =
		internal::collection_type_traits<T> &&
		internal::collection_constructors<T> &&
		internal::collection_interface<T> &&
		std::copyable<T> &&
		std::equality_comparable<T> &&
		streamable<T, uint8_t>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for range-based collections. 
	/// The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is an input range at minimum, following the
	///			  requirements of <see cref="std::ranges::input_range{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines extra range methods for const_iterators as
	///           specified by 
	///			  <see cref="collections::internal::ranged_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list> 
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept ranged_collection =
		collection<T> &&
		std::ranges::input_range<T> &&
		internal::ranged_collection_interface<T>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for bidirectional range-based 
	/// collections. The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::ranged_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is a bidirectional range at minimum, following the
	///			  requirements of <see cref="std::ranges::bidirectional_range{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines extra range methods for reverse_iterators as
	///           specified by 
	///			  <see cref="collections::internal::ranged_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list> 
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept bidirectional_ranged_collection =
		ranged_collection<T> &&
		std::ranges::bidirectional_range<T> &&
		internal::bidirectional_ranged_collection_interface<T>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for ordered collection types.
	/// The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::ranged_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The value_type (or elements) of the type are three way
	///			  comparable.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type itself is also three way comparable.
	///		</term></item></para>
	/// </list> 
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept ordered_collection =
		ranged_collection<T> &&
		std::three_way_comparable<typename T::value_type> &&
		std::three_way_comparable<T>;
}