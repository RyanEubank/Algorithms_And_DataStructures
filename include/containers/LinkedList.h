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

#include "../algorithms/collection_algorithms.h"
#include "../concepts/sequential.h"
#include "../util/NamedType.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// LinkedList is a collection class that maintains a doubly linked list 
	/// for fast insertion/removal at the ends.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the LinkedList.
	/// </typeparam> 
	/// <typeparam name="allocator_t">
	/// The type of the allocator responsible for allocating memory to the 
	/// linked list.
	/// </typeparam> ----------------------------------------------------------
	template <class element_t, class allocator_t = std::allocator<element_t>>
	class LinkedList final {
	private:

		template <bool isConst>
		class LinkedListIterator;

		using node = struct _node;
		using alloc_traits = std::allocator_traits<allocator_t>::template rebind_traits<node>;
		using node_allocator_type = alloc_traits::allocator_type;

	public:

		using allocator_type = allocator_t;
		using value_type = allocator_type::value_type;
		using size_type = size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = std::allocator_traits<allocator_t>::pointer;
		using const_pointer = std::allocator_traits<allocator_t>::const_pointer;
		using iterator = LinkedListIterator<false>;
		using const_iterator = LinkedListIterator<true>;
		
		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty LinkedList.
		/// </para></summary> -------------------------------------------------
		constexpr LinkedList() noexcept(noexcept(allocator_type{})) :
			_head(nullptr), 
			_tail(nullptr),
			_size(),
			_allocator(),
			_node_allocator(allocator_type{})
		{}

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
		constexpr explicit LinkedList(const allocator_type& alloc) noexcept :
			_head(nullptr),
			_tail(nullptr),
			_size(),
			_allocator(alloc),
			_node_allocator(alloc)
		{}

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
		LinkedList(LinkedList&& other) noexcept(
			std::is_nothrow_move_constructible_v<allocator_type>
		) : LinkedList(std::move(other._allocator)) {
			swapData(*this, other);
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
			while (begin != end)
				insertBack(*begin++);		
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
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;
			constexpr bool alwaysEqual = 
				alloc_traits::is_always_equal::value;

			bool equalAllocators = this->_allocator == other._allocator;

			if constexpr (alwaysEqual)
				elementWiseCopyAssign(other);
			else if (propagate && !equalAllocators) {
				clear();
				_allocator = other._allocator;
				_node_allocator = other._node_allocator;
				insert(end(), other.begin(), other.end());
			}
			else
				elementWiseCopyAssign(other);
			
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
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_move_assignment::value;
			constexpr bool alwaysEqual =
				alloc_traits::is_always_equal::value;

			bool equalAllocators = this->_allocator == other._allocator;

			if constexpr (alwaysEqual)
				swapData(*this, other);
			else if (equalAllocators)
				swapData(*this, other);
			else if (propagate)
				swapAll(*this, other);
			else
				elementwiseMoveAssign(other);

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
		[[nodiscard]] reference operator[](size_t index) {
			const_reference element = std::as_const(*this)[index];
			return const_cast<reference>(element);
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
		[[nodiscard]] const_reference operator[](size_t index) const {
			return getNodeAt(index)->_element;
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
			while (_head) {
				node* next = _head->_next;
				remove(_head);
				_head = next;
			}

			_head = nullptr;
			_tail = nullptr;
			_size = 0;
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
			return LinkedListIterator<false>(_head);
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
			return LinkedListIterator<false>();
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
			return LinkedListIterator<true>(_head);
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
			return LinkedListIterator<true>();
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
			return _head->_element;
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
			return _head->_element;
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
			return _tail->_element;
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
			return _tail->_element;
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
			insertAt(_head, element);
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
			insertAt(_head, element);
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
			insertAt(nullptr, element);
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
			insertAt(nullptr, element);
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
		/// </param> ----------------------------------------------------------
		void insert(Index index, const_reference element) {
			insertAt(index, element);
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
		/// </param> ----------------------------------------------------------
		void insert(Index index, value_type&& element) {
			insertAt(index, element);
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
		/// </param> ----------------------------------------------------------
		void insert(const_iterator position, const_reference element) {
			insertAt(position._node, element);
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
		/// </param> ----------------------------------------------------------
		void insert(const_iterator position, value_type&& element) {
			insertAt(position._node, element);
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
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		void insert(const_iterator position, in_iterator begin, sentinel end) {
			while (begin != end) 
				insertAt(position._node, *begin++);
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
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		void insert(Index index, in_iterator begin, sentinel end) {
			size_type i = index.get();
			validateIndexInRange(i);
			iterator pos = iterator(getNodeAt(i));
			insert(pos, begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or the list is empty.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to be removed.
		/// </param> ----------------------------------------------------------
		void remove(Index index) {
			size_type i = index.get();
			validateIndexExists(i);
			node* n = getNodeAt(i);
			removeNode(n);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given iterator position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The iterator position of the element to be removed.
		/// </param> ----------------------------------------------------------
		void remove(const_iterator position) {
			removeNode(position._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the first element in the list.
		/// </summary> --------------------------------------------------------
		void removeFront() {
			removeNode(_head);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the last element in the list.
		/// </summary> --------------------------s------------------------------
		void removeBack() {
			removeNode(_tail);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements between the given indices [begin, end).
		/// </summary>
		/// 
		/// <param name="range">
		/// An index range with a start and end member.
		/// </param> ----------------------------------------------------------
		void remove(IndexRange range) {
			validateIndexExists(range.begin);
			validateIndexInRange(range.end);

			if (range.begin < range.end)
				removeAll(range.begin, range.end);
			else
				throw std::exception("Begin index is greater than end.");
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements in the given iterator range [begin, end).
		/// </summary> --------------------------------------------------------
		void remove(const_iterator begin, const_iterator end) {
			node* n = nullptr;
			while (begin != end) {
				n = begin._node;
				++begin;
				removeNode(n);
			}
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
			insertAt(_head, std::forward<Args>(args)...);
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
			insertAt(nullptr, std::forward<Args>(args)...);
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
		/// </param> ----------------------------------------------------------
		template <class ...Args>
		void emplace(Index index, Args&&... args) {
			insertAt(index, std::forward<Args>(args)...);
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
		/// </param> ----------------------------------------------------------
		template <class ...Args>
		void emplace(const_iterator position, Args&&... args) {
			insertAt(position._node, std::forward<Args>(args)...);
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
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_swap::value;
			constexpr bool alwaysEqual = 
				alloc_traits::is_always_equal::value;

			bool equalAllocators = a._allocator == b._allocator;

			if constexpr (alwaysEqual)
				swapData(a, b);
			else if (equalAllocators)
				swapData(a, b);
			else if (propagate) 
				swapAll(a, b);
			else
				throw std::exception("Swap on unequal, stateful allocators");
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
			// TODO: refactor and optimize
			value_type v{};
			size_type size = 0;
			node* n = nullptr;

			is >> size;
			list.clear();

			for (size_type i = 0; i < size; ++i) {
				is >> v;
				list.insertAt(nullptr, std::move(v));
			}

			return is;
		}

	private:

		struct _node {
			value_type _element;
			struct _node* _next;
			struct _node* _prev;

			template <class... Args>
			_node(Args&&... args) :
				_element(std::forward<Args>(args)...),
				_next(nullptr),
				_prev(nullptr)
			{}
		};

		node* _head;
		node* _tail;
		allocator_type _allocator;
		node_allocator_type _node_allocator;
		size_type _size;

		[[nodiscard]] node* allocate() {
			return alloc_traits::allocate(_node_allocator, 1);
		}

		void deallocate(node* n) {
			alloc_traits::deallocate(_node_allocator, n, 1);
		}

		template <class... Args>
		void constructNode(node* n, Args&&... args) {
			alloc_traits::construct(
				_node_allocator, 
				n, 
				std::forward<Args>(args)...
			);
		}

		void destroyNode(node* n) {
			alloc_traits::destroy(_node_allocator, n);
		}

		template <class... Args>
		[[nodiscard]] node* createNode(Args&&... args) {
			node* n = allocate();
			constructNode(n, std::forward<Args>(args)...);
			return n;
		}

		[[nodiscard]] const node* getNodeAt(size_type index) const {
			size_type half_size = _size << 1;

			if (index <= half_size)
				return traverseForwardFrom(_head, index);
			else
				return traverseBackwardFrom(_tail, _size - 1 - index);
		}

		[[nodiscard]] node* getNodeAt(size_type index) {
			const node* n = (std::as_const(*this).getNodeAt(index));
			return const_cast<node*>(n);
		}

		[[nodiscard]] node* traverseForwardFrom(
			node* n, 
			size_type numSteps
		) const {
			for (size_type i = 0; i < numSteps; ++i)
				n = n->_next;
			return n;
		}

		[[nodiscard]] node* traverseBackwardFrom(
			node* n, 
			size_type numSteps
		) const {
			for (size_type i = 0; i < numSteps; ++i)
				n = n->_prev;
			return n;
		}

		void linkBefore(node* location, node* newNode) {

			if (location->_prev != nullptr) {
				location->_prev->_next = newNode;
				newNode->_prev = location->_prev;
			}

			location->_prev = newNode;
			newNode->_next = location;
		}

		void linkAfter(node* location, node* newNode) {

			if (location->_next != nullptr) {
				location->_next->_prev = newNode;
				newNode->_next = location->_next;
			}

			location->_next = newNode;
			newNode->_prev = location;
		}

		void unlink(node* n) {
			if (n->_prev)
				n->_prev->_next = n->_next;
			if (n->_next)
				n->_next->_prev = n->_prev;
		}

		void assignFrom(const_iterator begin, const_iterator end) {
			iterator destination = this->begin();
			while (destination != this->end()) 
				*destination++ = *begin++;
			insert(this->end(), begin, end);
		}

		void elementWiseCopyAssign(const LinkedList& other) {
			if (size() < other.size()) 
				assignFrom(other.begin(), other.end());
			else {
				auto pos = collections::copy(other, begin());
				remove(pos, end());
			}
		}

		void elementwiseMoveAssign(LinkedList&& other) {
			if (size() < other.size()) 
				assignFrom(std::move_iterator(other.begin()), other.end());
			else {
				auto pos = collections::move(other, begin());
				remove(pos, end());
			}
		}

		template <class... Args>
		void insertAt(node* location, Args&&... args) {
			node* newNode = createNode(std::forward<Args>(args)...);

			if (isEmpty()) {
				_head = newNode;
				_tail = newNode;
			}
			else if (location == _head) {
				linkBefore(_head, newNode);
				_head = newNode;
			}
			else if (location == nullptr) {
				linkAfter(_tail, newNode);
				_tail = newNode;
			}
			else
				linkBefore(location, newNode);

			_size++;
		}

		template <class... Args>
		void insertAt(Index index, Args&&... args) {
			size_type i = index.get();
			validateIndexInRange(i);
			node* location = getNodeAt(i);
			insertAt(location, std::forward<Args>(args)...);
		}

		void updateEndsOnRemove(node* n) {
			if (n == _head)
				_head = _head->_next;
			if (n == _tail)
				_tail = _tail->_prev;
		}

		void removeNode(node* n) {
			updateEndsOnRemove(n);
			unlink(n);
			destroyNode(n);
			deallocate(n);
			_size--;
		}

		void removeAll(size_type begin_index, size_type end_index) {
			node* n = getNodeAt(begin_index);
			node* next = nullptr;

			while (begin_index++ < end_index) {
				next = n->_next;
				removeNode(n);
				n = next;
			}
		}

		void validateIndexExists(size_type index) const {
			if (index >= size())
				throwInvalidIndex(index);
		}

		void validateIndexInRange(size_type index) const {
			if (index > size())
				throwInvalidIndex(index);
		}

		[[noreturn]] void throwInvalidIndex(size_t index) const {
			constexpr auto INVALID_INDEX = "Invalid Index: out of range.";
			std::stringstream err{};

			err << INVALID_INDEX << std::endl << "Index: " << index
				<< " Size: " << size() << std::endl;
			throw std::out_of_range(err.str().c_str());
		}

		friend void swapData(LinkedList& a, LinkedList& b) noexcept {
			using std::swap;
			swap(a._head, b._head);
			swap(a._tail, b._tail);
			swap(a._size, b._size);
		}

		friend void swapAll(LinkedList& a, LinkedList& b) noexcept {
			using std::swap;
			swap(a._allocator, b._allocator);
			swap(a._node_allocator, b._node_allocator);
			swapData(a, b);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// LinkedListIterator is a class that implements bidirectional 
		/// iteration over a doubly linked list.
		/// </summary>
		///
		/// <typeparam name="element_t">
		/// The type of the elements iterated over by the LinkedListIterator.
		/// </typeparam> ------------------------------------------------------
		template <bool isConst>
		class LinkedListIterator {
		private:

			using node = LinkedList::node;
			node* _node;

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a LinkedListIterator pointing to the specified node 
			/// in a LinkedList.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> ------------------------------------------------------
			LinkedListIterator(node* node) : _node(node) {}

			friend class LinkedList;

		public:

			using value_type = std::conditional_t<isConst, const element_t, element_t>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::bidirectional_iterator_tag;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty LinkedListIterator.
			/// </para></summary> ---------------------------------------------
			LinkedListIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given LinkedListIterator.
			/// </para></summary> ---------------------------------------------
			LinkedListIterator(const LinkedListIterator& copy) = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// Destructs the LinkedListIterator.
			///	</summary> ----------------------------------------------------
			~LinkedListIterator() = default;

			// ----------------------------------------------------------------
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
			/// </returns> ----------------------------------------------------
			LinkedListIterator& operator=(const LinkedListIterator& other) 
				= default;

			// ----------------------------------------------------------------
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
			/// </typeparam> --------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>>
			LinkedListIterator(LinkedListIterator<wasConst> copy)
				: _node(copy._node) {}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the iterator
			/// in its current state.
			///	</returns> ----------------------------------------------------
			reference operator*() const {
				return _node->_element;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> ----------------------------------------------------
			LinkedListIterator& operator++() {
				_node = _node->_next;
				return *this;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy of
			/// the iterator before updating.
			///	</returns> ----------------------------------------------------
			LinkedListIterator operator++(int) {
				auto copy = *this;
				_node = _node->_next;
				return copy;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Decrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns the
			/// iterator after updating.
			///	</returns> ----------------------------------------------------
			LinkedListIterator& operator--() {
				_node = _node->_prev;
				return *this;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Deccrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns a copy
			/// of the iterator before updating.
			///	</returns> ----------------------------------------------------
			LinkedListIterator operator--(int) {
				auto copy = *this;
				_node = _node->_prev;
				return copy;
			}

			// ----------------------------------------------------------------
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
			///	</returns> ----------------------------------------------------
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
		sequential<LinkedList<int>>,
		"LinkedList does not implement the list interface."
	);
}