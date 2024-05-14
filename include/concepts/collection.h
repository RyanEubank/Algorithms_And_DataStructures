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
#include <iterator>
#include <ranges>

#include "../archetypes/iterator_archetypes.h"
#include "../archetypes/range_archetypes.h"
#include "../concepts/streamable.h"
#include "../util/NamedType.h"

namespace collections {

	struct from_range_t { explicit from_range_t() = default; };
	inline constexpr from_range_t from_range{};

	using Size = NamedType<size_t, struct SizeType>;
	using Index = NamedType<size_t, struct IndexType>;

	struct IndexRange {
		size_t begin;
		size_t end;
	};

	namespace internal {

		template <class T, class U>
		concept like = std::convertible_to<T, U>;

		template <class T>
		using value_type_of = typename T::value_type;

		template <class T>
		using size_type_of = typename T::size_type;

		template <class T>
		using ref_type_of = typename T::reference;

		template <class T>
		using const_ref_type_of = typename T::const_reference;

		template <class T>
		using iter_type_of = typename T::iterator;
		
		template <class T>
		using const_iter_type_of = typename T::const_iterator;

		template <class T>
		using rev_iter_type_of = typename T::reverse_iterator;

		template <class T>
		using const_rev_iter_type_of = typename T::const_reverse_iterator;

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
			{ c2.size() } -> like<size_type_of<T>>;
			{ c2.isEmpty() } -> like<bool>;
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
			class iterator = input_iterator_archetype<value_type_of<T>>,
			class sentinel = iterator,
			class range = input_range_archetype<value_type_of<T>>,
			class ilist = std::initializer_list<value_type_of<T>>
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
		/// Requires a minimal set of methods defined for iterable collection 
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
		concept iterable_collection_interface = requires(T & c1, const T& c2) {
			{ c2.cbegin() } -> like<const_iter_type_of<T>>;
			{ c2.cend() } -> std::sentinel_for<const_iter_type_of<T>>;
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
		concept bidirectionally_iterable_collection_interface = 
		requires(T& c1, const T& c2) {
			typename T::reverse_iterator;
			typename T::const_reverse_iterator;
			{ c1.rbegin() } -> like<rev_iter_type_of<T>>;
			{ c1.rend() } -> std::sentinel_for<rev_iter_type_of<T>>;
			{ c2.rbegin() } -> like<const_rev_iter_type_of<T>>;
			{ c2.rend() } -> std::sentinel_for<const_rev_iter_type_of<T>>;
			{ c2.crbegin() } -> like<const_rev_iter_type_of<T>>;
			{ c2.crend() } -> std::sentinel_for<const_rev_iter_type_of<T>>;
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods defined for ordered collection 
		/// types. These methods have the following signatures:
		/// 
		/// <list type="bullet">
		///		<para><item><term>
		///			iterator insert(const_iterator, const lvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator insert(const_iterator, rvalue reference)
		///		</term></item></para> 
		///		<para><item><term>
		///			iterator insert(const_iterator, input_iterator&lt;T&gt;, sentinel&lt;T&gt;)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator emplace(const_iterator, Args&lt;T&gt;...)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator remove(const_iterator)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator remove(const_iterator, const_iterator)
		///		</term></item></para>
		/// </list>
		/// 
		/// </summary> --------------------------------------------------------
		template <
			class T,
			class begin_it = input_iterator_archetype<value_type_of<T>>, 
			class end_it = input_iterator_archetype<value_type_of<T>>,
			class ...Args
		>
		concept ordered_collection_interface = requires(
			T & c1,
			begin_it begin,
			end_it end,
			const_ref_type_of<T> lval_element,
			value_type_of<T>&& rval_element,
			const_iter_type_of<T> position,
			Args&&... args
		) {
			{ c1.insert(position, lval_element) } -> like<iter_type_of<T>>;
			{ c1.insert(position, rval_element) } -> like<iter_type_of<T>>;
			{ c1.insert(position, begin, end) } -> like<iter_type_of<T>>;
			{ c1.emplace(position, args...) } -> like<iter_type_of<T>>;
			{ c1.remove(position) } -> like<iter_type_of<T>>;
			{ c1.remove(position, position) } -> like<iter_type_of<T>>;
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods and operators for indexable
		/// collection types. The methods have the following signatures:
		/// 
		/// <list type="bullet">
		///		<para><item><term>
		///			reference operator[](index_t)
		///		</term></item></para>
		///		<para><item><term>
		///			const_reference operator[](index_t) const
		///		</term></item></para>
		///		<para><item><term>
		///			reference at(index_t)
		///		</term></item></para>
		///		<para><item><term>
		///			const_reference at(index_t) const
		///		</term></item></para>
		///		<para><item><term>
		///			iterator insert(strong_index_t, const lvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator insert(strong_index_t, rvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator insert(strong_index_t, input_iterator&lt;T&gt;, sentinel&lt;T&gt;)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator remove(strong_index_t)
		///		</term></item></para>
		///		<para><item><term>
		///			iterator emplace(strong_index_t, Args&lt;T&gt;...)
		///		</term></item></para>
		/// </list>
		///  
		/// </summary> ------------------------------------------------------------
		template <
			class T,
			class index_t,
			class strong_index_t = NamedType<index_t, struct IndexType>,
			class begin_it = input_iterator_archetype<value_type_of<T>>, 
			class end_it = input_iterator_archetype<value_type_of<T>>,
			class ...Args
		>
		concept indexable_collection_interface = requires(
			T& c1,
			const T& c2,
			const_ref_type_of<T> lval_element,
			value_type_of<T>&& rval_element,
			index_t index,
			strong_index_t str_index,
			begin_it begin,
			end_it end,
			Args&&... args
		) {
			{ c1[index] } -> like<ref_type_of<T>>;
			{ c2[index] } -> like<const_ref_type_of<T>>;
			{ c1.at(index) } -> like<ref_type_of<T>>;
			{ c2.at(index) } -> like<const_ref_type_of<T>>;
			{ c1.insert(str_index, lval_element) } -> like<iter_type_of<T>>;
			{ c1.insert(str_index, rval_element) } -> like<iter_type_of<T>>;
			{ c1.insert(str_index, begin, end) } -> like<iter_type_of<T>>;
			{ c1.remove(str_index) } -> like<iter_type_of<T>>;
			{ c1.emplace(str_index, args...) } -> like<iter_type_of<T>>;
		};


		// ------------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods and operators for sequential
		/// collection types. The methods have the following signatures:
		/// 
		///	<list type="bullet">
		///		<para><item><term>
		///			reference front()
		///		</term></item></para>
		///		<para><item><term>
		///			const_reference front() const
		///		</term></item></para>
		///		<para><item><term>
		///			reference back()
		///		</term></item></para>
		///		<para><item><term>
		///			const_reference back() const
		///		</term></item></para>
		///		<para><item><term>
		///			void insertFront(const lvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			void insertFront(rvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			void insertBack(const lvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			void insertBack(rvalue reference)
		///		</term></item></para>
		///		<para><item><term>
		///			void removeFront()
		///		</term></item></para>
		///		<para><item><term>
		///			void removeBack()
		///		</term></item></para>
		///		<para><item><term>
		///			void emplaceFront(Args&lt;T&gt;...)
		///		</term></item></para>
		///		<para><item><term>
		///			void emplaceBack(Args&lt;T&gt;...)
		///		</term></item></para>
		/// </list>
		/// 
		/// </summary> ------------------------------------------------------------
		template <class T,class ...Args>
		concept sequential_collection_interface = requires(
			T& c1,
			const T& c2,
			const_ref_type_of<T> lval_element,
			value_type_of<T>&& rval_element,
			Args&&... args
		) {
			{ c1.front() } -> like<ref_type_of<T>>;
			{ c2.front() } -> like<const_ref_type_of<T>>;
			{ c1.back() } -> like<ref_type_of<T>>;
			{ c2.back() } -> like<const_ref_type_of<T>>;
			{ c1.insertFront(lval_element) };
			{ c1.insertFront(rval_element) };
			{ c1.insertBack(lval_element) };
			{ c1.insertBack(rval_element) };
			{ c1.removeFront() };
			{ c1.removeBack() };
			{ c1.emplaceFront(args...) };
			{ c1.emplaceBack(args...) };
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods for searchable collection types.
		/// Generally collection whose seach algorithm is more efficient than
		/// standard linear search via <see cref="collections::find/>. The
		/// methods have the following signatures: 
		/// 
		/// <list type="bullet">
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
		/// </list>
		/// 
		/// </summary> --------------------------------------------------------
		template <
			class T, 
			class key_t = const_ref_type_of<T>,
			class predicate_t = std::function<bool(const_ref_type_of<T>)>
		>
		concept searchable_collection_interface = 
			std::predicate<predicate_t, const_ref_type_of<T>> &&
		requires (
			T& c1,
			const T& c2,
			key_t& key,
			predicate_t predicate
		) {
			{ c1.find(key) } -> like<iter_type_of<T>>;
			{ c1.find_if(predicate) } -> like<iter_type_of<T>>;
			{ c2.find(key) } -> like<const_iter_type_of<T>>;
			{ c2.find_if(predicate) } -> like<const_iter_type_of<T>>;
		};

		// --------------------------------------------------------------------
		/// <summary>
		/// Requires a minimal set of methods for associative collection types.
		/// The methods have the following signatures:
		/// 
		///	<list type="bullet">
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
			class begin_it = input_iterator_archetype<value_type_of<T>>, 
			class end_it = input_iterator_archetype<value_type_of<T>>,
			class ...Args
		>
		concept associative_collection_interface = requires(
			T& c1,
			begin_it begin,
			end_it end,
			const_ref_type_of<T> lval_element,
			value_type_of<T>&& rval_element,
			const_iter_type_of<T> position,
			Args&&... args
		) {
			{ c1.insert(lval_element) } -> like<iter_type_of<T>>;
			{ c1.insert(rval_element) } -> like<iter_type_of<T>>;
			{ c1.insert(begin, end) } -> like<iter_type_of<T>>;
			{ c1.remove(position) } -> like<iter_type_of<T>>;
			{ c1.remove(position, position) } -> like<iter_type_of<T>>;
			{ c1.emplace(args...) } -> like<iter_type_of<T>>;
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
	///			  <see cref="collections::internal::iterable_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list> 
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept iterable_collection =
		collection<T> &&
		std::ranges::input_range<T> &&
		internal::iterable_collection_interface<T>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for bidirectionally iterable 
	/// collections. The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is a bidirectional range at minimum, following the
	///			  requirements of <see cref="std::ranges::bidirectional_range{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines extra range methods for reverse_iterators as
	///           specified by 
	///			  <see cref="collections::internal::bidirectionally_iterable_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list> 
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept bidirectionally_iterable_collection =
		iterable_collection<T> &&
		std::ranges::bidirectional_range<T> &&
		internal::bidirectionally_iterable_collection_interface<T>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for ordered collection types.
	/// The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines methods for positional access required by
	///			  <see cref="internal::collections::ordered_collection_interface{T}/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type itself three way comparable if and only if its
	///           elements are three way comparable.
	///		</term></item></para>
	/// </list> 
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept ordered_collection =
		iterable_collection<T> &&
		internal::ordered_collection_interface<T> && (
			std::three_way_comparable<internal::value_type_of<T>> &&
			std::three_way_comparable<T>
		) || !std::three_way_comparable<internal::value_type_of<T>>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for indexable collection types.
	/// The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines methods for indexing according to
	///			  <see cref="collections::internal::indexable_collection_interface{T, index_t}"/>.
	///		</term></item></para>
	/// </list>  
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T, class index_t>
	concept indexable_collection =
		iterable_collection<T> &&
		internal::indexable_collection_interface<T, index_t>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for indexable collection types.
	/// The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines methods for sequential access according to
	///			  <see cref="collections::internal::sequential_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list>  
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept sequential_collection = 
		iterable_collection<T> &&
		internal::sequential_collection_interface<T>;
	
	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires comon behaviour and interface for searchable collection types
	/// that implement their own find and find_if algorithms efficiently. The
	/// requriements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines methods for searching elements according to
	///			  <see cref="collections::internal::searchable_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept searchable_collection =
		iterable_collection<T> &&
		internal::searchable_collection_interface<T>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires comon behaviour and interface for associative collection 
	/// types. The requriements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type defines methods for accessing elements according to
	///			  <see cref="collections::internal::associative_collection_interface{T}"/>.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept associative_collection =
		searchable_collection<T> &&
		internal::associative_collection_interface<T>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// Requires common behaviour and interface for list-like collection types.
	/// The requirements are as follows:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The type is constructible from a strongly type Size and value.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::bidirectionally_iterable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::sequential_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::ordered_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type meets all requirements defined in 
	///           <see cref="collections::indexable_collection{T}"/>.
	///		</term></item></para>
	///		<para><item><term>
	///			- The defines a remove methods with an index range:
	///			  iterator remove(IndexRange)
	///		</term></item></para>
	/// </list>  
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept list =
		std::is_constructible_v<T, Size, internal::value_type_of<T>> &&
		bidirectionally_iterable_collection<T> &&
		sequential_collection<T> &&
		ordered_collection<T> &&
		indexable_collection<T, internal::size_type_of<T>> &&
		requires(T& c, IndexRange index_range) {
			{ c.remove(index_range) } -> internal::like<internal::iter_type_of<T>>;
		};
}