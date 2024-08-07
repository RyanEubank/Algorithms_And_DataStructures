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

#include "base/BaseBST.h"

namespace collections {

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class allocator_t = std::allocator<element_t>
	> 
	class AVLTree : public impl::BaseBST<
		element_t, 
		compare_t, 
		allocator_t,
		AVLTree<element_t, compare_t, allocator_t>>
	{
	private:
		using tree	= AVLTree<element_t, compare_t, allocator_t>;
		using base	= impl::BaseBST<element_t, compare_t, allocator_t, tree>;

		using base_node		= base::node;
		using node			= struct avl_node;
		using alloc_traits	= base::alloc_traits;

		using node_allocator_t	= rebind<allocator_t, node>;
		using node_alloc_traits	= std::allocator_traits<node_allocator_t>;

		friend class base;

	public:
		using allocator_type			= base::allocator_type;
		using value_type				= base::value_type;
		using size_type					= base::size_type;
		using difference_type			= base::difference_type;
		using reference					= base::reference;
		using const_reference			= base::const_reference;
		using pointer					= base::pointer;
		using const_pointer				= base::const_pointer;
		using iterator					= base::iterator;
		using const_iterator			= base::const_iterator;
		using reverse_iterator			= base::reverse_iterator;
		using const_reverse_iterator	= base::const_reverse_iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty AVLTree.
		/// </para></summary> -------------------------------------------------
		constexpr AVLTree() 
			noexcept(std::is_nothrow_default_constructible_v<node_allocator_t>) :
			base(),
			_allocator(allocator_type{})
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
		constexpr explicit AVLTree(const allocator_type& alloc)
			noexcept(std::is_nothrow_copy_constructible_v<node_allocator_t>) :
			base(), 
			_allocator(alloc)
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
		AVLTree(AVLTree&& other)
			noexcept(std::is_nothrow_move_constructible_v<node_allocator_t>) :
			base(std::move(other)),
			_allocator(std::move(other._allocator))
		{

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
		/// Deep copies the data from the specified AVLTree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The AVLTree to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this BinarySearchTree with the copied data.
		/// </returns> --------------------------------------------------------
		AVLTree& operator=(const AVLTree& other) {
			return this->copyAssign(other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Moves Assignment Operator ~~~
		/// 
		/// <para>
		/// Moves the data from the specified AVLTree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The AVLTree to move from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this AVLTree with the moved data.
		/// </returns> --------------------------------------------------------
		AVLTree& operator=(AVLTree&& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			return this->moveAssign(std::move(other));
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

		node_allocator_t _allocator;

		template <class... Args>
		[[nodiscard]] node* createNode(Args&&... args) {
			node* n = node_alloc_traits::allocate(_allocator, 1);
			node_alloc_traits::construct(
				_allocator, n, std::forward<Args>(args)...
			);
			return n;
		}

		void destroyNode(base_node* n) {
			node_alloc_traits::destroy(_allocator, static_cast<node*>(n));
			node_alloc_traits::deallocate(_allocator, static_cast<node*>(n), 1);
		}

		[[nodiscard]] size_type heightOfNode(const base_node* n) const noexcept {
			return static_cast<const node*>(n)->_height;
		}

		void updateHeight(base_node* node_) {
			node* n = static_cast<node*>(node_);
			int64_t max = std::max(n->leftHeight(), n->rightHeight());
			n->_height = max + 1;
		}

		int64_t balanceOf(const base_node* node_) noexcept {
			const node* n = static_cast<const node*>(node_);
			return n->rightHeight() - n->leftHeight();
		}

		// ---------------------------------------------------------------------
		// AVL tree may need to rotate and rebalance the tree after insert and
		// delete. Does not do any work on search or element access.

		iterator onInsert(base_node* hint, const_reference element) {
			base_node* result = this->insertAt(hint, element);
			rebalanceOnInsert(result);
			return iterator(this, result); 
		}

		template <class... Args>
		iterator onEmplace(base_node* hint, Args&&... args) {
			base_node* result = this->emplaceAt(hint, std::forward<Args>(args)...);
			rebalanceOnInsert(result);
			return iterator(this, result); 
		}

		void onRemove(base_node* n) {
			n = this->removeAt(n);
			while (n) // multiple rotation mays be needed on removal
				n = rebalance(n)->_parent;
		}

		iterator onSearch(const_reference key) {
			node* n = this->search(key).get();
			return iterator(this, n);
		}

		iterator onAccessNode(base_node* n) {
			return iterator(this, n);
		}

		void rebalanceOnInsert(base_node* n) {
			while (n) {
				base_node* root = rebalance(n);
				if (root != n) // 1 rotation is enough to balance on insert
					break;
				n = root->_parent;
			}
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
				this->leftRotation(pivot->_left);
				result = this->rightRotation(pivot);
				updateHeight(result->_left);
				updateHeight(result->_right);
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
				this->rightRotation(pivot->_right);
				result = this->leftRotation(pivot);
				updateHeight(result->_left);
				updateHeight(result->_right);
				updateHeight(result);
			}
			else {
				result = this->leftRotation(pivot);
				updateHeight(result->_left);
				updateHeight(result);
			}

			return result;
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