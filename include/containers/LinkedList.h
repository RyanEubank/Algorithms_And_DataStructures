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

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary>
	/// LinkedList is a collection class that maintains a doubly linked list 
	/// for fast insertion/removal at the ends.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the LinkedList.
	/// </typeparam> 
	/// 
	/// <typeparam name="allocator_t">
	/// The type of the allocator responsible for allocating memory to the 
	/// linked list.
	/// </typeparam> -----------------------------------------------------------
	template <class element_t, class allocator_t = std::allocator<element_t>>
	class LinkedList final {
	private:

		template <bool isConst>
		class LinkedListIterator;

		using alloc_t		= rebind<allocator_t, element_t>;
		using alloc_traits	= std::allocator_traits<alloc_t>;

	public:

		using value_type		= element_t;
		using allocator_type	= allocator_t;
		using node_type			= Node<value_type, allocator_type, 2>;
		using size_type			= alloc_traits::size_type;
		using difference_type	= alloc_traits::difference_type;
		using pointer			= alloc_traits::pointer;
		using const_pointer		= alloc_traits::const_pointer;
		using reference			= value_type&;
		using const_reference	= const value_type&;

		using iterator					= LinkedListIterator<false>;
		using const_iterator			= LinkedListIterator<true>;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

	private:

		using node_base				= node_type::base;
		using node_allocator_type	= rebind<allocator_t, node_type>;
		using node_alloc_traits		= std::allocator_traits<node_allocator_type>;
		using node_ptr				= node_type::pointer;
		using const_node_ptr		= node_type::const_pointer;

		constexpr static auto prev = 0u;
		constexpr static auto next = 1u;

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty LinkedList.
		/// </para></summary> -------------------------------------------------
		constexpr LinkedList()
			noexcept(std::is_nothrow_default_constructible_v<allocator_type>) :
			_sentinel(),
			_size(),
			_allocator(allocator_type{})
		{
			_sentinel.to(next) = &_sentinel;
			_sentinel.to(prev) = &_sentinel;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Allocator Constructor ---
		/// 
		///	<para>
		/// Constructs an empty LinkedList.
		/// </para></summary>
		/// <param name="alloc">
		/// The allocator instance used by the list.
		/// </param> ----------------------------------------------------------
		constexpr explicit LinkedList(const allocator_type& alloc)
			noexcept(std::is_nothrow_copy_constructible_v<allocator_type>) :
			_sentinel(),
			_size(),
			_allocator(alloc)
		{
			_sentinel.to(next) = &_sentinel;
			_sentinel.to(prev) = &_sentinel;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		/// <para>
		/// Constructs a deep copy of the specified LinkedList.
		/// </para></summary> 
		/// 
		/// <param name="copy">
		/// The LinkedList to be copied.
		/// </param> ----------------------------------------------------------
		LinkedList(const LinkedList& copy) : LinkedList(
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
		/// Constructs a LinkedList by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The LinkedList to be moved into this one.
		/// </param> ----------------------------------------------------------
		LinkedList(LinkedList&& other) 
			noexcept(std::is_nothrow_move_constructible_v<allocator_type>) : 
			_sentinel(std::move(other._sentinel)),
			_size(std::move(other._size)),
			_allocator(std::move(other._allocator))
		{
			exchange(_sentinel, other._sentinel);
			other._sentinel.to(prev) = &other._sentinel;
			other._sentinel.to(next) = &other._sentinel;
			other._size = 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fill Constructor ~~~
		/// 
		/// <para>
		/// Constructs a LinkedList with the specified size, constucting 
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
		LinkedList(
			Size size,
			const_reference value = value_type{},
			const allocator_type& alloc = allocator_type{}
		) : LinkedList(alloc) {
			for (size_type i = 0; i < size.get(); ++i)
				insertBack(value);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a LinkedList with the a copy of the elements in the 
		/// specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		LinkedList(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : LinkedList(init.begin(), init.end(), alloc) {
		
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a LinkedList with the a copy of the elements from the
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
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel
		>
		LinkedList(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : LinkedList(alloc) {
			insert(this->end(), begin, end);	
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a LinkedList array with a copy of the elements from the
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
		LinkedList(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : LinkedList(std::ranges::begin(r), std::ranges::end(r), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the list safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~LinkedList() {
			clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified LinkedList to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The LinkedList to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this LinkedList with the copied data.
		/// </returns> --------------------------------------------------------
		LinkedList& operator=(const LinkedList& other) {
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
		/// Moves the data from the specified LinkedList to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The LinkedList to move from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this LinkedList with the moved data.
		/// </returns> --------------------------------------------------------
		LinkedList& operator=(LinkedList&& other) 
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
		/// LinkedList.
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
		/// in the LinkedList.
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
		/// Returns a bidirectional iterator to the first element in the 
		/// list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() noexcept {
			return iterator(_sentinel.to(next));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a bidirectional iterator to the location after the last 
		/// element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() noexcept {
			return iterator(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the 
		/// list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant bidirectional iterator to the first element in 
		/// the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const noexcept {
			return const_iterator(_sentinel.to(next));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant bidirectional iterator to the location after  
		/// the last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const noexcept {
			return const_iterator(&_sentinel);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the beginning of the list.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the first element in 
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
		/// Returns a constant random access iterator to the location after the 
		/// last element in the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reverse iterator to the element after the last element in
		/// the list.
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
		/// the list.
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
		/// the list.
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
		/// beginning of the list.
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
		/// the list.
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
		/// beginning of the list.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crend() const noexcept {
			return std::make_reverse_iterator(begin());
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
			return _sentinel.to(prev)->value();
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
			return _sentinel.to(prev)->value();
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
		/// </param> ----------------------------------------------------------
		void insertFront(const_reference element) {
			insertAt(_sentinel.to(next), element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the front of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertFront(value_type&& element) {
			insertAt(_sentinel.to(next), std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the back of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertBack(const_reference element) {
			insertAt(&_sentinel, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts a copy of the given element at the back of the list.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param> ----------------------------------------------------------
		void insertBack(value_type&& element) {
			insertAt(&_sentinel, std::move(element));
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
		/// Inserts the given element into the list before the given iterator 
		/// position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position to insert the element before.
		/// </param>
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		iterator insert(const_iterator position, value_type&& element) {
			return insertAt(position.node(), element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the list before the given iterator 
		/// position.
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
			if (begin != end) { // TODO: exception safety, clean up if error occurs before splicing
				size_type size = 1;
				node_ptr head = createNode(*begin++);
				node_ptr tail = head;

				while (begin != end) {
					node_ptr n = createNode(*begin++);
					tail->to(next) = n;
					n->to(prev) = tail;
					tail = tail->to(next);
					size++;
				}

				splice(position.node(), head, tail);
				_size += size;
				return iterator(head);
			}

			return iterator(position.node());
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
			iterator pos(getNodeAt(i));
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
			auto n = getNodeAt(i);
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
		/// Removes the first element in the list.
		/// </summary> --------------------------------------------------------
		void removeFront() {
			remove(_sentinel.to(next));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the last element in the list.
		/// </summary> --------------------------s------------------------------
		void removeBack() {
			remove(_sentinel.to(prev));
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

			if (range.begin < range.end)
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
		/// </param> ----------------------------------------------------------
		template <class ...Args>
		void emplaceFront(Args&&... args) {
			insertAt(_sentinel.to(next), std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the element in-place with the provided arguments at the 
		/// back of the list.
		/// </summary>
		/// 
		/// <param name="args">
		/// The arguments to construct the new element with.
		/// </param> ----------------------------------------------------------
		template <class ...Args>
		void emplaceBack(Args&&... args) {
			insertAt(&_sentinel, std::forward<Args>(args)...);
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
		///The arguments to construct the new element with.
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
		/// Swaps the contents of the given LinkedLists.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first list to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second list to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(LinkedList& a, LinkedList& b) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			a.swap(b);
		}

		// ---------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of this LinkedList with the given list.
		/// </summary>
		/// 
		/// <param name="other">
		/// The container to be swapped with.
		/// </param> -----------------------------------------------------------
		void swap(LinkedList& other) 
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
		/// The LinkedList appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The LinkedList appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given lists share deep equality based
		/// on contents and size.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const LinkedList& lhs,
			const LinkedList& rhs
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
		/// The LinkedList appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The LinkedList appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given linked lists share exact ordering.
		/// Always returns false for lists of different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(
			const LinkedList& lhs,
			const LinkedList& rhs
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
		/// The linked list being read from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const LinkedList& list
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
		/// The linked list being written to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			LinkedList& list
		) {
			size_type size = 0;
			is >> size;

			list.clear();

			for (size_type i = 0; i < size; ++i) {
				value_type value;
				is >> value;
				list.insert(list.end(), value);
			}

			return is;
		}

	private:

		[[no_unique_address, msvc::no_unique_address]] //TODO define macro for correct attr depending on compiler
		node_allocator_type _allocator;
		node_base _sentinel;
		size_type _size;

		template <class... Args>
		[[nodiscard]] node_ptr createNode(Args&&... args) {
			node_type* n = node_alloc_traits::allocate(_allocator, 1);
			node_alloc_traits::construct(
				_allocator, n, std::in_place_t{}, std::forward<Args>(args)...);
			return n;
		}

		void destroyNode(node_ptr n) {
			node_alloc_traits::destroy(_allocator, static_cast<node_type*>(n));
			node_alloc_traits::deallocate(_allocator, static_cast<node_type*>(n), 1);
		}

		void exchange(node_base& a, const node_base& b) {
			if (a.to(next) == &b)
				a.to(next) = &a;
			else
				a.to(next)->to(prev) = &a;

			if (a.to(prev) == &b)
				a.to(prev) = &a;
			else
				a.to(prev)->to(next) = &a;
		}

		void moveMembers(LinkedList&& other) noexcept {
			_sentinel = std::move(other._sentinel);
			_size = std::move(other._size);
			exchange(_sentinel, other._sentinel);
			other._sentinel.to(prev) = &other._sentinel;
			other._sentinel.to(next) = &other._sentinel;
			other._size = 0;
		}

		void swapMembers(LinkedList& other) noexcept {
			using std::swap;
			swap(_sentinel, other._sentinel);
			exchange(_sentinel, other._sentinel);
			exchange(other._sentinel, _sentinel);
			swap(_size, other._size);
		};

		[[nodiscard]] node_ptr getNodeAt(size_type index) {
			const_node_ptr n = std::as_const(*this).getNodeAt(index);
			return const_cast<node_ptr>(n);
		}

		[[nodiscard]] const_node_ptr getNodeAt(size_type index) const {
			size_type half_size = _size >> 1;

			if (index <= half_size)
				return traverseForwardFrom(_sentinel.to(next), index);
			else
				return traverseBackwardFrom(&_sentinel, _size - index);
		}

		[[nodiscard]] const_node_ptr traverseForwardFrom(
			const_node_ptr n, 
			size_type numSteps
		) const {
			for (size_type i = 0; i < numSteps; ++i)
				n = n->to(next);
			return static_cast<const_node_ptr>(n);
		}

		[[nodiscard]] const_node_ptr traverseBackwardFrom(
			const_node_ptr n, 
			size_type numSteps
		) const {
			for (size_type i = 0; i < numSteps; ++i)
				n = n->to(prev);
			return static_cast<const_node_ptr>(n);
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

		void elementWiseCopy(const LinkedList& other) {
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

		void elementWiseCopy(LinkedList&& other) {
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
				insert(end(), start, end);
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
			return iterator(newNode);
		}

		template <class... Args>
		iterator insertAt(Index index, Args&&... args) {
			size_type i = index.get();
			validateIndexInRange(i);
			node_ptr location = getNodeAt(i);
			return insertAt(location, std::forward<Args>(args)...);
		}

		iterator remove(node_ptr n) {
			return remove(n, n->to(next));
		}

		iterator remove(node_ptr head, node_ptr tail) {
			snip(head, tail);
			_size -= destroy(head, tail);
			return iterator(tail);
		}

		void splice(node_ptr position, node_ptr head, node_ptr tail) {
			head->to(prev) = position->to(prev);
			position->to(prev)->to(next) = head;
			tail->to(next) = position;
			position->to(prev) = tail;
		}

		void snip(node_ptr head, node_ptr tail) {
			head->to(prev)->to(next) = tail;
			tail->to(prev) = head->to(prev);
		}

		iterator removeAll(size_type begin_index, size_type end_index) {
			node_ptr begin = getNodeAt(begin_index);
			node_ptr n = nullptr;

			while (begin_index++ < end_index) {
				n = begin->to(next);
				remove(begin, n);
				begin = n;
			}

			return iterator(n);
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
		/// LinkedListIterator is a class that implements bidirectional 
		/// iteration over a doubly linked list.
		/// </summary>
		///
		/// <typeparam name="element_t">
		/// The type of the elements iterated over by the LinkedListIterator.
		/// </typeparam> -------------------------------------------------------
		template <bool isConst>
		class LinkedListIterator {
		private:
			using pNode = std::conditional_t<isConst, const_node_ptr, node_ptr>;

			pNode _node;

			explicit LinkedListIterator(pNode node) : _node(node) {}

			[[nodiscard]] constexpr node_ptr node() const {
				return const_cast<node_ptr>(_node);
			}

			friend class LinkedList;

		public:

			using value_type = std::conditional_t<isConst, const element_t, element_t>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::bidirectional_iterator_tag;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty LinkedListIterator.
			/// </para></summary> ----------------------------------------------
			LinkedListIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given LinkedListIterator.
			/// </para></summary> ----------------------------------------------
			LinkedListIterator(const LinkedListIterator& copy) = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// Destructs the LinkedListIterator.
			///	</summary> -----------------------------------------------------
			~LinkedListIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The LinkedListIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this LinkedListIterator with the copied data.
			/// </returns> -----------------------------------------------------
			LinkedListIterator& operator=(const LinkedListIterator& other) 
				= default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given LinkedListIterator for implicit 
			/// conversion from non-const version to a const LinkedListIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const LinkedListIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided LinkedListIterator to copy.
			/// </typeparam> ---------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			LinkedListIterator(LinkedListIterator<wasConst> copy)
				: LinkedListIterator(copy._node) {}

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

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> -----------------------------------------------------
			LinkedListIterator& operator++() {
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
			LinkedListIterator operator++(int) {
				auto copy = *this;
				_node = _node->to(next);
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Decrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns the
			/// iterator after updating.
			///	</returns> -----------------------------------------------------
			LinkedListIterator& operator--() {
				if (_node->to(prev))
					_node = _node->to(prev);
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Deccrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns a copy
			/// of the iterator before updating.
			///	</returns> -----------------------------------------------------
			LinkedListIterator operator--(int) {
				auto copy = *this;
				if (_node->to(prev))
					_node = _node->to(prev);
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The LinkedListIterator appearing on the left-hand side of the 
			/// '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The LinkedListIterator appearing on the right-hand side of the 
			/// '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the LinkedListIterators are both pointing to the
			/// same element, false otherwise.
			///	</returns> -----------------------------------------------------
			friend bool operator==(
				const LinkedListIterator& lhs,
				const LinkedListIterator& rhs
			) {
				return lhs._node == rhs._node;
			}
		};

		static_assert(
			std::bidirectional_iterator<iterator>,
			"LinkedListIterator is not a valid bidirectional iterator."
		);
	};


	static_assert(
		sequential_collection<LinkedList<int>>,
		"LinkedList does not meet the requirements for a sequential_collecion."
	);

	static_assert(
		bidirectional_collection<LinkedList<int>>,
		"LinkedList does not meet the requirements for a bidirectional collection."
	);
}