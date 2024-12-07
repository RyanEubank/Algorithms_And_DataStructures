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

#include <exception>
#include <initializer_list>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <ranges>
#include <sstream>
#include <type_traits>
#include <utility>

#include "Node.h"
#include "../algorithms/copy.h"
#include "../algorithms/compare.h"
#include "../algorithms/move.h"
#include "../algorithms/stream.h"
#include "../concepts/collection.h"
#include "../concepts/indexable.h"
#include "../concepts/iterable.h"
#include "../concepts/positional.h"
#include "../concepts/sequential.h"

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary>
	/// ForwardList is a collection class that maintains a singly linked list 
	/// for fast insertion/removal at the ends with quick forward iteration.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the ForwardList.
	/// </typeparam> 
	/// 
	/// <typeparam name="allocator_t">
	/// The type of the allocator responsible for allocating memory to the 
	/// linked list.
	/// </typeparam> -----------------------------------------------------------
	template <class element_t, class allocator_t = std::allocator<element_t>>
	class ForwardList {
	private:
	
		template <bool isConst>
		class ForwardListIterator;

		template <bool isConst>
		class StableForwardListIterator;

		using alloc_t		= rebind<allocator_t, element_t>;
		using alloc_traits	= std::allocator_traits<alloc_t>;

	public:

		using value_type		= element_t;
		using allocator_type	= allocator_t;
		using node_type			= Node<value_type, allocator_type, 1>;
		using size_type			= alloc_traits::size_type;
		using difference_type	= alloc_traits::difference_type;
		using pointer			= alloc_traits::pointer;
		using const_pointer		= alloc_traits::const_pointer;
		using reference			= value_type&;
		using const_reference	= const value_type&;

		using iterator					= ForwardListIterator<false>;
		using const_iterator			= ForwardListIterator<true>;
		using stable_iterator			= StableForwardListIterator<false>;
		using const_stable_iterator		= StableForwardListIterator<true>;

	private:

		using node_base				= node_type::base;
		using node_allocator_type	= rebind<allocator_t, node_type>;
		using node_alloc_traits		= std::allocator_traits<node_allocator_type>;
		using node_ptr				= node_type::base_ptr;
		using const_node_ptr		= node_type::const_base_ptr;
		using list_node_ptr			= node_alloc_traits::pointer;
		using const_list_node_ptr	= node_alloc_traits::const_pointer;

		constexpr static auto next = 0u;

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ForwardList.
		/// </para></summary> -------------------------------------------------
		constexpr ForwardList()
			noexcept(std::is_nothrow_default_constructible_v<allocator_type>) :
			_sentinel(),
			_tail(&_sentinel),
			_size(),
			_allocator(allocator_type{})
		{
			_sentinel.to(next) = _tail;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Allocator Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ForwardList.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the list.
		/// </param> ----------------------------------------------------------
		constexpr explicit ForwardList(const allocator_type& alloc)
			noexcept(std::is_nothrow_copy_constructible_v<allocator_type>) :
			_sentinel(),
			_tail(&_sentinel),
			_size(),
			_allocator(alloc)
		{
			_sentinel.to(next) = _tail;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		/// <para>
		/// Constructs a deep copy of the specified ForwardList.
		/// </para></summary> 
		/// 
		/// <param name="copy">
		/// The ForwardList to be copied.
		/// </param> ----------------------------------------------------------
		ForwardList(const ForwardList& copy) : ForwardList(
			copy.begin(),
			copy.end(),
			alloc_traits::select_on_container_copy_construction(copy._allocator)
		) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Move Constructor ---
		/// 
		/// <para>
		/// Constructs a ForwardList by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ForwardList to be moved into this one.
		/// </param> ----------------------------------------------------------
		ForwardList(ForwardList&& other) 
			noexcept(std::is_nothrow_move_constructible_v<allocator_type>) : 
			_sentinel(std::move(other._sentinel)),
			_tail(std::move(other._tail)),
			_size(std::move(other._size)),
			_allocator(std::move(other._allocator))
		{
			onMove(std::move(other));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fill Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ForwardList with the specified size, constucting 
		/// elements with the given value for each (or default) and uses the 
		/// given allocator (or default allocator).
		/// </para></summary>
		/// 
		/// <param name="capacity">
		/// The initial capacity for the list.
		/// </param>
		/// <param name="value">
		/// The value to initialize every element to. Uses default value for
		/// the type if unspecified.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the list. Default constructs the 
		/// allocator if unspecified.
		/// </param> ----------------------------------------------------------
		ForwardList(
			Size size,
			const_reference value = value_type{},
			const allocator_type& alloc = allocator_type{}
		) : ForwardList(alloc) {
			for (size_type i = 0; i < size.get(); ++i)
				insertBack(value);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a ForwardList with the a copy of the elements in the 
		/// specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		ForwardList(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : ForwardList(init.begin(), init.end(), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ForwardList with the a copy of the elements from the
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
		/// The allocator instance used by the list. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		ForwardList(
			in_iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : ForwardList(alloc) {
			insert(this->end(), begin, end);	
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ForwardList array with a copy of the elements from the
		/// given range.
		/// </para></summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to construct the list with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the list.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range range>
		ForwardList(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : ForwardList(std::ranges::begin(r), std::ranges::end(r), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the list safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~ForwardList() {
			clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified ForwardList to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ForwardList to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this ForwardList with the copied data.
		/// </returns> --------------------------------------------------------
		ForwardList& operator=(const ForwardList& other) {
			static constexpr bool isAlwaysEqual = 
				alloc_traits::is_always_equal::value;
			static constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;
			bool isInstanceEqual = _allocator == other._allocator;

			if (!isAlwaysEqual && !isInstanceEqual && willPropagate) 
				_allocator = other._allocator;

			elementWiseCopy(other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Moves Assignment Operator ~~~
		/// 
		/// <para>
		/// Moves the data from the specified ForwardList to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ForwardList to move from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this ForwardList with the moved data.
		/// </returns> --------------------------------------------------------
		ForwardList& operator=(ForwardList&& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			static constexpr bool isAlwaysEqual =
				alloc_traits::is_always_equal::value;
			static constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_move_assignment::value;
			bool isInstanceEqual = _allocator == other._allocator;

			if (isAlwaysEqual || isInstanceEqual) {
				clear();
				moveMembers(std::move(other));
			} 
			else if (willPropagate) {
				clear();
				_allocator = std::move(other._allocator);
				moveMembers(std::move(other));
			}
			else
				elementWiseCopy(std::move(other));

			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Index Operator ---
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference operator[](size_type index) {
			return getNodeAt(index)->value();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Index Operator ---
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const reference to the element at the specified index 
		/// in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference operator[](size_type index) const {
			return getNodeAt(index)->value();
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
			return static_cast<allocator_type>(_allocator);
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
			return node_alloc_traits::max_size(_allocator);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the list is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the list has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const noexcept {
			return !(_size);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes and deletes all nodes from the linked list emptying its 
		/// contents.
		/// </summary> --------------------------------------------------------
		void clear() noexcept {
			remove(begin(), end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the number of elements contained by the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of valid, constructed elements in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type size() const noexcept {
			return _size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the first element in the 
		/// list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() noexcept {
			return iterator(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the location after the last 
		/// element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() noexcept {
			return iterator(_tail);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the 
		/// list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element in 
		/// the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const noexcept {
			return const_iterator(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the location after  
		/// the last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const noexcept {
			return const_iterator(_tail);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the beginning of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element in 
		/// the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the location after the 
		/// last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a stable forward iterator to the first element in the 
		/// list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] stable_iterator stable_begin() noexcept {
			return stable_iterator(_sentinel.to(next));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a stable forward iterator to the location after the last 
		/// element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] stable_iterator stable_end() noexcept {
			return stable_iterator(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const stable forward iterator to the first element in the 
		/// list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_stable_iterator stable_begin() const noexcept {
			return const_stable_iterator(_sentinel.to(next));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const stable forward iterator to the location after the 
		/// last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_stable_iterator stable_end() const noexcept {
			return const_stable_iterator(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the first element in the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the first element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference front() {
			return _sentinel.to(next)->value();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the first element in the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reference to the first element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference front() const {
			return _sentinel.to(next)->value();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the last element in the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference back() {
			return _tail->value();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the last element in the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reference to the last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference back() const {
			return _tail->value();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs safe indexing of the list checking the bounds of
		/// the requested index before returning the appropriate element.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference at(size_type index) {
			validateIndexExists(index);
			return (*this)[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs safe indexing of the list checking the bounds of
		/// the requested index before returning the appropriate element.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the specified index 
		/// in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference at(size_type index) const {
			validateIndexExists(index);
			return (*this)[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the front of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> ---------------------------------------------------------
		iterator insertFront(const_reference element) {
			return insertAt(&_sentinel, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the front of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> ---------------------------------------------------------
		iterator insertFront(value_type&& element) {
			return insertAt(&_sentinel, std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the back of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> ---------------------------------------------------------
		iterator insertBack(const_reference element) {
			return insertAt(_tail, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the back of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> ---------------------------------------------------------
		iterator insertBack(value_type&& element) {
			return insertAt(_tail, std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the element into the list at the given index. Throws an 
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
		/// Inserts the element into the list at the given index. Throws an 
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
		/// Inserts the given element into the list before the given iterator 
		/// position.
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
			return insertAt(position.node(), element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the list after the given iterator 
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element after.
		/// </param>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns a stable iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		stable_iterator insertAfter(
			const_stable_iterator position, 
			const_reference element
		) {
			iterator result = insertAt(position.node(), element);
			return stable_iterator((++result)._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the list before the given iterator 
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insert(const_iterator position, value_type&& element) {
			return insertAt(position.node(), std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the list after the given iterator 
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element after.
		/// </param>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns a stable iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		stable_iterator insertAfter(
			const_stable_iterator position, 
			value_type&& element
		) {
			iterator result = insertAt(position.node(), std::move(element));
			return stable_iterator((++result)._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the list before the given iterator 
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the elements before.
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
			if (begin != end) { 
				node_chain result = createChain(begin, end);
				splice(position.node(), result.head, result.tail);
				_size += result.count;
			}

			return iterator(position.node());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the list after the given iterator 
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the elements after.
		/// </param>
		/// <param name="begin">
		/// The beginning iterator of the range to insert.
		/// </param>
		/// <param name="end">
		/// The end iterator of the range to insert.
		/// </param>
		/// 
		/// <returns>
		/// Returns a stable iterator to the first element inserted, or 
		/// position if begin == end.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		stable_iterator insertAfter(
			const_stable_iterator position, 
			in_iterator begin, 
			sentinel end
		) {
			iterator result = insert(const_iterator(position._node), begin, end);
			return stable_iterator((++result)._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the list before the given iterator 
		/// position.
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
			iterator pos(getNodeBefore(i));
			return insert(pos, begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or the list is empty.
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
			node_ptr n = getNodeBefore(i);
			return remove(n);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given iterator position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position of the element to be removed.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the value following the removed element.
		/// </returns> --------------------------------------------------------
		iterator remove(const_iterator position) {
			return remove(position.node());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element after the given iterator position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator to the value preceding the element to be removed.
		/// </param>
		/// 
		/// <returns>
		/// Returns a stable iterator to the value following the removed element.
		/// </returns> --------------------------------------------------------
		stable_iterator removeAfter(const_stable_iterator position) {
			iterator result = remove(position.node());
			return stable_iterator((++result)._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the first element in the list.
		/// </summary> --------------------------------------------------------
		void removeFront() {
			remove(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the last element in the list. Inefficient - equivalent to
		/// iterating through the list and calling remove(position).
		/// </summary> --------------------------s------------------------------
		void removeBack() {
			node_ptr n = &_sentinel;
			while (n->to(next) != _tail)
				n = n->to(next);
			remove(n);
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
				return removeAll(range.begin, range.end);
			else
				throw std::exception("Begin index is greater than end.");
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements in the given iterator range [begin, end).
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the elemement past end.
		/// </returns> --------------------------------------------------------
		iterator remove(const_iterator begin, const_iterator end) {
			return remove(begin.node(), end.node());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place with the provided arguments at the 
		/// front of the list.
		/// </summary>
		/// 
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> ---------------------------------------------------------
		template <class ...Args>
		iterator emplaceFront(Args&&... args) {
			return insertAt(&_sentinel, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place with the provided arguments at the 
		/// back of the list.
		/// </summary>
		/// 
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> ---------------------------------------------------------
		template <class ...Args>
		iterator emplaceBack(Args&&... args) {
			return insertAt(_tail, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place with the provided arguments at the 
		/// given index. Throws an exception if the index in invalid.
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
		/// before the given iterator position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param>
		///
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		template <class ...Args>
		iterator emplace(const_iterator position, Args&&... args) {
			return insertAt(position.node(), std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the given element in-place with the provided arguments
		/// after the given iterator position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element after.
		/// </param>
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param>
		///
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		template <class ...Args>
		stable_iterator emplaceAfter(
			const_stable_iterator position, 
			Args&&... args
		) {
			iterator result = 
				insertAt(position.node(), std::forward<Args>(args)...);
			return stable_iterator((++result)._node);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Splices nodes in the range (begin, end] after the node specified by
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The postion in the list to splice the range after.
		/// </param>
		/// 
		/// <param name="other">
		/// A reference to the other list nodes are being taken from. This can
		/// be equal to this list.
		/// </param>
		/// 
		/// <param name="begin">
		/// An iterator to the position before the range being spliced.
		/// </param>
		/// 
		/// <param name="end">
		/// An iterator to the last position being spliced.
		/// </param> -----------------------------------------------------------
		void splice(
			const_iterator position,
			ForwardList& other,
			const_iterator begin, 
			const_iterator end
		) {
			size_type dist = std::distance(begin, end);
			auto head = begin.node()->to(next);
			other.snip(begin.node(), end.node());
			splice(position.node(), head, end.node());
			other._size -= dist;
			_size += dist;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// 
		/// </summary>
		/// 
		/// <param name="position">
		/// 
		/// </param>
		/// 
		/// <param name="begin">
		/// 
		/// </param>
		/// 
		/// <param name="end">
		/// 
		/// </param> -----------------------------------------------------------
		void spliceAfter(
			const_stable_iterator position,
			ForwardList& other,
			const_stable_iterator begin, 
			const_stable_iterator end
		) {
			size_type dist = std::distance(begin, end);
			auto head = begin.node()->to(next);
			other.snip(begin.node(), end.node());
			splice(position.node(), head, end.node());
			other._size -= dist;
			_size += dist;
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given ForwardLists.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first list to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second list to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(ForwardList& a, ForwardList& b) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			a.swap(b);
		}

		// ---------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of this ForwardList with the given list.
		/// </summary>
		/// 
		/// <param name="other">
		/// The container to be swapped with.
		/// </param> -----------------------------------------------------------
		void swap(ForwardList& other) 
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
		/// The ForwardList appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The ForwardList appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given lists share deep equality based
		/// on contents and size.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const ForwardList& lhs,
			const ForwardList& rhs
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
		/// The ForwardList appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The ForwardList appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given linked lists share exact ordering.
		/// Always returns false for lists of different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(
			const ForwardList& lhs,
			const ForwardList& rhs
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
		/// The list being read from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const ForwardList& list
		) {
			collections::stream(list, os);
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
		/// The list being written to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			ForwardList& list
		) {
			size_type size = 0;
			is >> size;

			list.clear();

			for (size_type i = 0; i < size; ++i) {
				value_type value{};
				is >> value;
				list.insert(list.end(), value);
			}

			return is;
		}

	private:

		[[no_unique_address, msvc::no_unique_address]] //TODO define macro for correct attr depending on compiler
		node_allocator_type _allocator;
		node_base _sentinel;
		node_ptr _tail;
		size_type _size;

		struct node_chain {
			size_type count = 0;
			node_ptr head = nullptr;
			node_ptr tail = nullptr;
		};

		template <class... Args>
		[[nodiscard]] node_ptr createNode(Args&&... args) {
			node_type* n = node_alloc_traits::allocate(_allocator, 1);
			node_alloc_traits::construct(
				_allocator, n, std::in_place_t{}, std::forward<Args>(args)...);
			return n;
		}

		void destroyNode(node_ptr n) {
			node_alloc_traits::destroy(
				_allocator, static_cast<list_node_ptr>(n));
			node_alloc_traits::deallocate(
				_allocator, static_cast<list_node_ptr>(n), 1);
		}


		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		node_chain createChain(in_iterator begin, sentinel end) {
			if (begin == end)
				return { 0, nullptr, nullptr };

			size_type count = 0;
			node_ptr head = nullptr;
			node_ptr tail = nullptr;

			try {
				count = 1;
				head = createNode(*begin++);
				tail = head;

				while (begin != end) {
					node_ptr n = createNode(*begin++);
					tail->to(next) = n;
					tail = n;
					++count;
				}
			}
			catch (...) {
				destroy(head, tail);
				throw;
			}

			return { count, head, tail };
		}

		void fixLinkLoops(ForwardList& a, ForwardList& b) {
			if (a._sentinel.to(next) == &b._sentinel)
				a._sentinel.to(next) = &a._sentinel;
			if (a._tail == &b._sentinel)
				a._tail = &a._sentinel;
			a._tail->to(next) = &a._sentinel;
		}

		void onMove(ForwardList&& other) noexcept {
			fixLinkLoops(*this, other);
			other._sentinel.to(next) = &other._sentinel;
			other._tail = &other._sentinel;
			other._size = 0;
		}

		void moveMembers(ForwardList&& other) noexcept {
			_sentinel = std::move(other._sentinel);
			_tail = std::move(other._tail);
			_size = std::move(other._size);
			onMove(std::move(other));
		}

		void swapMembers(ForwardList& other) noexcept {
			using std::swap;
			swap(_sentinel, other._sentinel);
			swap(_tail, other._tail);
			swap(_size, other._size);
			fixLinkLoops(*this, other);
			fixLinkLoops(other, *this);
		};

		[[nodiscard]] node_ptr getNodeBefore(size_type index) {
			const_node_ptr n = std::as_const(*this).getNodeBefore(index);
			return const_cast<node_ptr>(n);
		}

		[[nodiscard]] const_node_ptr getNodeBefore(size_type index) const {
			if (index == 0)
				return &_sentinel;
			else
				return getNodeAt(index - 1);
		}

		[[nodiscard]] node_ptr getNodeAt(size_type index) {
			const_node_ptr n = std::as_const(*this).getNodeAt(index);
			return const_cast<node_ptr>(n);
		}

		[[nodiscard]] const_node_ptr getNodeAt(size_type index) const {
			const_node_ptr n = _sentinel.to(next);
			for (size_type i = 0; i < index; ++i)
				n = n->to(next);
			return n;
		}

		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		void assignFrom(in_iterator begin, sentinel end) {
			iterator destination = this->begin();
			while (destination != this->end()) 
				*destination++ = *begin++;
			insert(this->end(), begin, end);
		}

		void elementWiseCopy(const ForwardList& other) {
			if constexpr (std::is_copy_assignable_v<value_type>) {
				if (size() < other.size())
					assignFrom(other.begin(), other.end());
				else {
					auto pos = collections::copy(other, begin());
					remove(pos, end());
				}
			}
			else {
				clear();
				insert(end(), other.begin(), other.end());
			}
		}

		void elementWiseCopy(ForwardList&& other) {
			if constexpr (std::is_move_assignable_v<value_type>) {
				if (size() < other.size()) {
					auto start = std::move_iterator(other.begin());
					auto end = std::move_iterator(other.end());
					assignFrom(start, end);
				}
				else {
					auto pos = collections::move(other, begin());
					remove(pos, end());
				}
			}
			else {
				clear();
				auto start = std::move_iterator(other.begin());
				auto end = std::move_iterator(other.end());
				insert(this->end(), start, end);
			}
		}

		size_type destroy(node_ptr begin, node_ptr end) {
			size_type s = 0;
			node_ptr n = nullptr;

			while (begin != end) {
				n = begin->to(next);
				destroyNode(begin);
				begin = n;
				s++;
			}

			return s;
		}

		template <class... Args>
		iterator insertAt(node_ptr location, Args&&... args) {
			node_ptr newNode = createNode(std::forward<Args>(args)...);
			splice(location, newNode, newNode);
			_size++;
			return iterator(location);
		}

		template <class... Args>
		iterator insertAt(Index index, Args&&... args) {
			size_type i = index.get();
			validateIndexInRange(i);
			node_ptr location = getNodeBefore(i);
			return insertAt(location, std::forward<Args>(args)...);
		}

		iterator remove(node_ptr n) {
			return remove(n, n->to(next));
		}

		iterator remove(node_ptr head, node_ptr tail) {
			node_ptr begin = head->to(next); 
			node_ptr end = tail->to(next);
			snip(head, tail);
			_size -= destroy(begin, end);
			return iterator(head);
		}

		void snip(node_ptr head, node_ptr tail) {
			head->to(next) = tail->to(next);

			if (_tail == tail)
				_tail = head;
		}

		void splice(node_ptr position, node_ptr head, node_ptr tail) {
			tail->to(next) = position->to(next); // splices **AFTER** position
			position->to(next) = head;

			if (_tail == position)
				_tail = tail;
		}

		iterator removeAll(size_type begin_index, size_type end_index) {
			node_ptr begin = getNodeBefore(begin_index);
			node_ptr end = begin;

			while (begin_index++ < end_index) 
				end = end->to(next);

			return remove(begin, end);
		}

		void validateIndexExists(size_type index) const {
			if (index >= size())
				throwInvalidIndex(index);
		}

		void validateIndexInRange(size_type index) const {
			if (index > size())
				throwInvalidIndex(index);
		}

		[[noreturn]] void throwInvalidIndex(size_type index) const {
			constexpr auto INVALID_INDEX = "Invalid Index: out of range.";
			std::stringstream err{};

			err << INVALID_INDEX << std::endl << "Index: " << index
				<< " Size: " << size() << std::endl;
			throw std::out_of_range(err.str().c_str());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ForwardListIterator is a class that implements forward iteration 
		/// over a singly linked list.
		/// </summary>
		///
		/// <typeparam name="isConst">
		/// Whether the iterator is a const_iterator (iterates over const 
		/// elements) or not.
		/// </typeparam> -------------------------------------------------------
		template <bool isConst>
		class ForwardListIterator {
		private:
			using pNode = std::conditional_t<isConst, const_node_ptr, node_ptr>;

			pNode _node;

			explicit ForwardListIterator(pNode node) : _node(node) {}

			[[nodiscard]] constexpr node_ptr node() const {
				return const_cast<node_ptr>(_node);
			}

			friend class ForwardList;

		public:

			using value_type = std::conditional_t<isConst, const element_t, element_t>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty ForwardListIterator.
			/// </para></summary> ----------------------------------------------
			ForwardListIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given ForwardListIterator.
			/// </para></summary> ----------------------------------------------
			ForwardListIterator(const ForwardListIterator& copy) = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// Destructs the ForwardListIterator.
			///	</summary> -----------------------------------------------------
			~ForwardListIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The ForwardListIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this ForwardListIterator with the copied data.
			/// </returns> -----------------------------------------------------
			ForwardListIterator& operator=(const ForwardListIterator& other) 
				= default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given ForwardListIterator for implicit 
			/// conversion from non-const version to a const ForwardListIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const ForwardListIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided ForwardListIterator to copy.
			/// </typeparam> ---------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			ForwardListIterator(ForwardListIterator<wasConst> copy)
				: ForwardListIterator(copy._node) {}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the iterator
			/// in its current state. 
			///	</returns> -----------------------------------------------------
			reference operator*() const {
				return _node->to(next)->value();
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Arrow Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a pointer to the element pointed to by the iterator in 
			/// its current state.
			///	</returns> ----------------------------------------------------
			pointer operator->() const {
				return &_node->to(next)->value();
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> -----------------------------------------------------
			ForwardListIterator& operator++() {
				_node = _node->to(next);
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy of
			/// the iterator before updating.
			///	</returns> -----------------------------------------------------
			ForwardListIterator operator++(int) {
				auto copy = *this;
				_node = _node->to(next);
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The ForwardListIterator appearing on the left-hand side of the 
			/// '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The ForwardListIterator appearing on the right-hand side of the 
			/// '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the ForwardListIterators are both pointing to 
			/// the same element, false otherwise.
			///	</returns> -----------------------------------------------------
			friend bool operator==(
				const ForwardListIterator& lhs,
				const ForwardListIterator& rhs
			) {
				return lhs._node == rhs._node;
			}
		};

		// ---------------------------------------------------------------------
		/// <summary>
		/// ForwardListIterator is a class that implements forward iteration 
		/// over a singly linked list.
		/// </summary>
		///
		/// <typeparam name="isConst">
		/// Whether the iterator is a const_iterator (iterates over const 
		/// elements) or not.
		/// </typeparam> -------------------------------------------------------
		template <bool isConst>
		class StableForwardListIterator { //TODO implement explicit conversion constructor from iterators -> stable_iterators and visa-versa
		private:
			using pNode = std::conditional_t<isConst, const_node_ptr, node_ptr>;

			pNode _node;

			explicit StableForwardListIterator(pNode node) : _node(node) {}

			[[nodiscard]] constexpr node_ptr node() const {
				return const_cast<node_ptr>(_node);
			}

			friend class ForwardList;

		public:

			using value_type = std::conditional_t<isConst, const element_t, element_t>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty StableForwardListIterator.
			/// </para></summary> ----------------------------------------------
			StableForwardListIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given StableForwardListIterator.
			/// </para></summary> ----------------------------------------------
			StableForwardListIterator(const StableForwardListIterator& copy) 
				= default;

			// -----------------------------------------------------------------
			/// <summary>
			/// Destructs the StableForwardListIterator.
			///	</summary> -----------------------------------------------------
			~StableForwardListIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The StableForwardListIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this StableForwardListIterator with the copied data.
			/// </returns> -----------------------------------------------------
			StableForwardListIterator& operator=(
				const StableForwardListIterator& other) = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given StableForwardListIterator for 
			/// implicit conversion from non-const version to a const 
			/// StableForwardListIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const StableForwardListIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided StableForwardListIterator to 
			/// copy.
			/// </typeparam> ---------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			StableForwardListIterator(StableForwardListIterator<wasConst> copy)
				: StableForwardListIterator(copy._node) {}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the iterator
			/// in its current state. 
			///	</returns> -----------------------------------------------------
			reference operator*() const {
				return _node->value();
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Arrow Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a pointer to the element pointed to by the iterator in 
			/// its current state.
			///	</returns> ----------------------------------------------------
			pointer operator->() const {
				return &_node->value();
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> -----------------------------------------------------
			StableForwardListIterator& operator++() {
				_node = _node->to(next);
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy of
			/// the iterator before updating.
			///	</returns> -----------------------------------------------------
			StableForwardListIterator operator++(int) {
				auto copy = *this;
				_node = _node->to(next);
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The StableForwardListIterator appearing on the left-hand side of 
			/// the '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The StableForwardListIterator appearing on the right-hand side 
			/// of the '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the StableForwardListIterator are both pointing 
			/// to the same element, false otherwise.
			///	</returns> -----------------------------------------------------
			friend bool operator==(
				const StableForwardListIterator& lhs,
				const StableForwardListIterator& rhs
			) {
				return lhs._node == rhs._node;
			}
		};

		static_assert(
			std::forward_iterator<iterator>,
			"ForwardListIterator is not a valid forward iterator."
		);

		static_assert(
			std::forward_iterator<stable_iterator>,
			"StableForwardListIterator is not a valid forward iterator."
		);
	};

	static_assert(
		collection<ForwardList<int>>,
		"ForwardList does not meet the requirements for a collection."
	);

	static_assert(
		sequential<ForwardList<int>>,
		"ForwardList does not meet the requirements for sequential access."
	);

	static_assert(
		indexable<ForwardList<int>, size_t>,
		"ForwardList does not meet the requirements for indexed access."
	);

	static_assert(
		positional<ForwardList<int>>,
		"ForwardList does not meet the requirements for positional access."
	);

	static_assert(
		forward_iterable<ForwardList<int>>,
		"ForwardList does not meet the requirements for bidirectional iteration."
	);
}