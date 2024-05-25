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
			setSentinel(&_sentinel);
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
			setSentinel(&_sentinel);
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
				insert(
					std::move_iterator(other.begin()), 
					std::move_iterator(other.end())
				);
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
			return iterator(
				this, const_cast<_node_base*>(firstNodeIn(order)), order);
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
			return const_iterator(
				this, const_cast<_node_base*>(firstNodeIn(order)), order);
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
			return const_iterator(
				this, const_cast<_node_base*>(&_sentinel), order);
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
			return const_iterator(
				this, const_cast<_node_base*>(firstNodeIn(order)), order);
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
			return const_iterator(
				this, const_cast<_node_base*>(&_sentinel), order);
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
			node* n = traverseTo(element);
			return isNull(n) ? end() : iterator(this, n);
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
			return !isNull(traverseTo(element));
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
			const _node_base* n = traverseTo(element);
			return isNull(n) 
				? end() 
				: const_iterator(this, const_cast<_node_base*>(n));
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
			return !isNull(traverseTo(element));
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
			return tryInsert(element);
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
			return tryInsert(element);
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
				result = tryInsert(*begin++);

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
			return tryInsert(position._node, element);
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
			return tryInsert(position._node, element);
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
				result = tryInsert(result._node, *begin++);
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
		/// <returns>		/// Returns end after removing elements in the given range.
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
			return tryInsert(arg1, std::forward<Args>(args)...);
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
			return tryInsert(position._node, std::forward<Args>(args)...);
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
			is >> size;

			tree.clear();

			for (size_type i = 0; i < size; ++i) {
				value_type value;
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

		void setSentinel(_node_base* n) {
			_sentinel._left = n;
			_sentinel._right = n;
			_sentinel._parent = n;
		}

		[[nodiscard]] _node_base* root() {
			return _sentinel._parent;
		}

		[[nodiscard]] const _node_base* root() const {
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


		[[nodiscard]] bool isLeaf(const _node_base* n) const {
			return degree(n) == 0;
		}

		[[nodiscard]] size_type degree(const _node_base* n) const {
			size_type result = 0;
			if (!isNull(n->_left))
				result++;
			if (!isNull(n->_right))
				result++;
			return result;
		}

		[[nodiscard]] bool isDuplicate(
			const _node_base* n, 
			const_reference key
		) const {
			return static_cast<const node*>(n)->_element == key;
		}

		[[nodiscard]] bool isNull(const _node_base* n) const {
			return !n || n == &_sentinel;
		}

		[[nodiscard]] bool compare(
			const _node_base* n1,
			const _node_base* n2
		) const {
			auto e1 = static_cast<const node*>(n1)->_element;
			return compare(e1, n2);
		}

		[[nodiscard]] bool compare(
			const_reference key, 
			const _node_base* n
		) const {
			return compare_t{}(key, static_cast<const node*>(n)->_element);
		}

		[[nodiscard]] bool compare(
			const _node_base* n,
			const_reference key
		) const {
			return compare_t{}(static_cast<const node*>(n)->_element, key);
		}

		[[nodiscard]] bool compare(
			const_reference e1,
			const_reference e2
		) const {
			return compare_t{}(e1, e2);
		}

		[[nodiscard]] const _node_base* takeStep(
			const _node_base* from, 
			const_reference key
		) const {
			if (compare(key, from))
				return from->_left;
			else if (compare(from, key))
				return from->_right;
			else
				return from;
		}

		[[nodiscard]] _node_base* findParent(const_reference key) {
			const _node_base* parent = std::as_const(*this).findParent(key);
			return const_cast<_node_base*>(parent);
		}

		[[nodiscard]] const _node_base* findParent(const_reference key) const {
			const _node_base* parent = root();
			const _node_base* child = parent;

			while (!isNull(child) && !isDuplicate(child, key)) {
				parent = child;
				child = takeStep(parent, key);
			}
			return parent;
		}

		[[nodiscard]] _node_base* traverseTo(const_reference key) {
			const _node_base* n = std::as_const(*this).traverseTo(key);
			return const_cast<_node_base*>(n);
		}

		[[nodiscard]] const _node_base* traverseTo(const_reference key) const {
			const _node_base* parent = findParent(key);
			if (isNull(parent) || isDuplicate(parent, key))
				return parent;
			else
				return takeStep(parent, key);
		}

		[[nodiscard]] const _node_base* floor(
			const _node_base* hint, 
			const_reference key
		) const {
			const _node_base* predecessor = inOrderPredecessorOf(hint);
			while (!isNull(predecessor) && compare(key, predecessor)) {
				hint = predecessor;
				predecessor = inOrderPredecessorOf(hint);
			}
			return hint;
		}

		[[nodiscard]] const _node_base* ceiling(
			const _node_base* hint, 
			const_reference key
		) const {
			const _node_base* successor = inOrderSuccessorOf(hint);
			while (!isNull(successor) && compare(successor, key)) {
				hint = successor;
				successor = inOrderSuccessorOf(hint);
			}
			return hint;
		}

		[[nodiscard]] _node_base* checkHint(
			const _node_base* hint, 
			const_reference key
		) {
			const _node_base* result = hint;
			if (!isNull(hint)) {
				if (compare(key, hint))
					result = floor(hint, key);
				else if (compare(hint, key))
					result = ceiling(hint, key);
			}
			return const_cast<_node_base*>(result);
		}

		template <class... Args>
		iterator tryInsert(Args&&... args) {
			node* child = createNode(std::forward<Args>(args)...);
			_node_base* parent = findParent(child->_element);
			_node_base* result = tryLink(parent, child);
			return iterator(this, result);
		}

		template <class... Args>
		iterator tryInsert(_node_base* n, Args&&... args) {
			node* child = createNode(std::forward<Args>(args)...);
			_node_base* parent = checkHint(n, child->_element);
			_node_base* result = tryLink(parent, child);
			return iterator(this, result);
		}

		_node_base* tryLink(_node_base* parent, node* child) {
			if (isNull(parent)) {
				_sentinel._parent = child;
				_sentinel._left = child;
				_sentinel._right = child;
			}
			else if (!isDuplicate(parent, child->_element)) {
				linkNodes(parent, child);
				updateSentinelOnInsert(parent, child);
			}
			else {
				destroyNode(child);
				return parent;
			}

			_size++;
			return child;
		}

		void linkNodes(_node_base* parent, _node_base* child) {
			child->_parent = parent;

			if (compare(child, parent))
				parent->_left = child;
			else
				parent->_right = child;
		}

		void updateSentinelOnInsert(
			const _node_base* parent, 
			_node_base* child
		) {
			if (_sentinel._left == parent && parent->_left == child) 
				_sentinel._left = child;
			if (_sentinel._right == parent && parent->_right == child) 
				_sentinel._right = child;
		}

		void remove(_node_base* n) {
			if (isNull(n))
				return;

			_node_base* replacement = getReplacementFor(n);
			updateReplacementLinks(n, replacement);
			updateParentLinks(n, replacement);
			updateSentinelOnRemove(n);
			destroyNode(static_cast<node*>(n));
			_size--;
		}

		_node_base* getReplacementFor(const _node_base* n) {
			size_type degree = this->degree(n);

			if (degree == 0) 
				return nullptr;
			else if (degree == 1) 
				return isNull(n->_left) ? n->_right : n->_left;
			else 
				return const_cast<_node_base*>(inOrderPredecessorOf(n));
		}

		void updateReplacementLinks(
			const _node_base* n, 
			_node_base* replacement
		) {
			if (!isNull(replacement)) {
				if (replacement != n->_right) {
					replacement->_right = n->_right;

					if (replacement != n->_left) {
						replacement->_parent->_right = replacement->_left;
						replacement->_left = n->_left;
					}
				}
				replacement->_parent = n->_parent;
			}
		}

		void updateParentLinks(
			const _node_base* n, 
			_node_base* replacement
		) {
			_node_base* parent = n->_parent;

			if (!isNull(parent)) {
				if (parent->_left == n)
					parent->_left = replacement;
				else
					parent->_right = replacement;
			}
		}

		void updateSentinelOnRemove(const _node_base* n) {
			if (_sentinel._left == n)
				_sentinel._left = n->_parent;
			if (_sentinel._right == n)
				_sentinel._right = n->_parent;
			if (_sentinel._parent == n)
				_sentinel._parent = n->_parent;
		}

		const _node_base* leftMostChildOf(const _node_base* n) const {
			while (!isNull(n->_left))
				n = n->_left;
			return n;
		}

		const _node_base* rightMostChildOf(const _node_base* n) const {
			while (!isNull(n->_right))
				n = n->_right;
			return n;
		}

		const _node_base* leftMostAncestorOf(const _node_base* n) const {
			auto parent = n->_parent;
			while (!isNull(parent) && parent->_left == n) {
				n = parent;
				parent = parent->_parent;
			}
			return n;
		}

		const _node_base* rightMostAncestorOf(const _node_base* n) const {
			auto parent = n->_parent;
			while (!isNull(parent) && parent->_right == n) {
				n = parent;
				parent = parent->_parent;
			}
			return n;
		}

		const _node_base* findNextLeftSubtree(const _node_base* n) const {
			while (!isNull(n) && !isLeaf(n))
				n = !isNull(n->_left) ? n->_left : n->_right;
			return n;
		}

		const _node_base* findNextRightSubtree(const _node_base* n) const {
			while (!isNull(n) && !isLeaf(n))
				n = !isNull(n->_right) ? n->_right : n->_left;
			return n;
		}

		const _node_base* firstNodeIn(traversal_order order) const {
			if (isEmpty())
				return &_sentinel;

			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return smallestNode();
			case collections::traversal_order::PRE_ORDER:
				return root();
			case collections::traversal_order::POST_ORDER:
				return findNextLeftSubtree(root());
			case collections::traversal_order::LEVEL_ORDER:
				return root();
			default:
				return &_sentinel;
			}
		}

		const _node_base* lastNodeIn(traversal_order order) const {
			if (isEmpty())
				return &_sentinel;

			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return largestNode();
			case collections::traversal_order::PRE_ORDER:
				return findNextRightSubtree(root());
			case collections::traversal_order::POST_ORDER:
				return root();
			case collections::traversal_order::LEVEL_ORDER:
				throw std::exception("Not yet implemented"); //TODO
			default:
				return &_sentinel;
			}
		}

		const _node_base* successorOf(
			const _node_base* n, 
			traversal_order order
		) const {
			switch (order) {
			case collections::traversal_order::IN_ORDER:
				n = inOrderSuccessorOf(n);
				break;
			case collections::traversal_order::PRE_ORDER:
				n = preOrderSuccessorOf(n);
				break;
			case collections::traversal_order::POST_ORDER:
				n = postOrderSuccessorOf(n);
				break;
			case collections::traversal_order::LEVEL_ORDER:
				n = levelOrderSuccessorOf(n);
				break;
			}
			return n;
		}

		const _node_base* inOrderSuccessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			// if right sub-tree exists successor is left-most node
			if (!isNull(n->_right)) 
				return leftMostChildOf(n->_right);
			// else traverse back up to the root of the next unexplored subtree
			else {
				auto next = rightMostAncestorOf(n);
				return next != root() ? next->_parent : &_sentinel;
			}
		}

		const _node_base* preOrderSuccessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			// if a left or right child exist return the child first
			if (!isNull(n->_left))
				return n->_left;
			else if (!isNull(n->_right))
				return n->_right;
			// otherwise traverse up to next unexplored right subtree
			else {
				auto next = rightMostAncestorOf(n);
				return next != root() ? next->_parent->_right : &_sentinel;
			}
		}

		const _node_base* postOrderSuccessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			// if at root or a right child then parent is successor
			auto result = n->_parent;
			if (isNull(result))
				return &_sentinel;
			if (result->_right == n)
				return result;
		
			// otherwise find deepest level of right sibling
			return findNextLeftSubtree(result->_right);
		}

		const _node_base* levelOrderSuccessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			throw std::exception("Not yet implemented"); //TODO
		}

		const _node_base* predecessorOf(
			const _node_base* n, 
			traversal_order order
		) const {
			switch (order) {
			case collections::traversal_order::IN_ORDER:
				n = inOrderPredecessorOf(n);
				break;
			case collections::traversal_order::PRE_ORDER:
				n = preOrderPredecessorOf(n);
				break;
			case collections::traversal_order::POST_ORDER:
				n = postOrderPredecessorOf(n);
				break;
			case collections::traversal_order::LEVEL_ORDER:
				n = levelOrderPredecessorOf(n);
				break;
			}
			return n;
		}

		const _node_base* inOrderPredecessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			// if left sub-tree exists predeccessor is right-most node
			if (!isNull(n->_left)) 
				return rightMostChildOf(n->_left);
			// otherwise traverse up until the node is a right child
			// and return its parent.
			else {
				auto next = leftMostAncestorOf(n);
				return next != root() ? next->_parent : &_sentinel;
			}
		}

		const _node_base* preOrderPredecessorOf(const _node_base* n) const {
			if (isNull(n) || isNull(n->_parent))
				return &_sentinel;

			// return left sibling if it exists
			if (!isNull(n->_parent->_left) && n->_parent->_left != n)
				return n->_parent->_left;
			// else return parent
			else
				return n != root() ? n->_parent : &_sentinel;
		}

		const _node_base* postOrderPredecessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			// if a right or left child exist return the child first
			if (!isNull(n->_right))
				return n->_right;
			else if (!isNull(n->_left))
				return n->_left;
			// otherwise traverse up until the node is a right child
			// and return its left sibling.
			else {
				auto next = leftMostAncestorOf(n);
				return next != root() ? next->_parent->_left : &_sentinel;
			}
		}

		const _node_base* levelOrderPredecessorOf(const _node_base* n) const {
			if (isNull(n))
				return n;

			throw std::exception("Not yet implemented"); //TODO
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

			using node_base = BinarySearchTree::_node_base;
			using node = BinarySearchTree::node;

			const BinarySearchTree* _tree;
			node_base* _node;
			traversal_order _order;

			explicit BinaryTreeIterator(
				const BinarySearchTree* tree,
				node_base* n, 
				traversal_order order = traversal_order::IN_ORDER
			) : _tree(tree), _node(n), _order(order) {
				
			}

			void increment() {
				auto next = _tree->successorOf(_node, _order);
				next = next ? next : &_tree->_sentinel;
				_node = const_cast<node_base*>(next);
			}

			void decrement() {
				if (_tree->isNull(_node))
					_node = const_cast<node_base*>(_tree->lastNodeIn(_order));
				else {
					auto prev = _tree->predecessorOf(_node, _order);
					_node = const_cast<node_base*>(prev);
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
				return static_cast<node*>(_node)->_element;
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