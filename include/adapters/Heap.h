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
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <istream>
#include <memory>
#include <ostream>
#include <ranges>
#include <utility>

#include "../concepts/collection.h"
#include "../containers/DynamicArray.h"

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary>
	/// K_Ary_Heap is a container adapter that implements heap operations 
	/// maintaining a random access container visualized as a tree. Heap elements 
	/// can be inserted, deleted, or searched, as well as modified to change 
	/// their priority if elements implement addition/subtraction operators.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of elements contained by the heap.
	/// </typeparam>
	/// 
	/// <typeparam name="degree">
	/// The degree of the heap, or how many children each node has.
	/// </typeparam>
	/// 
	/// <typeparam name="compare_t">
	/// The comparison function used to order elements.
	/// </typeparam>
	/// 
	/// <typeparam name="allocator_t">
	/// The type of the allocator used by the heap.
	/// </typeparam> -----------------------------------------------------------
	template <
		class element_t,
		size_t degree,
		class compare_t = std::less<element_t>,
		class container_t = DynamicArray<element_t>
	> requires (
		collection<container_t> &&
		std::ranges::random_access_range<container_t> &&
		std::predicate<compare_t, element_t, element_t> && 
		degree >= 2
	)
	class K_Ary_Heap {
	public:

		using container			= container_t;
		using allocator_type	= container::allocator_type;
		using value_type		= container::value_type;
		using size_type			= container::size_type;
		using difference_type	= container::difference_type;
		using reference			= container::reference;
		using const_reference	= container::const_reference;
		using pointer			= container::pointer;
		using const_pointer		= container::const_pointer;

		// all heap iterators are constant to preserve heap ordering unless
		// specifically a key-value pair that allows modifying the node
		// value without changing the key.
		using iterator					= container::const_iterator;
		using const_iterator			= container::const_iterator;
		using reverse_iterator			= container::const_reverse_iterator;
		using const_reverse_iterator	= container::const_reverse_iterator;

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty heap, and default constructs its internal
		/// array.
		/// </para></summary> --------------------------------------------------
		constexpr K_Ary_Heap() : _container() {}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Container Constructor ~~~
		/// 
		///	<para>
		/// Constructs a heap from a copy of the provided array.
		/// </para></summary> 
		/// 
		/// <param name="c">
		/// The container to copy.
		/// </param> -----------------------------------------------------------
		constexpr K_Ary_Heap(const container& c) : _container(c) {
			heapify(_container);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Container Constructor ~~~
		/// 
		///	<para>
		/// Constructs a heap by moving the provided array.
		/// </para></summary> 
		/// 
		/// <param name="c">
		/// The array to move.
		/// </param> -----------------------------------------------------------
		constexpr K_Ary_Heap(container&& c) : _container(std::move(c)) {
			heapify(_container);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Allocator Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty heap passing the given allocator to the 
		/// heap's internal array.
		/// </para></summary> 
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the internal array.
		/// </param> -----------------------------------------------------------
		constexpr K_Ary_Heap(const allocator_type& alloc) noexcept :
			_container(alloc)
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs a heap with the a copy of the elements in the specified
		/// initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> -----------------------------------------------------------
		constexpr K_Ary_Heap(std::initializer_list<value_type> init) :
			K_Ary_Heap(init.begin(), init.end()) {}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a Heap with the a copy of the elements from the
		/// given iterator/sentinel pair.
		/// </para></summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the beginning iterator to copy from.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The type of the end iterator or sentinel.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the range to copy from.
		/// </param>
		/// <param name="end">
		/// The end of the range to copy from.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> -----------------------------------------------------------
		template <
			std::input_iterator iterator, 
			std::sentinel_for<iterator> sentinel
		>
		constexpr K_Ary_Heap(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : _container(begin, end, alloc) {
			heapify(_container.begin(), _container.end());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a heap with a copy of the elements from the given 
		/// range.
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="tag">
		/// Range construction tag to disabiguate this constructor from
		/// construction with an initializer list.
		/// </param>
		/// <param name="rg">
		/// The range to construct the heap with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> -----------------------------------------------------------
		template <std::ranges::input_range range>
		constexpr K_Ary_Heap(
			from_range_t tag,
			range&& rg,
			const allocator_type& alloc = allocator_type{}
		) : K_Ary_Heap(std::ranges::begin(rg), std::ranges::end(rg), alloc) {
			
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fill Constructor ~~~
		/// 
		/// <para>
		/// Constructs a heap with the specified size, constucting 
		/// elements with the given value for each (or default) and uses the 
		/// given allocator for the internal container (or default allocator).
		/// </para></summary>
		/// 
		/// <param name="size">
		/// The initial size of the heap.
		/// </param>
		/// <param name="value">
		/// The value to initialize every element to. Uses default value for
		/// the type if unspecified.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> -----------------------------------------------------------
		constexpr K_Ary_Heap(
			Size size,
			const_reference value = value_type{},
			const allocator_type& alloc = allocator_type{}
		) : _container(size, value, alloc) {
			// all elements the same therefore heap order is already satisfied.
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the size of the heap.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of elements contained by the heap.
		/// </returns>
		[[nodiscard]] size_type size() const {
			return _container.size();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the theoretical maximum size for the container.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the size limit of the container type.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] size_type max_size() const noexcept {
			return _container.max_size();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the heap of all elements.
		/// </summary> ---------------------------------------------------------
		void clear() {
			_container.clear();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns whether the heap is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the heap has zero elements, false otherwise.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] bool isEmpty() const {
			return _container.isEmpty();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the root of the heap.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the root of the 
		/// heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_reference top() const {
			return _container.front();
		}
		
		// ---------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element into the heap.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> 
		/// 
		/// <result> 
		/// Returns an iterator to the newly inserted element.
		/// </result> ----------------------------------------------------------
		iterator push(const_reference element) {
			_container.insertBack(element);
			return percolateUp(_container.begin(), size() - 1);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the root of the heap.
		/// </summary> ---------------------------------------------------------
		void pop() {
			using std::swap;
			swap(_container.front(), _container.back());
			_container.removeBack();
			percolateDown(_container.begin(), _container.end(), 0);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Changes the priority value of the given heap element by the 
		/// specified amount.
		/// </summary>
		/// 
		/// <param name="pos">
		/// An iterator to the element to increase/decrease.
		/// </param>
		/// 
		/// <param name="value">
		/// The new value of the element.
		/// </param>
		/// 
		/// <result> 
		/// Returns an iterator to the modified element.
		/// </result> ----------------------------------------------------------
		iterator changePriority(const_iterator pos, const_reference value) {
			size_type i = pos - begin();
			_container[i] = value;

			// heap 'iterator' is const - and therefore cannot be swapped
			// when changing heap order. Must be underlying container iterator
			typename container::iterator begin = _container.begin();
			typename container::iterator end = _container.end();

			if (i > 0 && compare(begin + i, begin + parent(i)))
				return percolateUp(begin, i);
			else
				return percolateDown(begin, end, i);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the root element in the 
		/// heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] iterator begin() noexcept {
			return _container.begin();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the position after the 
		/// last leaf element in the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] iterator end() noexcept {
			return _container.end();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the root element in the 
		/// heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_iterator begin() const noexcept {
			return _container.begin();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the heap.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the position after the 
		/// last leaf element in the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_iterator end() const noexcept {
			return _container.end();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the root element in the 
		/// heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return _container.begin();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the heap.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the position after the 
		/// last leaf element in the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return _container.end();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the position after the last 
		/// leaf element in the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] reverse_iterator rbegin() noexcept {
			return std::make_reverse_iterator(end());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the element after the end of 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the element before the 
		/// beginning of the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] reverse_iterator rend() noexcept {
			return std::make_reverse_iterator(begin());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the position after the last 
		/// leaf element in the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept {
			return std::make_reverse_iterator(end());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the element after the end of 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the element before the 
		/// beginning of the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_reverse_iterator rend() const noexcept {
			return std::make_reverse_iterator(begin());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the position after the last 
		/// leaf element in the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept {
			return std::make_reverse_iterator(end());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the element after the end of 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the element before the 
		/// beginning of the heap.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crend() const noexcept {
			return std::make_reverse_iterator(begin());
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given heaps.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first heap to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second heap to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(K_Ary_Heap& a, K_Ary_Heap& b) 
			noexcept(noexcept(swap(_container, _container)))
		{
			swap(a._container, b._container);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The heap appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The heap appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given heap's elements and ordering is equal.
		/// Flase otherwise.
		/// </returns> ---------------------------------------------------------
		friend bool operator==(const K_Ary_Heap& lhs, const K_Ary_Heap& rhs) {
			return lhs._container == rhs._container;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The heap appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The heap appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns the heap's ordering based on contents. 
		/// Always returns less than if the lhs is smaller than rhs.
		/// </returns> ---------------------------------------------------------
		friend auto operator<=>(const K_Ary_Heap& lhs, const K_Ary_Heap& rhs) {
			return lhs._container <=> rhs._container;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Output Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="charT">
		/// The type of the character stream returned by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being written to.
		/// </param>
		/// <param name="arr">
		/// The heap being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after writing.
		/// </returns> ---------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const K_Ary_Heap& heap
		) {
			os << heap._container;
			return os;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Input Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="charT">
		/// The type of the character stream returned by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being read from.
		/// </param>
		/// <param name="arr">
		/// The heap being read into.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> ---------------------------------------------------------
		template <typename charT>
		friend std::basic_istream<charT>& operator>>(
			std::basic_istream<charT>& is,
			K_Ary_Heap& heap
		) {
			is >> heap._container;
			return is;
		}

	private:

		container _container;

		template <std::random_access_iterator in_iterator>
		static constexpr bool compare(in_iterator i1, in_iterator i2) {
			return compare_t{}(*i1, *i2);
		}

		static constexpr size_type parent(size_type index) {
			return (index - 1) / degree;
		}

		template <
			std::random_access_iterator in_iterator,
			std::sentinel_for<iterator> sentinel
		>
		static constexpr bool inBounds(
			in_iterator begin, 
			sentinel end, 
			size_type index
		) {
			return end - (begin + index) > 0;
		}

		template <
			std::random_access_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		static constexpr size_type findMinChild(
			in_iterator begin, 
			sentinel end,
			size_type index
		) {
			size_t result = index;

			for (size_type j = 1; j <= degree; ++j) {
				size_type child = (degree * index) + j;

				if (!inBounds(begin, end, child))
					break;

				if (compare(begin + child, begin + result))
					result = child;
			}

			return result;
		}

		template <
			std::random_access_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		static constexpr in_iterator heapify(in_iterator begin, sentinel end) {
			size_type size = std::distance(begin, end); //TODO replace with collection implementation

			for (size_type i = ((size_t) size - 2) / degree; i >= 0 && i < size; --i) 
				percolateDown(begin, end, i);

			return begin;
		}

		template <
			std::random_access_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		static constexpr in_iterator percolateDown(
			in_iterator begin, 
			sentinel end, 
			size_type index
		) {
			using std::swap;

			while (inBounds(begin, end, index)) {
				size_type next = findMinChild(begin, end, index);

				if (next != index) {
					swap(*(begin + index), *(begin + next));
					index = next;
				}
				else
					break;
			}

			return begin + index;
		}

		template <std::random_access_iterator in_iterator>
		static constexpr in_iterator percolateUp(
			in_iterator begin,
			size_type index
		) {
			using std::swap;

			while (index > 0) {
				size_type parent_index = parent(index);

				if (compare(begin + index, begin + parent_index))
					swap(*(begin + index), *(begin + parent_index));
				else
					break;

				index = parent_index;
			}

			return begin + index;
		}
	};

	//TODO - implement k/v paired heap aliases, map types, etc.

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class container_t = DynamicArray<element_t>
	>
	using BinaryHeap = K_Ary_Heap<element_t, 2, compare_t, container_t>;

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class container_t = DynamicArray<element_t>
	>
	using TernaryHeap = K_Ary_Heap<element_t, 3, compare_t, container_t>;

	static_assert(
		collection<BinaryHeap<int>>,
		"Heap does not meet the requirements for a collection."
	);

	static_assert(
		random_access_iterable<BinaryHeap<int>>,
		"Heap does not meet the requirements for random access iteration."
	);
}
