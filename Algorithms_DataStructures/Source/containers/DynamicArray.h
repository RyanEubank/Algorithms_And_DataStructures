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

#include <assert.h>

#include "../algorithms/arrays.h"
#include "../algorithms/collection_algorithms.h"
#include "../concepts/list.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// DynamicArray is a collection class that maintains a dynamically sized
	/// array for fast random access to its elements.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of the elements contained by the array.
	/// </typeparam>
	/// <typeparam name="allocator_t">
	/// The type of the allocator responsible for allocating memory to the 
	/// array.
	/// </typeparam> ----------------------------------------------------------
	template <class element_t, class allocator_t = std::allocator<element_t>>
	class DynamicArray final {
	private:
		using alloc_traits = std::allocator_traits<allocator_t>;

	public:
		using allocator_type = allocator_t;
		using value_type = allocator_type::value_type;
		using size_type = size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = alloc_traits::pointer;
		using const_pointer = alloc_traits::const_pointer;
		using iterator = pointer;
		using const_iterator = const_pointer;

		static constexpr auto DEFAULT_CAPACITY = 0;
		static constexpr auto MAX_CAPACITY = std::numeric_limits<size_type>::max();

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty array.
		/// </para></summary> -------------------------------------------------
		constexpr DynamicArray() 
			noexcept(std::is_nothrow_default_constructible_v<allocator_type>) 
		:
			_array(nullptr),
			_end(nullptr),
			_capacity(DEFAULT_CAPACITY),
			_allocator()
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Allocator Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty array.
		/// </para></summary> 
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the array.
		/// </param> ----------------------------------------------------------
		constexpr explicit DynamicArray(const allocator_type& alloc) noexcept :
			_array(nullptr),
			_end(nullptr),
			_capacity(DEFAULT_CAPACITY),
			_allocator(alloc)
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Constructor ~~~
		/// 
		/// <para>
		/// Constructs a deep copy of the specified DynamicArray, including
		/// maintaining the same capacity and copying allocators.
		/// </para></summary>
		/// 
		/// <param name="copy">
		/// The DynamicArray to be copied.
		/// </param> ----------------------------------------------------------
		DynamicArray(const DynamicArray& copy) : DynamicArray(
			copy.begin(),
			copy.size(),
			alloc_traits::select_on_container_copy_construction(copy._allocator)
		) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The DynamicArray to be moved into this one.
		/// </param> ----------------------------------------------------------
		DynamicArray(DynamicArray&& other) 
			noexcept(std::is_nothrow_move_constructible_v<allocator_type>) :
		DynamicArray(std::move(other._allocator)) {
			swapMembers(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Reserve Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray with the specified capcity and allocator, 
		/// or default allocator if not provided.
		/// </para></summary>
		/// 
		/// <param name="capacity">
		/// The initial capacity for the array.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the array. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> ----------------------------------------------------------
		DynamicArray(
			Reserve capacity,
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(alloc) {
			reserve(capacity.get());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fill Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray with the specified size, constucting 
		/// elements with the given value for each (or default) and uses the 
		/// given allocator (or default allocator).
		/// </para></summary>
		/// 
		/// <param name="capacity">
		/// The initial capacity for the array.
		/// </param>
		/// <param name="value">
		/// The value to initialize every element to. Uses default value for
		/// the type if unspecified.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the array. Default constructs the 
		/// allocator if unspecified.
		/// </param> ----------------------------------------------------------
		DynamicArray(
			Size size,
			const_reference value = value_type{},
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(alloc) {
			resize(size.get(), value);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Initialization Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray with the a copy of the elements in the 
		/// specified initialization list.
		/// >/para></summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the array. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> ----------------------------------------------------------
		DynamicArray(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(init.begin(), init.size(), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray with the a copy of the elements from the
		/// given iterator pair.
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
		/// The beginning of the iterator pair to copy from.
		/// </param>
		/// <param name="end">
		/// The end of the iterator pair to copy from.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the array. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator iterator, 
			std::sentinel_for<iterator> sentinel
		>
		DynamicArray(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(alloc) {
			if constexpr (std::forward_iterator<iterator>)
				reserve(std::distance(begin, end));
			insert(end(), begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a Dynamic array with a copy of the elements from the
		/// given range.
		/// </para></summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to construct the array with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the array.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range range>
		DynamicArray(
			from_range_t tag,
			range&& rg,
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(alloc) {
			if constexpr (std::ranges::forward_range<range>)
				reserve(std::ranges::size(rg));
			insert(end(), std::ranges::begin(rg), std::ranges::end(rg));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the array safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~DynamicArray() {
			releaseResources();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Assignment Operator ~~~
		/// 
		/// <para>
		/// Copies the data from the given argument to this array.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The array to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the caller with the copied data.
		/// </returns> --------------------------------------------------------
		DynamicArray& operator=(const DynamicArray& other) noexcept {
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;

			if (propagate && this->_allocator != other._allocator) {
				releaseResources();
				_allocator = other._allocator;
				insert(end(), other.begin(), other.end());
			}
			else if (size() < other.size()) {
				auto start = other.begin();
				auto mid = std::next(other.begin(), this->size());
				auto end = other.end();
				collections::copy(start, mid, begin());
				insert(end(), mid, end);
			}
			else {
				collections::copy(other.begin(), other.end(), begin());
				auto newEnd = std::next(begin(), other.size());
				remove(newEnd, end());
			}
			
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Assignment Operator ~~~
		/// 
		/// <para>
		/// Copies the data from the given argument to this array.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The array to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the caller with the copied data.
		/// </returns> --------------------------------------------------------
		DynamicArray& operator=(DynamicArray&& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			constexpr bool propagate =
				alloc_traits::propagate_on_container_move_assignment::value;
			constexpr bool alwaysEqual = alloc_traits::is_always_equal::value;

			if (alwaysEqual || this->_allocator == other._allocator)
				swapMembers(*this, other);
			else if (propagate) {
				using std::swap;
				swap(_allocator, other._allocator);
				swapMembers(*this, other);
			}
			else if (size() < other.size()) {
				size_type size_diff = other.size() - size();
				collections::move(other._array, &other._array[size()], _array);
				moveAppendFrom(&other._array[size()], size_diff);
			}
			else {
				collections::move(other, _array);
				removeAll(&_array[other.size()], end());
			}
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Index Operator ~~~
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference operator[](size_type index) {
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Index Operator ~~~
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const reference to the element at the specified index 
		/// in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference operator[](size_type index) const {
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs safe indexing of the array checking the bounds of
		/// the requested index before returning the appropriate element.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference at(size_type index) {
			validateIndexExists(index);
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs safe indexing of the array checking the bounds of
		/// the requested index before returning the appropriate element.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the specified index 
		/// in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference at(size_type index) const {
			validateIndexExists(index);
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the internal array managed by the data structure.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the internal array as a pointer.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] pointer asRawArray() noexcept {
			return _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the internal array managed by the data structure.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the internal array as a constant pointer.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_pointer asRawArray() const noexcept {
			return _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the capacity of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the value of the array's internal capacity.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_t capacity() const noexcept {
			return _capacity;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the allocator managing memory for the container.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the allocator managing memory for the 
		/// container.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] allocator_type allocator() const noexcept {
			return _allocator;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the array is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the array has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const noexcept {
			return _end == _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the array of all elements. Removed elements
		/// are destroyed.
		/// </summary> --------------------------------------------------------
		void clear() noexcept {
			destroyElements(_array, _end);
			_end = _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the number of elements contained by the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of valid, constructed elements in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type size() const noexcept {
			return _end - _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Shrinks the array to fit its current contents.
		/// </summary> --------------------------------------------------------
		void trim() noexcept {
			reserve(size());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Allocates enough space in the array to hold the specified
		/// number of elements. Throws an exception if memory cannot be 
		/// reserved.
		/// </summary>
		/// 
		/// <param name="capacity">
		/// The number of elements to reserve space for.
		/// </param> ----------------------------------------------------------
		void reserve(size_type capacity) {
			validateCapacity(capacity);

			if (_array == nullptr) {
				_array = allocate(capacity);
				_end = _array;
				_capacity = capacity;
			}
			else
				reallocate(capacity);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Sets the size of the array to the specified number of
		/// elements. Throws an exception if memory cannot be reserved.
		/// </summary>
		/// 
		/// <param name="size">
		/// The number of elements to reserve space for.
		/// </param>
		/// <param name="value">
		/// The value to construct new elements with.
		/// </value> ----------------------------------------------------------
		void resize(size_type size, const_reference value = value_type{}) {
			reserve(size);
			constructElements(_end, _array + size, value);
			_end = _array + size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a random access iterator to the first element in the 
		/// array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() noexcept {
			return _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a random access iterator to the location after the last 
		/// element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() noexcept {
			return _end;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the first element in 
		/// the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const noexcept {
			return _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the location after the 
		/// last element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const noexcept {
			return _end;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the first element in the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the first element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference front() {
			return *_array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the first element in the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reference to the first element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference front() const {
			return *_array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the last element in the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the last element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference back() {
			return *(_end - 1);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the last element in the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reference to the last element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference back() const {
			return *(_end - 1);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the element into the array at the given index,
		/// and maintains stable order of the existing elements. Throws an 
		/// exception if the index in invalid.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param> 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insert(const Index& index, const_reference element) {
			size_type i = index.get();
			validateIndexInRange(i);
			insert(_array + i, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the array before the given iterator 
		/// position, and maintains stable order of the existing elements. 
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insert(iterator position, const_reference element) {
			size_type offset = position - _array;
			ensureCapacity();
			position = _array + offset;

			if (position == _end)
				constructElement(position, element);
			else {
				constructElement(_end);
				collections::shiftArray(position, _end, 1);
				*position = element;
			}

			++_end;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the array at the given index by
		/// growing the array by one and swapping out the new last element. 
		/// Does not preserve the ordering of the original elements. Throws an 
		/// exception if the index in invalid.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param>
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertUnstable(const Index& index, const_reference element) {
			size_type i = index.get();
			validateIndexInRange(i);
			insertUnstable(_array + i, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the array before the given iterator 
		/// position by growing the array by one and swapping out the new last 
		/// element. Does not preserve the ordering of the original elements.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertUnstable(iterator position, const_reference element) {
			using std::swap;

			size_type offset = position - _array;
			ensureCapacity();
			position = _array + offset;

			constructElement(_end, element);
			swap(*position, *_end++);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the element at the front of the array.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertFront(const_reference element) {
			insert(_array, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element at the back of the array.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertBack(const_reference element) {
			insert(_end, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or array is empty.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to be removed.
		/// </param> ----------------------------------------------------------
		void remove(Index index) {
			size_type i = index.get();
			validateIndexExists(i);
			remove(_array + i);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given iterator position. Throws an 
		/// exception if the index in invalid or array is empty.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position of the element to be removed.
		/// </param> ----------------------------------------------------------
		void remove(iterator position) {
			if (position != (_end - 1))
				collections::shiftArray(position + 1, _end, -1);
			destroyElement((_end--) - 1);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or the array is empty.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to be removed.
		/// </param> ----------------------------------------------------------
		void removeUnstable(Index index) {
			size_type i = index.get();
			validateIndexExists(i);
			removeUnstable(_array + i);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given iterator position. Throws an 
		/// exception if the array is empty.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to be removed.
		/// </param> ----------------------------------------------------------
		void removeUnstable(iterator position) {
			using std::swap;
			if (position != _end - 1)
				swap(*position, *(_end - 1));
			destroyElement((_end--) - 1);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the first element in the array.
		/// </summary> --------------------------------------------------------
		void removeFront() {
			remove(_array);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the last element in the array.
		/// </summary> --------------------------------------------------------
		void removeBack() {
			remove(_end - 1);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements between the given indices [begin, end).
		/// </summary>
		/// 
		/// <param name="range">
		/// An index range with a start and end member.
		/// </param> ----------------------------------------------------------
		void removeAll(const IndexRange& range) {
			validateIndexExists(range.begin);
			validateIndexInRange(range.end);
			
			if (range.begin < range.end)
				removeAll(_array + range.begin, _array + range.end);
			else
				throw std::exception("Begin index is greater than end.");
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements in the given iterator range [begin, end).
		/// </summary> --------------------------------------------------------
		void removeAll(iterator begin, iterator end) {
			int64_t range_size = end - begin;
			collections::shiftArray(end, _end, -range_size);
			destroyElements(_end - range_size, _end);
			_end -= range_size;
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given DynamicArrays.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first array to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second array to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(DynamicArray& a, DynamicArray& b) noexcept {
			swapMembers(a, b);

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The DynamicArray appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The DynamicArray appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given arrays share deep equality based
		/// on contents and size.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const DynamicArray& lhs,
			const DynamicArray& rhs
		) noexcept {
			bool isSizeEqual = (lhs.size() == rhs.size());
			if (isSizeEqual)
				return collections::lexicographic_compare(lhs, rhs) == 0;
			return false;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Comparison Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The DynamicArray appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The DynamicArray appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given arrays share ordering based
		/// on contents and size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(
			const DynamicArray& lhs,
			const DynamicArray& rhs
		) noexcept requires std::three_way_comparable<value_type> {

			auto compareSize = lhs.size() <=> rhs.size();
			if (compareSize == 0)
				return collections::lexicographic_compare(lhs, rhs);
			return static_cast<decltype(value_type{} <=> value_type{})> (compareSize);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Input Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The type of the character stream read by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being written to.
		/// </param>
		/// <param name="arr">
		/// The array being read from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream written with the given array.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const DynamicArray& arr
		) {
			collections::stream(arr, os);
			return os;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Input Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The type of the character stream read by the operator.
		/// </typeparam>
		/// 
		/// <param name="is">
		/// The stream being read from.
		/// </param>
		/// <param name="arr">
		/// The BaseArray being written to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			DynamicArray& arr
		) {
			size_type size = 0;
			is >> size;

			arr.clear();
			arr.resize(size);

			for (size_type i = 0; i < size; ++i)
				is >> arr[i];

			return is;
		}

	private:

		static constexpr auto ERR_MAX_SIZE = "Capacity exceeds maximum size.\n";
		static constexpr auto ERR_TOO_SMALL = "Capacity too small for contents.\n";

		pointer _array;
		pointer _end;
		allocator_type _allocator;
		size_type _capacity;

		[[nodiscard]] pointer allocate(size_type size) {
			return alloc_traits::allocate(_allocator, size);
		}

		void deallocate(pointer ptr, size_type size) {
			alloc_traits::deallocate(_allocator, ptr, size);
		}

		template <class ... Args>
		void constructElement(pointer ptr, Args&&... args) {
			alloc_traits::construct(
				_allocator, 
				ptr, 
				std::forward<Args>(args)...
			);
		}

		void destroyElement(pointer ptr) {
			alloc_traits::destroy(_allocator, ptr);
		}

		void releaseResources() {
			if (_array != nullptr) {
				clear();
				deallocate(_array, _capacity);
				_array = nullptr;
				_capacity = 0;
			}
		}

		template <std::input_iterator iterator>
		void copyConstruct(
			pointer memstart,
			size_type capacity,
			pointer destination,
			iterator element
		) {
			try {
				constructElement(destination, *element);
			}
			catch (...) {
				destroyElements(memstart, destination);
				deallocate(memstart, capacity);
				throw;
			}
		}

		void copyTo(pointer destination, size_type capacity) {
			pointer begin = _array, end = _end, offset = destination;
			while (begin != end) 
				copyConstruct(destination, capacity, offset++, begin++);
		}

		void moveTo(pointer destination) {
			pointer begin = _array, end = _end;
			while (begin != end) {
				constructElement(destination, std::move(*begin));
				++begin;
				++destination;
			}
		}

		void tranferElementsTo(pointer destination, size_type capacity) {
			assert(size() <= capacity);

			if constexpr (std::is_nothrow_move_constructible_v<value_type>)
				moveTo(destination);
			else 
				copyTo(destination, capacity);
		}

		void reallocate(size_type capacity) {
			pointer copy = allocate(capacity);
			tranferElementsTo(copy, capacity);

			size_type size = this->size();
			destroyElements(_array, _end);
			deallocate(_array, _capacity);

			_array = copy;
			_end = copy + size;
			_capacity = capacity;
		}

		//template <class ... Args>
		//void constructElements(pointer begin, pointer end, Args&&... args) {
		//	for ( ; begin != end; ++begin)
		//		constructElement(begin, std::forward<Args>(args)...);
		//}

		//void destroyElements(pointer begin, pointer end) {
		//	for ( ; begin != end; ++begin)
		//		destroyElement(begin);
		//}
		// 
		//template <std::input_iterator iterator>
		//void copyAppendFrom(iterator it, size_type size) {
		//	if (_capacity < this->size() + size)
		//		reserve(this->size() + size);
		//	for (size_t i = 0; i < size; ++i)
		//		tryCopyElement(_array, _capacity, _end++, it++);
		//}

		//template <std::input_iterator iterator>
		//void moveAppendFrom(iterator it, size_type size) {
		//	if (_capacity < this->size() + size)
		//		reserve(this->size() + size);
		//	for (size_t i = 0; i < size; ++i)
		//		constructElement(_end++, std::move(*it++));
		//}

		void ensureCapacity() {
			if (_array == nullptr)
				reserve(1);
			if (size() >= _capacity)
				expand();
		}

		void expand() {
			if (_capacity >= MAX_CAPACITY) 
				throwAllocationError(ERR_MAX_SIZE);

			size_type doubledSize = size() << 1;
			if (doubledSize < size())
				reserve(MAX_CAPACITY);
			else
				reserve(doubledSize);
		}

		void validateCapacity(size_type capacity) {
			if (capacity > _capacity && capacity > MAX_CAPACITY) 
				throwAllocationError(ERR_MAX_SIZE);
			else if (capacity < size())
				throwAllocationError(ERR_TOO_SMALL);
		}

		void validateIndexExists(size_type index) const {
			if (index >= size())
				throwInvalidIndex(index);
		}

		void validateIndexInRange(size_type index) const {
			if (index > size())
				throwInvalidIndex(index);
		}

		[[noreturn]] void throwAllocationError(std::string msg) {
			throw std::length_error("Allocation failed: " + msg);
		}

		[[noreturn]] void throwInvalidIndex(size_t index) const {
			constexpr auto INVALID_INDEX = "Invalid Index: out of range.";
			std::stringstream err{};

			err << INVALID_INDEX << std::endl << "Index: " << index
				<< " Size: " << size() << std::endl;
			throw std::out_of_range(err.str().c_str());
		}

		friend void swapMembers(DynamicArray& a, DynamicArray& b) noexcept {
			using std::swap;
			swap(a._array, b._array);
			swap(a._end, b._end);
			swap(a._capacity, b._capacity);
		};
	};

	static_assert(
		list<DynamicArray<int>>,
		"DynamicArray does not implement the list interface."
	);
}