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

#include "../../algorithms/collection_algorithms.h"
#include "../../concepts/collection.h"
#include "../../adapters/TreeTraversalAdapters.h"
#include "../../adapters/Queue.h"

namespace collections::impl {

	template <
		class element_t,
		class compare_t, 
		class allocator_t, 
		class derived_t
	>
	class BSTBase {
	protected:

		template <bool isConst>
		class BinaryTreeIterator;

		using alloc_traits = std::allocator_traits<allocator_t>;
		using node = struct _node;

	public:

		using allocator_type = allocator_t;
		using value_type = allocator_type::value_type;
		using size_type = size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = BinaryTreeIterator<false>;
		using const_iterator = BinaryTreeIterator<true>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the tree safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~BSTBase() {
			clear();
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

			while (begin != end) {
				node* n = begin++._node;
				this_derived()->destroyNode(n);
			}

			_size = 0;
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
			return _root->_element;
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
			return _min->_element;
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
			return _max->_element;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the height of the node for the given iterator.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the height of the tree node pointed to by the specified
		/// iterator position.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type heightOf(const_iterator position) const noexcept {
			return this_derived()->heightOfNode(position._node);
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
			return iterator(this, nullptr, order);
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
			return const_iterator(this, nullptr, order);
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
			return const_iterator(this, nullptr, order);
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
		/// Returns a reverse iterator to the first element according to 
		/// the reverse traversal order.
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
			const node* n = this_derived()->search(element).get();
			return iterator(this, n);
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
			return this_derived()->search(element).get() != nullptr;
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
			const node* n = this_derived()->search(element).get();
			return const_iterator(this, n);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the first element that matches the 
		/// predicate.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if an element matching the requested key is found.
		/// </returns> --------------------------------------------------------
		bool contains(const_reference element) const {
			return this_derived()->search(element).get() != nullptr;
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
			return insertAt(_root, element);
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
			return insertAt(_root, element);
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
				result = insertAt(_root, *begin++);
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
		/// Inserts the given range into the tree, with position as a 
		/// location hint.
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
				removeAt((begin++)._node);
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
			return emplaceAt(_root, arg1, std::forward<Args>(args)...);
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
			return emplaceAt(position._node, std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given binary trees.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first tree to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second tree to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(BSTBase& a, BSTBase& b) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_swap::value;
			constexpr bool alwaysEqual = 
				alloc_traits::is_always_equal::value;

			bool equalAllocators = a._allocator == b._allocator;

			if constexpr (alwaysEqual)
				a.swapData(b);
			else if (equalAllocators)
				a.swapData(b);
			else if (propagate) 
				this_derived()->swapAll(a, b);
			else
				throw std::exception("Swap on unequal, stateful allocators");
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The tree appearing on the left side of the operator.
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
		friend bool operator==(const BSTBase& lhs,const BSTBase& rhs) noexcept {
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
		/// The tree appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The tree appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given trees share exact element ordering
		/// according to in-order traversal. Always returns false for trees of 
		/// different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(const BSTBase& lhs, const BSTBase& rhs) 
			noexcept requires std::three_way_comparable<value_type> 
		{
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
			const BSTBase& tree
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
			BSTBase& tree
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
	
	protected:

		enum class Direction {
			LEFT,
			RIGHT,
			NONE
		};

		struct _node {

			value_type _element;
			_node* _parent;
			_node* _left;
			_node* _right;

			template <class ... Args>
			_node(Args&&... args) : 
				_element(std::forward<Args>(args)...),
				_parent(nullptr),
				_left(nullptr),
				_right(nullptr)
			{
			
			}

			[[nodiscard]] size_type degree() const {
				return (_left ? 1 : 0) + (_right ? 1 : 0);
			}

			[[nodiscard]] bool isLeaf() const {
				return !(_left || _right);
			}

			[[nodiscard]] bool isLeftChild() const {
				return _parent && this == _parent->_left;
			}

			[[nodiscard]] bool isRightChild() const {
				return _parent && this == _parent->_right;
			}

			[[nodiscard]] bool isChildOf(const node* n) const {
				return (this != n->_left) && (this != n->_right);
			}
		};

		struct _lookupResult {
			const node* _parent = nullptr;
			Direction _direction = Direction::NONE;

			const node* get() {
				switch (_direction) {
				case (Direction::LEFT):
					return _parent->_left;
				case (Direction::RIGHT):
					return _parent->_right;
				case (Direction::NONE):
					return _parent;
				default:
					return nullptr;
				}
			}
		};

		allocator_type _allocator;
		size_type _size;
		node* _root;
		node* _min;
		node* _max;

		constexpr BSTBase() 
			noexcept(std::is_nothrow_default_constructible_v<allocator_type>) :
			_root(nullptr),
			_min(nullptr),
			_max(nullptr),
			_size(0),
			_allocator()
		{

		}

		constexpr explicit BSTBase(const allocator_type& alloc) noexcept : 
			_root(nullptr),
			_min(nullptr),
			_max(nullptr),
			_size(0),
			_allocator(alloc)
		{

		}

		void swapData(BSTBase& other) noexcept {
			using std::swap;
			swap(_root, other._root);
			swap(_min, other._min);
			swap(_max, other._max);
			swap(_size, other._size);
		};

		node* rightRotation(node* pivot) {
			node* parent = pivot->_parent;
			node* child = pivot->_left;

			if (child->_right)
				child->_right->_parent = pivot;

			pivot->_left = child->_right;
			child->_right = pivot;	

			onRotation(pivot, child);
			return child;
		}

		node* leftRightRotation(node* pivot) {
			leftRotation(pivot->_left);
			return rightRotation(pivot);
		}

		node* leftRotation(node* pivot) {
			node* parent = pivot->_parent;
			node* child = pivot->_right;

			if (child->_left)
				child->_left->_parent = pivot;

			pivot->_right = child->_left;
			child->_left = pivot;

			onRotation(pivot, child);
			return child;
		}

		node* rightLeftRotation(node* pivot) {
			rightRotation(pivot->_right);
			return leftRotation(pivot);
		}

	private:

		[[nodiscard]] derived_t* this_derived() {
			return static_cast<derived_t*>(this);
		}

		[[nodiscard]] const derived_t* this_derived() const {
			return static_cast<const derived_t*>(this);
		}

	
		// ----------------------- INSERTION HELPERS ----------------------- //

		template <class... Args>
		iterator emplaceAt(node* hint, Args&&... args) {
			node* n = this_derived()->createNode(std::forward<Args>(args)...);
			_lookupResult lookup = getInsertLocation(hint, n->_element);
			const node* result = lookup.get();

			if (result) {
				this_derived()->destroyNode(n);
				return iterator(this, result);
			}
			else {
				insertNode(lookup, n);
				this_derived()->onInsert(n);
				return iterator(this, n);
			}

		}

		iterator insertAt(node* hint, const_reference element) {
			_lookupResult lookup = getInsertLocation(hint, element);
			const node* result = lookup.get();

			if (result)
				return iterator(this, result);
			else {
				node* n = this_derived()->createNode(element);
				insertNode(lookup, n);
				this_derived()->onInsert(n);
				return iterator(this, n);
			}
		}

		void insertNode(_lookupResult lookup, node* n) {
			node* parent = const_cast<node*>(lookup._parent);

			if (parent) {
				n->_parent = parent;

				if (lookup._direction == Direction::LEFT) {
					parent->_left = n;
					if (_min == parent)
						_min = n;
				}
				else {
					parent->_right = n;
					if (_max == parent)
						_max = n;
				}
			}
			else {
				_min = n;
				_max = n;
				_root = n;
			}

			_size++;
		}

		[[nodiscard]] _lookupResult getInsertLocation(
			const node* hint, 
			const_reference key
		) {
			if (isEmpty())
				return { nullptr, Direction::NONE };
			else if (compare(key, _min->_element))
				return { _min, Direction::LEFT };
			else if (compare(_max->_element, key))
				return { _max, Direction::RIGHT };
			else if (!hint || hint == _root) 
				return search(key);
			else if (compare(key, hint->_element))
				return checkInsertHintPredecessor(hint, key);
			else if (this->compare(hint->_element, key))
				return checkInsertHintSuccessor(hint, key);
			else
				return { hint, Direction::NONE };
		}

		[[nodiscard]] _lookupResult checkInsertHintPredecessor(
			const node* hint,
			const_reference key
		) {
			node* prev = predecessorOf(hint, traversal_order::IN_ORDER);
			if (compare(prev->_element, key)) {
				if (prev->_right)
					return { hint, Direction::LEFT };
				else
					return { prev, Direction::RIGHT };
			}
			return search(key);
		}

		[[nodiscard]] _lookupResult checkInsertHintSuccessor(
			const node* hint,
			const_reference key
		) {
			node* next = successorOf(hint, traversal_order::IN_ORDER);
			if (!next || compare(key, next->_element)) {
				if (hint->_right)
					return { next, Direction::LEFT };
				else
					return { hint, Direction::RIGHT };
			}
			return search(key);
		}

		// ----------------------- DELETION HELPERS ------------------------ //

		void removeAt(node* n) {
			if (n) {
				node* result = this->remove(n);
				this_derived()->destroyNode(n);
				this->_size--;
				this_derived()->onRemove(result);
			}
		}

		node* remove(node* n) {
			size_type degree = n->degree();

			if (degree == 0) 
				return removeDegree0(n);
			else if (degree == 1)
				return removeDegree1(n);
			else
				return removeDegree2(n);
		}

		node* removeDegree0(node* n) {
			updateLinksOnRemove(n, nullptr);
			return n->_parent;
		}

		node* removeDegree1(node* n) {
			node* replacement = n->_left ? n->_left : n->_right;
			updateLinksOnRemove(n, replacement);
			return replacement;
		}

		node* removeDegree2(node* n) {
			node* replacement = const_cast<node*>(inOrderPredecessorOf(n));
			node* result = remove(replacement);

			updateLinksOnRemove(n, replacement);

			replacement->_left = n->_left;
			if (replacement->_left)
				replacement->_left->_parent = replacement;

			replacement->_right = n->_right;
			if (replacement->_right)
				replacement->_right->_parent = replacement;

			if (result == n)
				result = replacement;

			return result;
		}

		void updateLinksOnRemove(node* n, node* replacement) {
			swapChild(n, replacement);

			if (_min == n)
				_min = replacement ? replacement : n->_parent;
			if (_max == n)
				_max = replacement ? replacement : n->_parent;
		}

		// ----------------------------- UTILS ----------------------------- //

		void swapChild(node* child, node* replacement) {
			if (child->isLeftChild())
				child->_parent->_left = replacement;
			else if (child->isRightChild())
				child->_parent->_right = replacement;
			else
				_root = replacement;

			if (replacement)
				replacement->_parent = child->_parent;
		}

		void onRotation(node* pivot, node* child) {
			swapChild(pivot, child);
			pivot->_parent = child;
		}

		[[nodiscard]] static bool compare(
			const_reference e1,
			const_reference e2
		) {
			return compare_t{}(e1, e2);
		}

		[[nodiscard]] _lookupResult search(const_reference key) {
			return std::as_const(*this).search(key);
		}

		[[nodiscard]] _lookupResult search(const_reference key) const {
			_lookupResult result = { _root, Direction::NONE };
			const node* child = result._parent;

			while (child && child->_element != key) {
				result._parent = child;

				if (compare(key, result._parent->_element)) {
					child = result._parent->_left;
					result._direction = Direction::LEFT;
				}
				else if (compare(result._parent->_element, key)) {
					child = result._parent->_right;
					result._direction = Direction::RIGHT;
				}
			}

			this_derived()->onSearch(result);
			return result;
		}

		// ----------------------- TRAVERSAL HELPERS ----------------------- //

		[[nodiscard]] static const node* leftMostChildOf(const node* n) {
			while (n->_left)
				n = n->_left;
			return n;
		}

		[[nodiscard]] static const node* rightMostChildOf(const node* n) {
			while (n->_right)
				n = n->_right;
			return n;
		}

		[[nodiscard]] static const node* leftMostAncestorOf(const node* n) {
			while (n->_parent && (!n->_parent->_left || n->isLeftChild())) 
				n = n->_parent;

			return n->_parent;
		}

		[[nodiscard]] static const node* rightMostAncestorOf(const node* n) {
			while (n->_parent && (!n->_parent->_right || n->isRightChild())) 
				n = n->_parent;

			return n->_parent;
		}

		[[nodiscard]] static const node* findNextLeftSubtree(const node* n) {
			while (n && !n->isLeaf())
				n = n->_left ? n->_left : n->_right;
			return n;
		}

		[[nodiscard]] static const node* findNextRightSubtree(const node* n) {
			while (n && !n->isLeaf())
				n = n->_right ? n->_right : n->_left;
			return n;
		}

		// ----------------------- TRAVERSAL_METHODS ----------------------- //

		[[nodiscard]] const node* firstNodeIn(traversal_order order) const {
			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return _min;
			case collections::traversal_order::PRE_ORDER:
				return _root;
			case collections::traversal_order::POST_ORDER:
				return findNextLeftSubtree(_root);
			case collections::traversal_order::LEVEL_ORDER:
				return _root;
			default:
				return nullptr;
			}
		}

		[[nodiscard]] const node* lastNodeIn(traversal_order order) const {
			switch (order) {
			case collections::traversal_order::IN_ORDER:
				return _max;
			case collections::traversal_order::PRE_ORDER:
				return findNextRightSubtree(_root);
			case collections::traversal_order::POST_ORDER:
				return _root;
			case collections::traversal_order::LEVEL_ORDER:
				return lastNodeLevelOrder();
			default:
				return nullptr;
			}
		}

		[[nodiscard]] const node* lastNodeLevelOrder() const {
			Queue<const node*> queue { _root };
			const node* n = nullptr;

			while (!queue.isEmpty()) {
				size_type count = queue.size();

				while (count--) {
					n = queue.front();

					if (n->_left)
						queue.enqueue_back(n->_left);
					if (n->_right)
						queue.enqueue_back(n->_right);

					queue.dequeue_front();
				}
			}

			return n;
		}

		[[nodiscard]] node* successorOf(const node* n, traversal_order order) {
			const node* result =  std::as_const(*this).successorOf(n, order);
			return const_cast<node*>(result);
		}

		[[nodiscard]] const node* successorOf(
			const node* n, 
			traversal_order order
		) const {
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

		[[nodiscard]] node* predecessorOf(const node* n, traversal_order order) {
			const node* result = std::as_const(*this).predecessorOf(n, order);
			return const_cast<node*>(result);
		}

		[[nodiscard]] const node* predecessorOf(
			const node* n, 
			traversal_order order
		) const {
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

		[[nodiscard]] const node* inOrderSuccessorOf(const node* n) const {
			if (!n)
				return nullptr;

			if (n->_right)
				return leftMostChildOf(n->_right);
			else if (n->_parent && n->isLeftChild())
				return n->_parent;
			else
				return rightMostAncestorOf(n);
		}

		[[nodiscard]] const node* preOrderSuccessorOf(const node* n) const {
			if (!n)
				return nullptr;

			if (n->_left)
				return n->_left;
			else if (n->_right)
				return n->_right;
			else {
				auto root = rightMostAncestorOf(n);
				return root ? root->_right : nullptr;
			}
		}

		[[nodiscard]] const node* postOrderSuccessorOf(const node* n) const {
			if (!n)
				return nullptr;

			if (!n->_parent || n->isRightChild() || n->_parent->degree() == 1)
				return n->_parent;

			return findNextLeftSubtree(n->_parent->_right);
		}

		[[nodiscard]] const node* levelOrderSuccessorOf(const node* n) const {
			if (!n)
				return nullptr;

			Queue<const node*> queue{ _root };
			const node* next = nullptr;

			while (!queue.isEmpty()) {
				size_type count = queue.size();

				while (count--) {
					next = queue.front();

					if (next->_left)
						queue.enqueue_back(next->_left);
					if (next->_right)
						queue.enqueue_back(next->_right);

					queue.dequeue_front();

					if (next == n) 
						return queue.isEmpty() ? nullptr : queue.front();
				}
			}

			return n;
		}

		[[nodiscard]] const node* inOrderPredecessorOf(const node* n) const {
			if (!n)
				return lastNodeIn(traversal_order::IN_ORDER);

			if (n->_left)
				return rightMostChildOf(n->_left);
			else if (n->_parent && n->isRightChild())
				return n->_parent;
			else
				return leftMostAncestorOf(n);
		}

		[[nodiscard]] const node* preOrderPredecessorOf(const node* n) const {
			if (!n)
				return lastNodeIn(traversal_order::PRE_ORDER);

			if (n->_parent->_left && n->isRightChild())
				return n->_parent->_left;
			else
				return n->_parent;
		}

		[[nodiscard]] const node* postOrderPredecessorOf(const node* n) const {
			if (!n)
				return lastNodeIn(traversal_order::POST_ORDER);

			if (n->_right)
				return n->_right;
			else if (n->_left)
				return n->_left;
			else {
				const node* root = leftMostAncestorOf(n);
				return root ? root->_left : nullptr;
			}
		}

		[[nodiscard]] const node* levelOrderPredecessorOf(const node* n) const {
			if (!n)
				return lastNodeIn(traversal_order::LEVEL_ORDER);
			if (n == _root)
				return nullptr;

			Queue<const node*> queue{ _root };
			Queue<const node*> reverse{};
			const node* next = nullptr;

			while (!queue.isEmpty()) {
				size_type count = queue.size();

				while (count--) {
					next = queue.front();
					queue.dequeue_front();

					if (next == n) 
						return reverse.back();

					if (next->_left)
						queue.enqueue_back(next->_left);
					if (next->_right)
						queue.enqueue_back(next->_right);
				
					reverse.enqueue_back(next);
				}
			}

			return n;
		}

		// ----------------------------------------------------------------
		/// <summary>
		/// BinaryTreeIterator is a class that implements bidirectional 
		/// iteration over a binary tree.
		/// </summary>
		///
		/// <typeparam name="isConst">
		/// Boolean whether the type is an iterator or const_iterator.
		/// </typeparam> --------------------------------------------------
		template <bool isConst>
		class BinaryTreeIterator {
		private:

			using node = BSTBase::node;

			const BSTBase* _tree;
			node* _node;
			traversal_order _order;

			explicit BinaryTreeIterator(
				const BSTBase* tree,
				const node* n, 
				traversal_order order = traversal_order::IN_ORDER
			) : 
				_tree(tree), 
				_node(const_cast<node*>(n)), 
				_order(order) 
			{

			}

			void increment() {
				const node* successor = _tree->successorOf(_node, _order);
				_node = const_cast<node*>(successor);
			}

			void decrement() {
				const node* prev = _tree->predecessorOf(_node, _order);
				_node = const_cast<node*>(prev);
			}

			friend derived_t;
			friend class BSTBase;

		public:

			using value_type = std::conditional_t<isConst, const element_t, element_t>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::bidirectional_iterator_tag;

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty BinaryTreeIterator.
			/// </para></summary> -----------------------------------------
			BinaryTreeIterator() = default;

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given BinaryTreeIterator.
			/// </para></summary> -----------------------------------------
			BinaryTreeIterator(const BinaryTreeIterator& copy) = default;

			// ------------------------------------------------------------
			/// <summary>
			/// Destructs the BinaryTreeIterator.
			///	</summary> ------------------------------------------------
			~BinaryTreeIterator() = default;

			// ------------------------------------------------------------
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
			/// </returns> ------------------------------------------------
			BinaryTreeIterator& operator=(const BinaryTreeIterator& other) 
				= default;

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given BinaryTreeIterator for 
			/// implicit conversion from non-const version to a const 
			/// BinaryTreeIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const BinaryTreeIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided BinaryTreeIterator to 
			/// copy.
			/// </typeparam> ----------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			BinaryTreeIterator(BinaryTreeIterator<wasConst> copy) : 
				BinaryTreeIterator(copy._tree, copy._node, copy._order)
			{

			}

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the 
			/// iterator in its current state.
			///	</returns> ------------------------------------------------
			reference operator*() const {
				return _node->_element;
			}

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the 
			/// iterator after updating.
			///	</returns> ------------------------------------------------
			BinaryTreeIterator& operator++() {
				increment();
				return *this;
			}

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy 
			/// of the iterator before updating.
			///	</returns> ------------------------------------------------
			BinaryTreeIterator operator++(int) {
				auto copy = *this;
				increment();
				return copy;
			}

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Decrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns the
			/// iterator after updating.
			///	</returns> ------------------------------------------------
			BinaryTreeIterator& operator--() {
				decrement();
				return *this;
			}

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Deccrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns a 
			/// copy of the iterator before updating.
			///	</returns> ------------------------------------------------
			BinaryTreeIterator operator--(int) {
				auto copy = *this;
				decrement();
				return copy;
			}

			// ------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The BinaryTreeIterator appearing on the left-hand side of 
			/// the '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The BinaryTreeIterator appearing on the right-hand side of 
			/// the '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the BinaryTreeIterator are both pointing to 
			/// the same element, false otherwise.
			///	</returns> ------------------------------------------------
			friend bool operator==(
				const BinaryTreeIterator& lhs,
				const BinaryTreeIterator& rhs
			) {
				return lhs._tree == rhs._tree && lhs._node == rhs._node;
			}
		};

		static_assert(
			std::bidirectional_iterator<iterator>,
			"BinaryTreeIterator is not a valid bidirectional iterator."
		);
	};
}
