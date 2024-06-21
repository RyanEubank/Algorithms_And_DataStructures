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
#include "../concepts/collection.h"
#include "../adapters/TreeTraversalAdapters.h"

namespace collections {

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class allocator_t = std::allocator<element_t>
	> requires std::predicate<compare_t, element_t, element_t>
	class BinarySearchTree {
	private:

		template <bool isConst>
		class BinaryTreeIterator;

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
		using iterator = BinaryTreeIterator<false>;
		using const_iterator = BinaryTreeIterator<true>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty BinarySearchTree.
		/// </para></summary> -------------------------------------------------
		constexpr BinarySearchTree() 
			noexcept(std::is_nothrow_default_constructible_v<allocator_type>) :
			_sentinel(),
			_size(),
			_allocator(),
			_node_allocator(allocator_type{})
		{
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Allocator Constructor ---
		/// 
		///	<para>
		/// Constructs an empty BinarySearchTree.
		/// </para></summary>
		/// <param name="alloc">
		/// The allocator instance used by the tree.
		/// </param> ----------------------------------------------------------
		constexpr explicit BinarySearchTree(const allocator_type& alloc) 
			noexcept :
			_sentinel(),
			_size(),
			_allocator(alloc),
			_node_allocator(alloc)
		{
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		/// <para>
		/// Constructs a deep copy of the specified BinarySearchTree.
		/// </para></summary> 
		/// 
		/// <param name="copy">
		/// The BinarySearchTree to be copied.
		/// </param> ----------------------------------------------------------
		BinarySearchTree(const BinarySearchTree& copy) : BinarySearchTree(
			copy.begin<traversal_order::POST_ORDER>(),
			copy.end<traversal_order::POST_ORDER>(),
			alloc_traits::select_on_container_copy_construction(copy._allocator)
		) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Move Constructor ---
		/// 
		/// <para>
		/// Constructs a BinarySearchTree by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The BinarySearchTree to be moved into this one.
		/// </param> ----------------------------------------------------------
		BinarySearchTree(BinarySearchTree&& other) noexcept(
			std::is_nothrow_move_constructible_v<allocator_type>
		) : BinarySearchTree(std::move(other._allocator)) {
			swapData(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a BinarySearchTree with the a copy of the elements in
		/// the specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		BinarySearchTree(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : BinarySearchTree(init.begin(), init.end(), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a BinarySearchTree with the a copy of the elements from
		/// the given iterator pair.
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
		/// The allocator instance used by the tree. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel
		>
		BinarySearchTree(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : BinarySearchTree(alloc) {
			insert(begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a BinarySearchTree array with a copy of the elements
		/// from the given range.
		/// </para></summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to construct the tree with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the tree.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range range>
		BinarySearchTree(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : BinarySearchTree(std::ranges::begin(r), std::ranges::end(r), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the tree safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~BinarySearchTree() {
			clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified BinarySearchTree to this 
		/// one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The BinarySearchTree to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this BinarySearchTree with the copied data.
		/// </returns> --------------------------------------------------------
		BinarySearchTree& operator=(const BinarySearchTree& other) {
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;
			constexpr bool alwaysEqual = 
				alloc_traits::is_always_equal::value;

			bool equalAllocators = this->_allocator == other._allocator;

			clear();

			if (propagate && !equalAllocators) {
				_allocator = other._allocator;
				_node_allocator = other._node_allocator;
			}

			insert(other.begin(), other.end());

			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Moves Assignment Operator ~~~
		/// 
		/// <para>
		/// Moves the data from the specified BinarySearchTree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The BinarySearchTree to move from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this BinarySearchTree with the moved data.
		/// </returns> --------------------------------------------------------
		BinarySearchTree& operator=(BinarySearchTree&& other) 
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
			else {
				clear();
				auto begin = std::move_iterator(other.begin());
				auto end = std::move_iterator(other.end());
				insert(begin, end);
			}

			return *this;
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
		/// Returns whether the tree is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the tree has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const noexcept {
			return !(_size);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes and deletes all nodes from the tree emptying its contents.
		/// </summary> --------------------------------------------------------
		void clear() noexcept {
			auto begin = this->begin<traversal_order::POST_ORDER>();
			auto end = this->end<traversal_order::POST_ORDER>();

			while (begin != end) 
				remove((begin++)._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the number of elements contained by the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of valid, constructed elements in the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type size() const noexcept {
			return _size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the root of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const_reference to the element at the root of the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference root() const noexcept {
			return static_cast<const node*>(rootNode())->_element;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the minimum element in the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const_reference to the leftmost element in the tree, or
		/// the smallest node.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference minimum() const noexcept {
			return static_cast<const node*>(smallestNode())->_element;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the maximum element in the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const_reference to the rightmost element in the tree, or
		/// the largest node.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference maximum() const noexcept {
			return static_cast<const node*>(largestNode())->_element;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the tree's
		/// traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of traversal the iterator should implement.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a bidirectional iterator to the first element according to 
		/// the traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] iterator begin() noexcept {
			return iterator(this, firstNodeIn(order), order);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the tree's
		/// traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of traversal the iterator should implement.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a bidirectional iterator past the last element according to 
		/// the traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] iterator end() noexcept {
			return iterator(this, &_sentinel, order);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the tree's 
		/// traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of traversal the iterator should implement.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const bidirectional iterator to the first element 
		/// according to the traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_iterator begin() const noexcept {
			return const_iterator(this, firstNodeIn(order), order);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the tree's
		/// traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of traversal the iterator should implement.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const bidirectional iterator past the last element 
		/// according to the traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_iterator end() const noexcept {
			return const_iterator(this, &_sentinel, order);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the tree's 
		/// traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of traversal the iterator should implement.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const bidirectional iterator to the first element 
		/// according to the traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return const_iterator(this, firstNodeIn(order), order);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the tree's
		/// traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of traversal the iterator should implement.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const bidirectional iterator past the last element 
		/// according to the traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_iterator cend() const noexcept {
			return const_iterator(this, &_sentinel, order);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of forward traversal the iterator is based on.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a reverse iterator to the first element according to the
		/// reverse traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] reverse_iterator rbegin() noexcept {
			return std::make_reverse_iterator(end<order>());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the last element in the 
		/// tree's traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of forward traversal the iterator is based on.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a reverse iterator past the last element according to the 
		/// reverse traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] reverse_iterator rend() noexcept {
			return std::make_reverse_iterator(begin<order>());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of forward traversal the iterator is based on.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const reverse iterator to the first element according to 
		/// the reverse traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept {
			return std::make_reverse_iterator(end<order>());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the last element in the 
		/// tree's traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of forward traversal the iterator is based on.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const reverse iterator past the last element according to 
		/// the reverse traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_reverse_iterator rend() const noexcept {
			return std::make_reverse_iterator(begin<order>());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of forward traversal the iterator is based on.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const reverse iterator to the first element according to 
		/// the reverse traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept {
			return std::make_reverse_iterator(end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the last element in the 
		/// tree's traversal.
		/// </summary>
		/// 
		/// <typeparam name="order">
		/// The order of forward traversal the iterator is based on.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a const reverse iterator past the last element according to 
		/// the reverse traversal order.
		/// </returns> --------------------------------------------------------
		template <traversal_order order = traversal_order::IN_ORDER>
		[[nodiscard]] const_reverse_iterator crend() const noexcept {
			return std::make_reverse_iterator(begin<order>());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the given element.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the requested element if it exists in the
		/// tree, otherwise end() is returned.
		/// </returns> --------------------------------------------------------
		iterator find(const_reference element) {
			auto n = traverseTo(rootNode(), element);
			return n ? iterator(this, n) : end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the given element.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if an element matching the requested key is found.
		/// </returns> --------------------------------------------------------
		bool contains(const_reference element) {
			return traverseTo(rootNode(), element) != nullptr;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the given element.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_iterator to the requested element if it exists in 
		/// the tree, otherwise end() is returned.
		/// </returns> --------------------------------------------------------
		const_iterator find(const_reference element) const {
			auto n = traverseTo(rootNode(), element);
			return n ? const_iterator(this, n) : end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the first element that matches the predicate.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		///  Returns true if an element matching the requested key is found.
		/// </returns> --------------------------------------------------------
		bool contains(const_reference element) const {
			return traverseTo(rootNode(), element) != nullptr;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the tree.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element, or the element
		/// preventing insertion if unsuccessful.
		/// </returns> --------------------------------------------------------
		iterator insert(const_reference element) {
			return insertAt(rootNode(), element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the tree.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element, or the element
		/// preventing insertion if unsuccessful.
		/// </returns> --------------------------------------------------------
		iterator insert(value_type&& element) {
			return insertAt(rootNode(), element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the tree.
		/// </summary>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the range to insert.
		/// </param>
		/// <param name="end">
		/// The end iterator of the range to insert.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the last element inserted, or the element 
		/// preventing its insertion. Returns the end() iterator for the tree
		/// if begin == end.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		iterator insert(in_iterator begin, sentinel end) {
			iterator result = this->end();
			while (begin != end) 
				result = insertAt(rootNode(), *begin++);
			return result;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the tree with position as a 
		/// location hint.
		/// </summary>
		/// 
		/// <param name="position">
		/// The location hint of where to try insertion at.
		/// </param>
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element, or the element
		/// preventing insertion.
		/// </returns> --------------------------------------------------------
		iterator insert(const_iterator position, const_reference element) {
			return insertAt(position._node, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the tree, with position as a 
		/// location hint.
		/// </summary>
		/// 
		/// <param name="position">
		/// The location hint of where to try insertion at.
		/// </param>
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element, or the element
		/// preventing insertion.
		/// </returns> --------------------------------------------------------
		iterator insert(const_iterator position, value_type&& element) {
			return insertAt(position._node, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the tree, with position as a location
		/// hint.
		/// </summary>
		/// 
		/// <param name="position">
		/// The location hint of where to try insertion at.
		/// </param>
		/// <param name="begin">
		/// The beginning iterator of the range to insert.
		/// </param>
		/// <param name="end">
		/// The end iterator of the range to insert.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the last element inserted, or the element 
		/// preventing its insertion. Returns the end() iterator for the tree
		/// if begin == end.
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
			auto result = iterator(this, position._node);
			while (begin != end) 
				result = insertAt(result._node, *begin++);
			return result;
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
			auto begin = position++;
			return remove(begin, position);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes all elements in the given iterator range [begin, end).
		/// </summary>
		/// 
		/// <returns>
		/// Returns end after removing elements in the given range.
		/// </returns> --------------------------------------------------------
		iterator remove(
			const_iterator begin, 
			const_iterator end
		) {
			while (begin != end) 
				remove((begin++)._node);
			return iterator(this, end._node);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the given element in-place with the provided arguments
		/// before the given iterator position.
		/// </summary>
		/// 
		/// <param name="args">
		///The arguments to construct the new element with.
		/// </param>
		///
		/// <returns>
		/// Returns an iterator to the inserted element.
		/// </returns> --------------------------------------------------------
		template <class T, class ...Args> 
			requires (!std::convertible_to<T, const_iterator>)
		iterator emplace(T&& arg1, Args&&... args) {
			return insertAt(rootNode(), arg1, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs the given element in-place with the provided arguments
		/// before the given iterator position.
		/// </summary>
		/// 
		/// <param name="position">
		/// The location hint of where to try insertion at.
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
			return insertAt(position._node, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given BinarySearchTrees.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first tree to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second tree to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(BinarySearchTree& a, BinarySearchTree& b) 
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
		/// The BinarySearchTree appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The BinarySearchTree appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given trees share equality based on size and
		/// ordering according to in-order traversal. This is not the same as 
		/// isomorphic equality.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const BinarySearchTree& lhs,
			const BinarySearchTree& rhs
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
		/// The BinarySearchTree appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The BinarySearchTree appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given trees share exact element ordering
		/// according to in-order traversal.
		/// Always returns false for trees of different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(
			const BinarySearchTree& lhs,
			const BinarySearchTree& rhs
		) noexcept requires std::three_way_comparable<value_type> {
			using comparison = decltype(value_type{} <=> value_type{});

			auto compareSize = lhs.size() <=> rhs.size();
			if (compareSize == 0)
				return collections::lexicographic_compare(lhs, rhs);

			return static_cast<comparison>(compareSize);
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
		/// The tree being read from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const BinarySearchTree& tree
		) {
			auto begin = tree.begin<traversal_order::PRE_ORDER>();
			auto end = tree.end<traversal_order::PRE_ORDER>();
			collections::stream(begin, end, tree.size(), os);
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
			BinarySearchTree& tree
		) {
			size_type size = 0;
			value_type value{};
			is >> size;

			tree.clear();

			for (size_type i = 0; i < size; ++i) {
				is >> value;
				tree.insert(value);
			}

			return is;
		}

	private:

		struct _node_base {
			_node_base* _parent = nullptr;
			_node_base* _left = nullptr;
			_node_base* _right = nullptr;

			[[nodiscard]] size_type degree() const {
				if (_left && _right)
					return 2;
				else if (_left || _right)
					return 1;
				else
					return 0;
			}

			[[nodiscard]] bool isLeaf() const {
				return degree() == 0;
			}
		};

		struct _node : _node_base {
			value_type _element;

			template <class ... Args>
			_node(Args&&... args) : _element(std::forward<Args>(args)...) {}
		};

		_node_base _sentinel;
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

		void initSentinel(_node_base* n) {
			_sentinel._left = n;
			_sentinel._right = n;
			_sentinel._parent = n;
		}

		[[nodiscard]] _node_base* rootNode() {
			return _sentinel._parent;
		}

		[[nodiscard]] const _node_base* rootNode() const {
			return _sentinel._parent;
		}

		[[nodiscard]] _node_base* smallestNode() {
			return _sentinel._left;
		}

		[[nodiscard]] const _node_base* smallestNode() const {
			return _sentinel._left;
		}

		[[nodiscard]] _node_base* largestNode() {
			return _sentinel._right;
		}

		[[nodiscard]] const _node_base* largestNode() const {
			return _sentinel._right;
		}

		[[nodiscard]] static reference elementOf(_node_base* n) {
			return static_cast<node*>(n)->_element;
		}

		[[nodiscard]] static const_reference elementOf(const _node_base* n) {
			return static_cast<const node*>(n)->_element;
		}

		[[nodiscard]] static bool compare(
			const _node_base* n1,
			const _node_base* n2
		) {
			return compare(elementOf(n1), elementOf(n2));
		}

		[[nodiscard]] static bool compare(
			const_reference key, 
			const _node_base* n
		) {
			return compare_t{}(key, elementOf(n));
		}

		[[nodiscard]] static bool compare(
			const _node_base* n,
			const_reference key
		) {
			return compare_t{}(elementOf(n), key);
		}

		[[nodiscard]] static bool compare(
			const_reference e1,
			const_reference e2
		) {
			return compare_t{}(e1, e2);
		}

		template <class... Args>
		iterator insertAt(_node_base* hint, Args&&... args) {
			node* child = createNode(std::forward<Args>(args)...);
			_node_base* parent = getInsertLocation(hint, child->_element);
			const _node_base* result = tryInsert(parent, child);
			return iterator(this, result);
		}

		[[nodiscard]] _node_base* getInsertLocation(
			const _node_base* hint, 
			const_reference key
		) {
			const _node_base* result = hint;

			if (isEmpty())
				result = nullptr;
			else if (compare(key, smallestNode()))
				result = smallestNode();
			else if (compare(largestNode(), key))
				result = largestNode();
			else if (!hint || hint == rootNode())
				result = findParent(rootNode(), key);
			else if (compare(key, hint))
				result = checkPredecessor(hint, key);
			else if (compare(hint, key))
				result = checkSuccessor(hint, key);
			
			return const_cast<_node_base*>(result);
		}

		[[nodiscard]] const _node_base* checkPredecessor(
			const _node_base* hint,
			const_reference key
		) {
			auto prev = inOrderPredecessorOf(hint);
			if (compare(prev, key)) 
				return prev->_right ? hint : prev;
			return findParent(rootNode(), key);
		}

		[[nodiscard]] const _node_base* checkSuccessor(
			const _node_base* hint,
			const_reference key
		) {
			auto next = inOrderSuccessorOf(hint);
			if (!next || compare(key, next)) 
				return hint->_right ? next : hint;
			return findParent(rootNode(), key);
		}

		[[nodiscard]] const _node_base* tryInsert(
			_node_base* parent, 
			node* child
		) {
			if (parent) {
				const _node_base* duplicate = stepToward(parent, child->_element);
				if (!duplicate) 
					link(parent, child);
				else {
					destroyNode(child);
					return duplicate;
				}
			}
			else 
				initSentinel(child);

			_size++;
			return child;
		}

		void link(_node_base* parent, _node_base* child) {
			child->_parent = parent;

			if (compare(child, parent))
				parent->_left = child;
			else
				parent->_right = child;

			if (_sentinel._left == parent && parent->_left == child) 
				_sentinel._left = child;
			else if (_sentinel._right == parent && parent->_right == child) 
				_sentinel._right = child;
		}

		void remove(_node_base* n) {
			if (n) {
				_node_base* replacement = replacementFor(n);
				unlink(n, replacement);
				destroyNode(static_cast<node*>(n));
				_size--;
			}
		}

		[[nodiscard]] _node_base* replacementFor(const _node_base* n) {
			size_type degree = n->degree();

			if (degree == 0)
				return nullptr;
			else if (degree == 1) 
				return n->_left ? n->_left : n->_right;
			else 
				return const_cast<_node_base*>(inOrderPredecessorOf(n));
		}

		void unlink(
			const _node_base* n,
			_node_base* replacement
		) {
			if (replacement) {
				if (n->degree() == 2) {
					if (replacement->_parent->_right == replacement)
						replacement->_parent->_right = replacement->_left;
					else
						replacement->_parent->_left = replacement->_right;

					if (replacement != n->_left) {
						replacement->_left = n->_left;
						if (n->_left)
							n->_left->_parent = replacement;
					}
					if (replacement != n->_right) {
						replacement->_right = n->_right;
						if (n->_right)
							n->_right->_parent = replacement;
					}
				}
				replacement->_parent = n->_parent;
			}

			if (n->_parent) {
				if (n->_parent->_left == n)
					n->_parent->_left = replacement;
				else
					n->_parent->_right = replacement;
			}

			if (_sentinel._left == n)
				_sentinel._left = replacement ? replacement : n->_parent;
			if (_sentinel._right == n)
				_sentinel._right = replacement ? replacement : n->_parent;
			if (_sentinel._parent == n)
				_sentinel._parent = replacement;
		}

		[[nodiscard]] const _node_base* firstNodeIn(
			traversal_order order
		) const {
			if (isEmpty())
				return &_sentinel;

			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return smallestNode();
			case collections::traversal_order::PRE_ORDER:
				return rootNode();
			case collections::traversal_order::POST_ORDER:
				return findNextLeftSubtree(rootNode());
			case collections::traversal_order::LEVEL_ORDER:
				return rootNode();
			default:
				return &_sentinel;
			}
		}

		[[nodiscard]] const _node_base* lastNodeIn(
			traversal_order order
		) const {
			if (isEmpty())
				return &_sentinel;

			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return largestNode();
			case collections::traversal_order::PRE_ORDER:
				return findNextRightSubtree(rootNode());
			case collections::traversal_order::POST_ORDER:
				return rootNode();
			case collections::traversal_order::LEVEL_ORDER:
				throw std::exception("Not yet implemented"); //TODO
			default:
				return &_sentinel;
			}
		}

		friend void swapData(BinarySearchTree& a, BinarySearchTree& b) noexcept {
			using std::swap;

			if (a.isEmpty() && b.isEmpty())
				return;

			auto a_left = a._sentinel._left;
			auto a_right = a._sentinel._right;
			auto a_parent = a._sentinel._parent;
			auto b_left = b._sentinel._left;
			auto b_right = b._sentinel._right;
			auto b_parent = b._sentinel._parent;

			a._sentinel._left = b.isEmpty() ? &a._sentinel : b_left;
			a._sentinel._right = b.isEmpty() ? &a._sentinel : b_right;
			a._sentinel._parent = b.isEmpty() ? &a._sentinel : b_parent;

			b._sentinel._left = a.isEmpty() ? &b._sentinel : a_left;
			b._sentinel._right = a.isEmpty() ? &b._sentinel : a_right;
			b._sentinel._parent = a.isEmpty() ? &b._sentinel : a_parent;

			swap(a._size, b._size);
		};

		friend void swapAll(BinarySearchTree& a, BinarySearchTree& b) noexcept {
			using std::swap;
			swap(a._allocator, b._allocator);
			swap(a._node_allocator, b._node_allocator);
			swapData(a, b);
		}

		[[nodiscard]] static const _node_base* stepToward(
			const _node_base* from, 
			const_reference key
		) {
			if (compare(key, from))
				return from->_left;
			else if (compare(from, key))
				return from->_right;
			else
				return from;
		}

		[[nodiscard]] static const _node_base* findParent(
			const _node_base* root,
			const_reference key
		) {
			const _node_base* parent = root;
			const _node_base* child = parent;

			while (child && (elementOf(child) != key)) {
				parent = child;
				child = stepToward(parent, key);
			}

			return parent;
		}

		[[nodiscard]] static const _node_base* traverseTo(
			const _node_base* root,
			const_reference key
		) {
			while (root && (elementOf(root) != key))
				root = stepToward(root, key);
			return root;
		}

		[[nodiscard]] static const _node_base* leftMostChildOf(
			const _node_base* n
		) {
			while (n->_left)
				n = n->_left;
			return n;
		}

		[[nodiscard]] static const _node_base* rightMostChildOf(
			const _node_base* n
		) {
			while (n->_right)
				n = n->_right;
			return n;
		}

		[[nodiscard]] static const _node_base* leftMostAncestorOf(
			const _node_base* n
		) {
			const _node_base* parent = n->_parent;
			while (parent && parent->_left == n) {
				n = parent;
				parent = parent->_parent;
			}
			return n;
		}

		[[nodiscard]] static const _node_base* rightMostAncestorOf(
			const _node_base* n
		) {
			const _node_base* parent = n->_parent;
			while (parent && parent->_right == n) {
				n = parent;
				parent = parent->_parent;
			}
			return n;
		}

		[[nodiscard]] static const _node_base* findNextLeftSubtree(
			const _node_base* n
		) {
			while (n && !n->isLeaf())
				n = n->_left ? n->_left : n->_right;
			return n;
		}

		[[nodiscard]] static const _node_base* findNextRightSubtree(
			const _node_base* n
		) {
			while (n && !n->isLeaf())
				n = n->_right ? n->_right : n->_left;
			return n;
		}

		[[nodiscard]] static const _node_base* successorOf(
			const _node_base* n, 
			traversal_order order
		) {
			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return inOrderSuccessorOf(n);
			case collections::traversal_order::PRE_ORDER:
				return preOrderSuccessorOf(n);
			case collections::traversal_order::POST_ORDER:
				return postOrderSuccessorOf(n);
			case collections::traversal_order::LEVEL_ORDER:
				return levelOrderSuccessorOf(n);
			default:
				return n;
			}
		}


		[[nodiscard]] static const _node_base* predecessorOf(
			const _node_base* n, 
			traversal_order order
		) {
			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return inOrderPredecessorOf(n);
			case collections::traversal_order::PRE_ORDER:
				return preOrderPredecessorOf(n);
			case collections::traversal_order::POST_ORDER:
				return postOrderPredecessorOf(n);
			case collections::traversal_order::LEVEL_ORDER:
				return levelOrderPredecessorOf(n);
			default:
				return n;
			}
		}

		[[nodiscard]] static const _node_base* inOrderSuccessorOf(
			const _node_base* n
		) {
			if (!n)
				return nullptr;

			// if right sub-tree exists successor is left-most node
			if (n->_right) 
				return leftMostChildOf(n->_right);
			// else traverse back up to the root of the next unexplored subtree
			else {
				const _node_base* next = rightMostAncestorOf(n);
				return next->_parent;
			}
		}

		[[nodiscard]] static const _node_base* preOrderSuccessorOf(
			const _node_base* n
		) {
			if (!n)
				return n;

			// if a left or right child exist return the child first
			if (n->_left)
				return n->_left;
			else if (n->_right)
				return n->_right;
			// otherwise traverse up to next unexplored right subtree
			else {
				auto next = rightMostAncestorOf(n);
				return next->_parent ? next->_parent->_right : nullptr;
			}
		}

		[[nodiscard]] static const _node_base* postOrderSuccessorOf(
			const _node_base* n
		) {
			if (!n)
				return n;

			// if at root or a right child then parent is successor
			auto result = n->_parent;
			if (!result || result->_right == n)
				return result;
		
			// otherwise find deepest level of right sibling
			return findNextLeftSubtree(result->_right);
		}

		[[nodiscard]] static const _node_base* levelOrderSuccessorOf(
			const _node_base* n
		) {
			if (!n)
				return n;

			throw std::exception("Not yet implemented"); //TODO
		}

		[[nodiscard]] static const _node_base* inOrderPredecessorOf(
			const _node_base* n
		) {
			if (!n)
				return n;

			// if left sub-tree exists predeccessor is right-most node
			if (n->_left) 
				return rightMostChildOf(n->_left);
			// otherwise traverse up until the node is a right child
			// and return its parent.
			else {
				const _node_base* next = leftMostAncestorOf(n);
				return next->_parent;
			}
		}

		[[nodiscard]] static const _node_base* preOrderPredecessorOf(
			const _node_base* n
		) {
			if (!n || !n->_parent)
				return nullptr;

			// return left sibling if it exists
			if (n->_parent->_left && n->_parent->_left != n)
				return n->_parent->_left;
			// else return parent
			else
				return n->_parent;
		}

		[[nodiscard]] static const _node_base* postOrderPredecessorOf(
			const _node_base* n
		) {
			if (!n)
				return n;

			// if a right or left child exist return the child first
			if (n->_right)
				return n->_right;
			else if (n->_left)
				return n->_left;
			// otherwise traverse up until the node is a right child
			// and return its left sibling.
			else {
				const _node_base* next = leftMostAncestorOf(n);
				return next->_parent ? next->_parent->_left : nullptr;
			}
		}

		[[nodiscard]] static const _node_base* levelOrderPredecessorOf(
			const _node_base* n
		) {
			if (!n)
				return n;

			throw std::exception("Not yet implemented"); //TODO
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// BinaryTreeIterator is a class that implements bidirectional 
		/// iteration over a binary tree.
		/// </summary>
		///
		/// <typeparam name="isConst">
		/// Boolean whether the type is an iterator or const_iterator.
		/// </typeparam> ------------------------------------------------------
		template <bool isConst>
		class BinaryTreeIterator {
		private:

			using base_node = BinarySearchTree::_node_base;
			using node = BinarySearchTree::node;

			const BinarySearchTree* _tree;
			base_node* _node;
			traversal_order _order;

			explicit BinaryTreeIterator(
				const BinarySearchTree* tree,
				const base_node* n, 
				traversal_order order = traversal_order::IN_ORDER
			) : _tree(tree), _node(const_cast<base_node*>(n)), _order(order) {
				
			}

			void increment() {
				const base_node* successor = successorOf(_node, _order);
				const base_node* next = successor ? successor : &_tree->_sentinel;
				_node = const_cast<base_node*>(next);
			}

			void decrement() {
				if (_node == &_tree->_sentinel)
					_node = const_cast<base_node*>(_tree->lastNodeIn(_order));
				else {
					auto prev = predecessorOf(_node, _order);
					_node = const_cast<base_node*>(prev);
				}
			}

			friend class BinarySearchTree;

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
			/// Constructs an empty BinaryTreeIterator.
			/// </para></summary> ---------------------------------------------
			BinaryTreeIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given BinaryTreeIterator.
			/// </para></summary> ---------------------------------------------
			BinaryTreeIterator(const BinaryTreeIterator& copy) = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// Destructs the BinaryTreeIterator.
			///	</summary> ----------------------------------------------------
			~BinaryTreeIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The BinaryTreeIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this BinaryTreeIterator with the copied data.
			/// </returns> ----------------------------------------------------
			BinaryTreeIterator& operator=(const BinaryTreeIterator& other) 
				= default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given BinaryTreeIterator for implicit 
			/// conversion from non-const version to a const BinaryTreeIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const BinaryTreeIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided BinaryTreeIterator to copy.
			/// </typeparam> --------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			BinaryTreeIterator(BinaryTreeIterator<wasConst> copy) : 
				BinaryTreeIterator(copy._tree, copy._node, copy._order)
			{

			}

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
				return elementOf(_node);
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
			BinaryTreeIterator& operator++() {
				increment();
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
			BinaryTreeIterator operator++(int) {
				auto copy = *this;
				increment();
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
			BinaryTreeIterator& operator--() {
				decrement();
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
			BinaryTreeIterator operator--(int) {
				auto copy = *this;
				decrement();
				return copy;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The BinaryTreeIterator appearing on the left-hand side of the 
			/// '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The BinaryTreeIterator appearing on the right-hand side of the 
			/// '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the BinaryTreeIterator are both pointing to the
			/// same element, false otherwise.
			///	</returns> ----------------------------------------------------
			friend bool operator==(
				const BinaryTreeIterator& lhs,
				const BinaryTreeIterator& rhs
			) {
				return lhs._node == rhs._node;
			}
		};

		static_assert(
			std::bidirectional_iterator<iterator>,
			"BinaryTreeIterator is not a valid bidirectional iterator."
		);
	};

	static_assert(
		associative_collection<BinarySearchTree<int>>,
		"BinarySearchTree does not implement the associative collection interface."
	);

	static_assert(
		bidirectional_collection<BinarySearchTree<int>>,
		"BinarySearchTree does not meet the requirements for a bidirectional collection."
	);

	static_assert(
		ordered_collection<BinarySearchTree<int>>,
		"BinarySearchTree does not meet the requirements for an ordered collection."
	);
}