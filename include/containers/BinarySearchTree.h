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

#include "../concepts/collection.h"

namespace collections {

	template <class element_t, class allocator_t = std::allocator<element_t>>
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
		constexpr BinarySearchTree() noexcept(noexcept(allocator_type{})) {
			
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
		constexpr explicit BinarySearchTree(const allocator_type& alloc) noexcept {
			
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
		) : BinarySearchTree(
			std::ranges::begin(r), 
			std::ranges::end(r), 
			alloc
		) {

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
		/// Returns an iterator pointing to the root of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a bidirectional iterator to the first element in the 
		/// tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() noexcept {
			return BinaryTreeIterator<false>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a bidirectional iterator to the location after the last 
		/// element in the tree according to in-order traversal.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() noexcept {
			return BinaryTreeIterator<false>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the root of the 
		/// tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant bidirectional iterator to the first element in 
		/// the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const noexcept {
			return BinaryTreeIterator<true>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the end of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant bidirectional iterator to the location after  
		/// the last element in the tree according to in-order traversal.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const noexcept {
			return BinaryTreeIterator<true>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the root of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant bidirectional iterator to the first element in 
		/// the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return BinaryTreeIterator<true>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the end of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant bidirectional iterator to the location after  
		/// the last element in the tree according to in-order traversal.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return BinaryTreeIterator<true>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a reverse iterator pointing to the first element in the 
		/// reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reverse iterator to the element before the beginning of 
		/// the tree.
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
		/// the tree.
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
		/// the tree.
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
		/// beginning of the tree.
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
		/// the tree.
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
		/// beginning of the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crend() const noexcept {
			return std::make_reverse_iterator(begin());
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
		/// Returns an iterator to the requested element if it exists in the
		/// tree, otherwise end() is returned.
		/// </returns>
		iterator find(const_reference element) {
			return end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the first element that matches the predicate.
		/// </summary>
		/// 
		/// <param name="element">
		/// The predicate to compare elements against.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the first element matching the given 
		/// predicate, or the end() iterator if none exist.
		/// </returns>
		template <class T> requires std::predicate<T, const_reference>
		iterator find_if(T predicate) {
			return end();
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
		/// Returns a const_iterator to the requested element if it exists in the
		/// tree, otherwise end() is returned.
		/// </returns>
		const_iterator find(const_reference element) const {
			return end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the first element that matches the predicate.
		/// </summary>
		/// 
		/// <param name="element">
		/// The predicate to compare elements against.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_iterator to the first element matching the given 
		/// predicate, or the end() iterator if none exist.
		/// </returns>
		template <class T> requires std::predicate<T, const_reference>
		const_iterator find_if(T predicate) const {
			return end();
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
			return end();
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
			return end();
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
		/// Returns an iterator to the first element inserted, or the first
		/// element that prevented insertion. Returns the end iterator of the
		/// tree if begin == end.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		iterator insert(in_iterator begin, sentinel end) {
			return (*this).end();
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
			return end();
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
			return end();
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
		/// Returns an iterator to the first element inserted, or the first 
		/// element preventing insertion.
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
			return (*this).end();
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
			return end();
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
			return (*this).end();
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
		template <class ...Args>
		iterator emplace(Args&&... args) {
			return end();
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
			return end();
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
			collections::stream(tree, os);
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
			BinarySearchTree& list
		) {
			return is;
		}

	private:

		struct _node {
			struct _node* _left;
			struct _node* _right;
			struct _node* _parent;
			value_type _element;
		};

		_node* _root;
		allocator_type _allocator;
		node_allocator_type _node_allocator;
		size_type _size;

		// --------------------------------------------------------------------
		/// <summary>
		/// BinaryTreeIterator is a class that implements bidirectional 
		/// iteration over a binary tree.
		/// </summary>
		///
		/// <typeparam name="element_t">
		/// The type of the elements iterated over by the BinaryTreeIterator.
		/// </typeparam> ------------------------------------------------------
		template <bool isConst>
		class BinaryTreeIterator {
		private:

			using node = BinarySearchTree::node;
			node* _node;

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a BinaryTreeIterator pointing to the specified node 
			/// in the tree.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> ------------------------------------------------------
			explicit BinaryTreeIterator(node* node) : _node(node) {}

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
			BinaryTreeIterator(BinaryTreeIterator<wasConst> copy)
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
				_node = _node->_left;
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
				_node = _node->_left;
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
				_node = _node->_parent;
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
				_node = _node->_parent;
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
}