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

#include <utility>
#include "base/BaseBST.h"

namespace collections {

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class allocator_t = std::allocator<element_t>
	>
	class SplayTree : public impl::BaseBST<
		element_t,
		compare_t,
		allocator_t,
		SplayTree<element_t, compare_t, allocator_t>>
	{
	private:
		using tree = SplayTree<element_t, compare_t, allocator_t>;
		using base = impl::BaseBST<element_t, compare_t, allocator_t, tree>;

		using node					= base::node;
		using alloc_traits			= base::alloc_traits;
		using node_allocator_t		= rebind<allocator_t, node>;
		using node_alloc_traits		= std::allocator_traits<node_allocator_t>;

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
		/// Constructs an empty SplayTree.
		/// </para></summary> -------------------------------------------------
		constexpr SplayTree() 
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
		/// Constructs an empty SplayTree.
		/// </para></summary>
		/// <param name="alloc">
		/// The allocator instance used by the tree.
		/// </param> ----------------------------------------------------------
		constexpr explicit SplayTree(const allocator_type& alloc) 
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
		/// Constructs a deep copy of the specified SplayTree.
		/// </para></summary> 
		/// 
		/// <param name="copy">
		/// The SplayTree to be copied.
		/// </param> ----------------------------------------------------------
		SplayTree(const SplayTree& copy) : SplayTree(
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
		/// Constructs a SplayTree by moving the data from the provided 
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The SplayTree to be moved into this one.
		/// </param> ----------------------------------------------------------
		SplayTree(SplayTree&& other)
			noexcept(std::is_nothrow_move_constructible_v<node_allocator_t>) :
			base(std::move(other)),
			_allocator(std::move(other._allocator))
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a SplayTree with the a copy of the elements in
		/// the specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		SplayTree(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : SplayTree(init.begin(), init.end(), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a SplayTree with the a copy of the elements from 
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
		SplayTree(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : SplayTree(alloc) {
			this->insert(begin, end);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a SplayTree array with a copy of the elements
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
		SplayTree(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : SplayTree(
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
		/// Deep copies the data from the specified SplayTree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The SplayTree to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this SplayTree with the copied data.
		/// </returns> --------------------------------------------------------
		SplayTree& operator=(const SplayTree& other) {
			return this->copyAssign(other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Moves Assignment Operator ~~~
		/// 
		/// <para>
		/// Moves the data from the specified SplayTree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The SplayTree to move from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this SplayTree with the moved data.
		/// </returns> --------------------------------------------------------
		SplayTree& operator=(SplayTree&& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			return this->moveAssign(std::move(other));
		}

	private:

		node_allocator_t _allocator;

		[[nodiscard]] node* allocate() {
			return node_alloc_traits::allocate(_allocator, 1);
		}

		void deallocate(node* n) {
			node_alloc_traits::deallocate(_allocator, n, 1);
		}

		template <class... Args>
		void constructNode(node* n, Args&&... args) {
			node_alloc_traits::construct(
				_allocator, 
				n, 
				std::forward<Args>(args)...
			);
		}

		void destroyNode(node* n) {
			node_alloc_traits::destroy(_allocator, n);
		}

		template <class... Args>
		[[nodiscard]] node* createNode(Args&&... args) {
			node* n = allocate();
			constructNode(n, std::forward<Args>(args)...);
			return n;
		}

		[[nodiscard]] size_type heightOfNode(const node* n) const noexcept {
			return this->heightAt(n);
		}

		// ---------------------------------------------------------------------
		// Splay tree will rotate accessed nodes to the root on all operations -
		// insert, delete, and search (unless const to preserve contract).

		iterator onInsert(node* hint, const_reference element) {
			node* result = this->insertAt(hint, element);
			splay(result);
			return iterator(this, result); 
		}

		template <class... Args>
		iterator onEmplace(node* hint, Args&&... args) {
			node* result = this->emplaceAt(hint, std::forward<Args>(args)...);
			splay(result);
			return iterator(this, result); 
		}

		void onRemove(node* n) {
			node* result = this->removeAt(n);
			splay(result);
		}

		iterator onSearch(const_reference key) {
			struct base::_lookupResult result = this->search(key);
			node* n = result.get();
			n ? splay(n) : splay(result._lastAccessed);
			return iterator(this, n);
		}

		iterator onAccessNode(node* n) {
			splay(n);
			return iterator(this, n);
		}

		void splay(node* n) {
			if (n) {
				while (n->_parent)
					rotateUp(n);
			}
		}

		void rotateUp(node* n) {
			if (n->_parent == this->_root) {
				if (n->isLeftChild()) 
					this->rightRotation(n->_parent);
				else
					this->leftRotation(n->_parent);
			}
			else if (n->isLeftChild()){
				if (n->_parent->isLeftChild()) {
					this->rightRotation(n->_parent->_parent);
					this->rightRotation(n->_parent);
				}
				else
					this->rightLeftRotation(n->_parent->_parent);
			}
			else {
				if (n->_parent->isRightChild()) {
					this->leftRotation(n->_parent->_parent);
					this->leftRotation(n->_parent);
				}
				else
					this->leftRightRotation(n->_parent->_parent);
			}
		}
	};

	static_assert(
		associative_collection<SplayTree<int>>,
		"SplayTree does not implement the associative collection interface."
	);

	static_assert(
		bidirectional_collection<SplayTree<int>>,
		"SplayTree does not meet the requirements for a bidirectional collection."
	);

	static_assert(
		ordered_collection<SplayTree<int>>,
		"SplayTree does not meet the requirements for an ordered collection."
	);
}