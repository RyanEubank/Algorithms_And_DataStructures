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

#include "BinarySearchTree.h"

namespace collections {

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class allocator_t = std::allocator<element_t>
	> 
	class AVLTree : public impl::BSTBase<
		element_t, 
		compare_t, 
		allocator_t,
		AVLTree<element_t, compare_t, allocator_t>
	> {
	private:
		using avl = AVLTree<element_t, compare_t, allocator_t>;
		using base = impl::BSTBase<element_t, compare_t, allocator_t, avl>;
		using base_node = base::base_node;
		using node = struct avl_node;
		using alloc_traits = std::allocator_traits<allocator_t>::template rebind_traits<node>;
		using node_allocator_type = alloc_traits::allocator_type;

		friend class base;

	public:
		using allocator_type = allocator_t;
		using value_type = base::value_type;
		using size_type = base::size_type;
		using reference = base::reference;
		using const_reference = base::const_reference;
		using pointer = std::allocator_traits<allocator_t>::pointer;
		using const_pointer = std::allocator_traits<allocator_t>::const_pointer;
		using iterator = base::iterator;
		using const_iterator = base::const_iterator;
		using reverse_iterator = base::reverse_iterator;
		using const_reverse_iterator = base::const_reverse_iterator;	

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty AVLTree.
		/// </para></summary> -------------------------------------------------
		constexpr AVLTree() : 
			base(),
			_node_allocator(allocator_type{})
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Allocator Constructor ---
		/// 
		///	<para>
		/// Constructs an empty AVLTree.
		/// </para></summary>
		/// <param name="alloc">
		/// The allocator instance used by the tree.
		/// </param> ----------------------------------------------------------
		constexpr explicit AVLTree(const allocator_type& alloc) noexcept : 
			base(alloc), 
			_node_allocator(alloc)
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		/// <para>
		/// Constructs a deep copy of the specified AVLTree.
		/// </para></summary> 
		/// 
		/// <param name="copy">
		/// The AVLTree to be copied.
		/// </param> ----------------------------------------------------------
		AVLTree(const AVLTree& copy) : AVLTree(
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
		/// Constructs a AVLTree by moving the data from the provided 
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The AVLTree to be moved into this one.
		/// </param> ----------------------------------------------------------
		AVLTree(AVLTree&& other) noexcept(
			std::is_nothrow_move_constructible_v<allocator_type>
		) : AVLTree(std::move(other._allocator)) {
			swapData(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a AVLTree with the a copy of the elements in
		/// the specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		AVLTree(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : AVLTree(init.begin(), init.end(), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a AVLTree with the a copy of the elements from 
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
		AVLTree(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : AVLTree(alloc) {
			this->insert(begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a AVLTree array with a copy of the elements
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
		AVLTree(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : AVLTree(
			std::ranges::begin(r), 
			std::ranges::end(r), 
			alloc
		) {

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
		AVLTree& operator=(const AVLTree& other) {
			constexpr bool propagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;
			constexpr bool alwaysEqual = 
				alloc_traits::is_always_equal::value;

			bool equalAllocators = this->_allocator == other._allocator;

			this->clear();

			if (propagate && !equalAllocators) {
				this->_allocator = other._allocator;
				_node_allocator = other._node_allocator;
			}

			this->insert(other.begin(), other.end());

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
		AVLTree& operator=(AVLTree&& other) 
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
				this->clear();
				auto begin = std::move_iterator(other.begin());
				auto end = std::move_iterator(other.end());
				this->insert(begin, end);
			}

			return *this;
		}

	private:

		struct avl_node : base::_node {
			size_type _height = 0;

			int64_t leftHeight() const {
				return this->_left 
					? static_cast<const node*>(this->_left)->_height 
					: -1;
			}

			int64_t rightHeight() const {
				return this->_right 
					? static_cast<const node*>(this->_right)->_height 
					: -1;
			}
		};

		node_allocator_type _node_allocator;

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

		void destroyNode(base_node* n) {
			alloc_traits::destroy(_node_allocator, static_cast<node*>(n));
		}

		template <class... Args>
		[[nodiscard]] node* createNode(Args&&... args) {
			node* n = allocate();
			constructNode(n, std::forward<Args>(args)...);
			return n;
		}

		void updateHeight(base_node* n) {
			node* avl_n = static_cast<node*>(n);
			int64_t max = std::max(avl_n->leftHeight(), avl_n->rightHeight());
			avl_n->_height = max + 1;
		}

		int64_t balanceOf(const base_node* n) noexcept {
			const node* avl_n = static_cast<const node*>(n);
			return avl_n->rightHeight() - avl_n->leftHeight();
		}

		size_type heightOfNode(const base_node* n) const noexcept {
			return static_cast<const node*>(n)->_height;
		}

		template <class... Args>
		iterator insertAt(base_node* hint, Args&&... args) {
			size_type prev_size = this->size();
			node* child = createNode(std::forward<Args>(args)...);
			const base_node* result = this->tryInsert(hint, child);

			if (this->size() > prev_size) 
				balanceTreeOnInsert(child->_parent);

			return iterator(this, result);
		}

		void balanceTreeOnInsert(base_node* n) {
			while (n) {
				base_node* root = rebalance(n);
				if (root != n) // rotation occurred, tree is rebalanced
					break;
				n = root->_parent;
			}
		}

		void removeAt(base_node* n) {
			if (n) {
				base_node* replacement = this->remove(n);
				balanceTreeOnRemove(replacement);
				destroyNode(n);
				this->_size--;
			}
		}

		void balanceTreeOnRemove(base_node* n) {
			while (n) 
				n = rebalance(n)->_parent;
		}

		base_node* rebalance(base_node* n) {
			int64_t balance = balanceOf(n);

			if (balance < -1)
				n = rebalanceLeftChild(n);
			else if (balance > 1)
				n = rebalanceRightChild(n);
			else 
				updateHeight(n);

			return n;
		}

		base_node* rebalanceLeftChild(base_node* pivot) {
			base_node* result = nullptr;

			if (balanceOf(pivot->_left) == 1) {
				result = this->leftRightRotation(pivot);
				updateHeight(result->_right);
				updateHeight(result->_left);
				updateHeight(result);
			}
			else {
				result = this->rightRotation(pivot);
				updateHeight(result->_right);
				updateHeight(result);
			}

			return result;
		}

		base_node* rebalanceRightChild(base_node* pivot) {
			base_node* result = nullptr;

			if (balanceOf(pivot->_right) == -1) {
				result = this->rightLeftRotation(pivot);
				updateHeight(result->_right);
				updateHeight(result->_left);
				updateHeight(result);
			}
			else {
				result = this->leftRotation(pivot);
				updateHeight(result->_left);
				updateHeight(result);
			}

			return result;
		}

		friend void swapAll(AVLTree& a, AVLTree& b) noexcept {
			using std::swap;
			swap(a._allocator, b._allocator);
			swap(a._node_allocator, b._node_allocator);
			swapData(a, b);
		}	
	};

	static_assert(
		associative_collection<AVLTree<int>>,
		"AVLTree does not implement the associative collection interface."
	);

	static_assert(
		bidirectional_collection<AVLTree<int>>,
		"AVLTree does not meet the requirements for a bidirectional collection."
	);

	static_assert(
		ordered_collection<AVLTree<int>>,
		"AVLTree does not meet the requirements for an ordered collection."
	);
}