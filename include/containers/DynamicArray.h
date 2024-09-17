/* =============================================================================
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
 * ========================================================================== */

#pragma once

#include <cstdlib>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <istream>
#include <iterator>
#include <limits>
#include <memory>
#include <ostream>
#include <ranges>
#include <sstream>
#include <type_traits>
#include <utility>

#include "../algorithms/compare.h"
#include "../algorithms/copy.h"
#include "../algorithms/move.h"
#include "../algorithms/shift.h"
#include "../algorithms/stream.h"
#include "../concepts/collection.h"
#include "../concepts/indexable.h"
#include "../concepts/iterable.h"
#include "../concepts/positional.h"
#include "../concepts/sequential.h"
#include "../util/types.h"

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary>
	/// DynamicArray is a collection class that maintains a dynamically sized
	/// contiguous array for fast random access to its elements.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of the elements contained by the array.
	/// </typeparam>
	/// <typeparam name="allocator_t">
	/// The type of the allocator responsible for allocating memory to the
	/// array.
	/// </typeparam> -----------------------------------------------------------
	template <class element_t, class allocator_t = std::allocator<element_t>>
	class DynamicArray final {
	private:

		using alloc_t		= rebind<allocator_t, element_t>;
		using alloc_traits	= std::allocator_traits<alloc_t>;

	public:

		using value_type		= element_t;
		using allocator_type	= allocator_t;
		using reference			= value_type&;
		using const_reference	= const value_type&;
		using size_type			= alloc_traits::size_type;
		using difference_type	= alloc_traits::difference_type;
		using pointer			= alloc_traits::pointer;
		using const_pointer		= alloc_traits::const_pointer;

		using iterator					= pointer;
		using const_iterator			= const_pointer;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty array.
		/// </para></summary> --------------------------------------------------
		constexpr DynamicArray() 
			noexcept(std::is_nothrow_default_constructible_v<allocator_type>) :
			_begin(nullptr),
			_end(nullptr),
			_final(nullptr),
			_allocator()
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Allocator Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty array.
		/// </para></summary> 
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the array.
		/// </param> -----------------------------------------------------------
		constexpr explicit DynamicArray(const allocator_type& alloc) 
			noexcept(std::is_nothrow_copy_constructible_v<allocator_type>) :
			_begin(nullptr),
			_end(nullptr),
			_final(nullptr),
			_allocator(alloc)
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Constructor ~~~
		/// 
		/// <para>
		/// Constructs a deep copy of the specified array.
		/// </para></summary>
		/// 
		/// <param name="copy">
		/// The array to be copied.
		/// </param> -----------------------------------------------------------
		DynamicArray(const DynamicArray& copy) : DynamicArray(
			copy.begin(),
			copy.end(),
			alloc_traits::select_on_container_copy_construction(copy._allocator)) 
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Constructor ~~~
		/// 
		/// <para>
		/// Constructs an array by moving the data from the provided array into
		/// this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The array to be moved into this one.
		/// </param> -----------------------------------------------------------
		DynamicArray(DynamicArray&& other) 
			noexcept(std::is_nothrow_move_constructible_v<allocator_type>) : 
			_begin(std::move(other._begin)),
			_end(std::move(other._end)),
			_final(std::move(other._final)),
			_allocator(std::move(other._allocator))
		{
			other._begin = nullptr;
			other._end = nullptr;
			other._final = nullptr;
		}

		// ---------------------------------------------------------------------
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
		/// allocator if unspecified.
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
		/// <param name="size">
		/// The size of the array.
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
		/// allocator if unspecified.
		/// </param> ----------------------------------------------------------
		DynamicArray(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(init.begin(), init.end(), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray with the a copy of the elements from the
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
		/// The allocator instance used by the array. Default constructs the 
		/// allocator if unspecified.
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
			insert(_end, begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a DynamicArray with a copy of the elements from the
		/// given range.
		/// </para></summary>
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
		/// The range to construct the array with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the array. Default constructs the 
		/// allocator if unspecified.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range range>
		DynamicArray(
			from_range_t tag,
			range&& rg,
			const allocator_type& alloc = allocator_type{}
		) : DynamicArray(std::ranges::begin(rg), std::ranges::end(rg), alloc) {

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
		/// ~~~ Copy Assignment Operator ~~~
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
		DynamicArray& operator=(const DynamicArray& other) {
			static constexpr bool isAlwaysEqual = 
				alloc_traits::is_always_equal::value;
			static constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;
			bool isInstanceEqual = _allocator == other._allocator;

			if (!isAlwaysEqual && !isInstanceEqual && willPropagate) {
				releaseResources();
				_allocator = other._allocator;
			}

			elementWiseCopy(other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Assignment Operator ~~~
		/// 
		/// <para>
		/// Moves the data from the given argument to this array.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The array to move from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the caller with the moved data.
		/// </returns> --------------------------------------------------------
		DynamicArray& operator=(DynamicArray&& other) noexcept(
			alloc_traits::propagate_on_container_move_assignment::value || 
			alloc_traits::is_always_equal::value
		) {
			static constexpr bool isAlwaysEqual =
				alloc_traits::is_always_equal::value;
			static constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_move_assignment::value;
			bool isInstanceEqual = _allocator == other._allocator;

			if (isAlwaysEqual || isInstanceEqual) {
				releaseResources();
				moveMembers(std::move(other));
			} 
			else if (willPropagate) {
				releaseResources();
				_allocator = std::move(other._allocator);
				moveMembers(std::move(other));
			}
			else
				elementWiseCopy(std::move(other));

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
			return _begin[index];
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
			return _begin[index];
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
			return _begin[index];
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
			return _begin[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the internal array managed by the data structure.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a pointer to the internal array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] pointer asRawArray() noexcept {
			return _begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the internal array managed by the data structure.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const pointer to the internal array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_pointer asRawArray() const noexcept {
			return _begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the capacity of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the capacity of the internal array, or total reserved 
		/// space in number of elements it can hold.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_t capacity() const noexcept {
			return _final - _begin;
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
		/// Returns the number of elements contained by the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of valid, constructed elements in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type size() const noexcept {
			return _end - _begin;
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
			return alloc_traits::max_size(_allocator);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the array is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the array contains zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const noexcept {
			return _end == _begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the array of all elements. Removed elements
		/// are destroyed.
		/// </summary> --------------------------------------------------------
		void clear() noexcept {
			destroyElements(_begin, _end);
			_end = _begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Shrinks the array to fit its current contents exactly. No 
		/// additional unconstructed memory will be reserved proceeding the 
		/// call.
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

			if (!_begin) {
				_begin = allocate(capacity);
				_end = _begin;
				_final = _begin + capacity;
			}
			else
				reallocate(capacity);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Allocates and constructs the size of the array for the specified 
		/// number of elements. Throws an exception if memory cannot be 
		/// reserved.
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
			for (size_type i = this->size(); i < size; ++i)
				constructElement(_end++, value);
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
			return _begin;
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
			return _begin;
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
		/// Returns a const_iterator pointing to the beginning of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the first element in 
		/// the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return _begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the end of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the location after the 
		/// last element in the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return _end;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reverse iterator to the element after the end of the
		/// array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reverse_iterator rbegin() noexcept {
			return std::make_reverse_iterator(end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the element after the end of 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reverse iterator to the element before the beginning of
		/// the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reverse_iterator rend() noexcept {
			return std::make_reverse_iterator(begin());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the first element in 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reverse iterator to the element after the end of 
		/// the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept {
			return std::make_reverse_iterator(end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the element after the 
		/// end of the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reverse iterator to the element before the 
		/// beginning of the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator rend() const noexcept {
			return std::make_reverse_iterator(begin());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the first element in 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reverse iterator to the element after the end of 
		/// the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept {
			return std::make_reverse_iterator(end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the element after the 
		/// end of the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reverse iterator to the element before the 
		/// beginning of the array.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crend() const noexcept {
			return std::make_reverse_iterator(begin());
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
			return *_begin;
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
			return *_begin;
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
		/// Inserts a copy of the element at the front of the array.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertFront(const_reference element) {
			insertAt(_begin, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the element at the front of the array.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertFront(value_type&& element) {
			insertAt(_begin, std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element at the back of the array.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertBack(const_reference element) {
			insertAt(_end, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element at the back of the array.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertBack(value_type&& element) {
			insertAt(_end, std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the element into the array at the given index,
		/// and maintains stable order of the existing elements. Throws an 
		/// exception if the index is invalid.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param> 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insert(Index index, const_reference element) {
			return insertAt(index, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the element into the array at the given index,
		/// and maintains stable order of the existing elements. Throws an 
		/// exception if the index is invalid.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param> 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insert(Index index, value_type&& element) {
			return insertAt(index, std::move(element));
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
		/// Const lvalue reference to the element to be inserted.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insert(const_iterator position, const_reference element) {
			return insertAt(const_cast<iterator>(position), element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// In-place inserts the given element into the array before the given 
		/// iterator position, and maintains stable order of the existing 
		/// elements. 
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="element">
		/// Rvalue reference to the element to be inserted in place.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insert(const_iterator position, value_type&& element) {
			return insertAt(const_cast<iterator>(position), std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the array before the given 
		/// iterator position, and maintains stable order of the existing 
		/// elements. 
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="begin">
		/// The beginning iterator of the range to insert.
		/// </param>
		/// <param name="end">
		/// The end iterator of the range to insert.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the first element inserted, or position if
		/// begin == end.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator in_iterator, 
			std::sentinel_for<in_iterator> sentinel
		>
		iterator insert(
			const_iterator position, 
			in_iterator begin, 
			sentinel end
		) {
			return insertAt(const_cast<iterator>(position), begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the array before the given index, and 
		/// maintains stable order of the existing elements. 
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the elements before.
		/// </param>
		/// <param name="begin">
		/// The beginning iterator of the range to insert.
		/// </param>
		/// <param name="end">
		/// The end iterator of the range to insert.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the first element inserted, or the iterator
		/// at index if begin == end.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		iterator insert(Index index, in_iterator begin, sentinel end) {
			size_type i = index.get();
			validateIndexInRange(i);
			return insertAt(_begin + i, begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the array at the given index by
		/// growing the array by one and swapping out the new last element. 
		/// Does not preserve the ordering of the original elements. Throws an 
		/// exception if the index is invalid.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param>
		/// Const lvalue reference to the element to be inserted.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insertUnstable(Index index, const_reference element) {
			size_type i = index.get();
			validateIndexInRange(i);
			return insertUnstable(_begin + i, element);
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
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insertUnstable(
			const_iterator position, 
			const_reference element
		) {
			using std::swap;

			size_type offset = position - _begin;
			ensureCapacity();
			iterator pos = _begin + offset;

			constructElement(_end, element);
			swap(*pos, *_end++);
			return pos;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or array is empty.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to be removed.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the value following the removed element.
		/// </returns> --------------------------------------------------------
		iterator remove(Index index) {
			size_type i = index.get();
			validateIndexExists(i);
			return remove(_begin + i);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given iterator position. Throws an 
		/// exception if the index in invalid or array is empty.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position of the element to be removed.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the position following the removed element.
		/// </returns> --------------------------------------------------------
		iterator remove(const_iterator position) {
			auto offset = position - _begin;
			if (position++ != (_end - 1)) 
				collections::shift(const_cast<iterator>(position), _end, -1);
			destroyElement((_end--) - 1);
			return _begin + offset;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or the array is empty.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to be removed.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the position following the removed element.
		/// </returns> --------------------------------------------------------
		iterator removeUnstable(Index index) {
			size_type i = index.get();
			validateIndexExists(i);
			return removeUnstable(_begin + i);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given iterator position. Throws an 
		/// exception if the array is empty.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to be removed.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the position following the removed element.
		/// </returns> --------------------------------------------------------
		iterator removeUnstable(const_iterator position) {
			using std::swap;
			if (position != _end - 1) 
				swap(*const_cast<iterator>(position), *(_end - 1));
			destroyElement((_end--) - 1);
			return _begin + (position - _begin);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the first element in the array.
		/// </summary> --------------------------------------------------------
		void removeFront() {
			remove(_begin);
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
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the element at index range.end
		/// </returns> --------------------------------------------------------
		iterator remove(IndexRange range) {
			validateIndexExists(range.begin);
			validateIndexInRange(range.end);
			
			if (range.begin <= range.end)
				return remove(_begin + range.begin, _begin + range.end);
			else
				throw std::exception("Begin index is greater than end.");
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements in the given iterator range [begin, end).
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the elemement following end.
		/// </returns> --------------------------------------------------------
		iterator remove(const_iterator begin, const_iterator end) {
			size_type offset = begin - _begin;
			int64_t range_size = end - begin;
			collections::shift(const_cast<iterator>(end), _end, -range_size);
			destroyElements(_end - range_size, _end);
			_end -= range_size;
			return _begin + offset;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place at the front of the array with the
		/// provided arguments.
		/// </summary>
		/// 
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param> ----------------------------------------------------------
		template <class ...Args>
		void emplaceFront(Args&&... args) {
			insertAt(_begin, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place at the back of the array with the
		/// provided arguments.
		/// </summary>
		/// 
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param> ----------------------------------------------------------
		template <class ...Args>
		void emplaceBack(Args&&... args) {
			insertAt(_end, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place at the given index with the provided 
		/// arguments, and maintains stable order of the existing elements. 
		/// Throws an exception if the index in invalid.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param> 
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param> 
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		template <class ...Args>
		iterator emplace(Index index, Args&&... args) {
			return insertAt(index, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the given element in-place with the provided arguments 
		/// before the given iterator position, and maintains stable order of 
		/// the existing elements. 
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="args">
		///The arguments to construct the new element with.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		template <class ...Args>
		iterator emplace(const_iterator position, Args&&... args) {
			return insertAt(
				const_cast<iterator>(position), 
				std::forward<Args>(args)...
			);
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
		friend void swap(DynamicArray& a, DynamicArray& b) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			a.swap(b);
		}

		// ---------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of this DynamicArray with the given array..
		/// </summary>
		/// 
		/// <param name="other">
		/// The container to be swapped with.
		/// </param> -----------------------------------------------------------
		void swap(DynamicArray& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			static constexpr bool isAlwaysEqual = 
				alloc_traits::is_always_equal::value;
			static constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_swap::value;
			bool isInstanceEqual = _allocator == other._allocator;

			if (isAlwaysEqual || isInstanceEqual)
				swapMembers(other);
			else if (willPropagate) {
				using std::swap;
				swap(_allocator, other._allocator);
				swapMembers(other);
			}
			else // Undefined behavior under STL specification
				;
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
		/// Returns true if the given arrays share exact ordering based
		/// on contents. Always returns false for arrays of different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(
			const DynamicArray& lhs,
			const DynamicArray& rhs
		) noexcept requires std::three_way_comparable<value_type> {
			return collections::lexicographic_compare(lhs, rhs);
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
		/// Returns the output stream after writing.
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
		/// The array being written to.
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

		[[no_unique_address, msvc::no_unique_address]] 
		allocator_type _allocator;
		pointer _begin;
		pointer _end;
		pointer _final;

		[[nodiscard]] pointer allocate(size_type size) {
			return alloc_traits::allocate(_allocator, size);
		}

		void deallocate(pointer ptr, size_type size) {
			alloc_traits::deallocate(_allocator, ptr, size);
		}

		template <class ... Args>
		void constructElement(pointer ptr, Args&&... args) {
			alloc_traits::construct(
				_allocator, ptr, std::forward<Args>(args)...);
		}

		void destroyElement(pointer ptr) {
			alloc_traits::destroy(_allocator, ptr);
		}

		void destroyElements(pointer begin, pointer end) {
			while (begin != end)
				destroyElement(begin++);
		}

		void releaseResources() noexcept {
			if (_begin != nullptr) {
				clear();
				deallocate(_begin, capacity());
				_begin = nullptr;
				_final = nullptr;
			}
		}

		void tranferTo(pointer copy, size_type capacity) {
			pointer begin = _begin;
			pointer current = copy;

			while (begin != _end) {
				try {
					constructElement(current++, std::move_if_noexcept(*begin));
					++begin;
				}
				catch (...) {
					destroyElements(copy, current);
					deallocate(copy, capacity);
					throw;
				}
			}
		}

		void reallocate(size_type capacity) {
			size_type size = this->size();

			pointer copy = allocate(capacity);
			tranferTo(copy, capacity);
			clear();
			deallocate(_begin, this->capacity());

			_begin = copy;
			_end = copy + size;
			_final = copy + capacity;
		}

		void elementWiseCopy(const DynamicArray& other) {
			if constexpr (std::is_copy_assignable_v<value_type>) {
				if (size() < other.size()) {
					collections::copy_n(other._begin, size(), _begin);
					insert(_end, other._begin + size(), other._end);
				}
				else {
					iterator pos = collections::copy(other, _begin);
					remove(pos, _end);
				}
			}
			else {
				clear();
				insert(_end, other._begin, other._end);
			}
		}

		void elementWiseCopy(DynamicArray&& other) {
			if constexpr (std::is_move_assignable_v<value_type>) {
				if (size() < other.size()) {
					collections::move_n(other._begin, size(), _begin);
					auto start = std::move_iterator(other._begin + size());
					auto end = std::move_iterator(other._end);
					insert(_end, start, end);
				}
				else {
					iterator pos = collections::move(other, _begin);
					remove(pos, _end);
				}
			}
			else {
				clear();
				auto start = std::move_iterator(other._begin);
				auto end = std::move_iterator(other._end);
				insert(_end, start, end);
			}
		}

		void moveMembers(DynamicArray&& other) noexcept {
			_begin = std::move(other._begin);
			_end = std::move(other._end);
			_final = std::move(other._final);
			other._begin = nullptr;
			other._end = nullptr;
			other._final = nullptr;
		}

		void swapMembers(DynamicArray& other) noexcept {
			using std::swap;
			swap(_begin, other._begin);
			swap(_end, other._end);
			swap(_final, other._final);
		};

		template <class T> 
		iterator insertAt(iterator pos, T&& element) {
			size_type offset = pos - begin();
			ensureCapacity();
			pos = _begin + offset;

			if (pos == _end)
				constructElement(_end++, std::forward<T>(element));
			else {
				constructElement(_end, *(_end - 1));
				collections::shift(pos, _end++ - 1, 1);
				*pos = std::forward<T>(element);
			}
			return pos;
		}

		template <class T>
		iterator insertAt(Index index, T&& element) {
			size_type i = index.get();
			validateIndexInRange(i);
			return insertAt(_begin + i, std::forward<T>(element));
		}

		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		iterator insertAt(iterator position, in_iterator begin, sentinel end) {
			size_type offset = position - _begin;

			while (begin != end) {
				position = insertAt(const_cast<iterator>(position), *begin++);
				position++;
			}
			return begin() + offset;
		}

		template <
			std::forward_iterator fwd_iterator,
			std::sentinel_for<fwd_iterator> sentinel
		>
		iterator insertAt(iterator position, fwd_iterator begin, sentinel end) {
			size_type range_size = std::distance(begin, end);
			size_type total = size() + range_size;

			if (capacity() < total) {
				size_type offset = position - _begin;
				reserve(total);
				position = _begin + offset;
			}

			if (position == _end) 
				constructAtEnd(begin, end);
			else {
				constructAtEnd(_end - range_size, _end);
				collections::shift(position, _end - range_size, range_size);
				collections::copy(begin, end, position);
			}
			return position;
		}

		template <
			std::forward_iterator fwd_iterator,
			std::sentinel_for<fwd_iterator> sentinel
		>
		void constructAtEnd(fwd_iterator begin, sentinel end) {
			while (begin != end)
				constructElement(_end++, *begin++);
		}

		void ensureCapacity() {
			if (_begin == nullptr)
				reserve(8);
			else if (size() >= capacity())
				expand();
		}

		void expand() {
			if (capacity() >= max_size())
				allocationError(ERR_MAX_SIZE);

			size_type doubledSize = size() << 1;
			if (doubledSize < size())
				reserve(max_size());
			else
				reserve(doubledSize);
		}

		void validateCapacity(size_type capacity) {
			if (capacity >  max_size())
				allocationError(ERR_MAX_SIZE);
			if (capacity < size())
				allocationError(ERR_TOO_SMALL);
		}

		void validateIndexExists(size_type index) const {
			[[unlikely]] if (index >= size())
				invalidIndex(index);
		}

		void validateIndexInRange(size_type index) const {
			[[unlikely]] if (index > size())
				invalidIndex(index);
		}

		[[noreturn]] void allocationError(std::string msg) {
			throw std::length_error("Allocation failed: " + msg);
		}

		[[noreturn]] void invalidIndex(size_type index) const {
			constexpr auto INVALID_INDEX = "Invalid Index: out of range.";
			std::stringstream err{};

			err << INVALID_INDEX << std::endl << "Index: " << index
				<< " Size: " << size() << std::endl;
			throw std::out_of_range(err.str().c_str());
		}
	};

	static_assert(
		collection<DynamicArray<int>>,
		"DynamicArray does not meet the requirements for a collection."
	);

	static_assert(
		sequential<DynamicArray<int>>,
		"DynamicArray does not meet the requirements for sequential access."
	);

	static_assert(
		indexable<DynamicArray<int>, size_t>,
		"DynamicArray does not meet the requirements for indexed access."
	);

	static_assert(
		positional<DynamicArray<int>>,
		"DynamicArray does not meet the requirements for positional access."
	);

	static_assert(
		random_access_iterable<DynamicArray<int>>,
		"DynamicArray does not meet the requirements for random access iteration."
	);
}