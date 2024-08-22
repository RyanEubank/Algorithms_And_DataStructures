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

#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

#include "base/BaseBST.h"
#include "../concepts/collection.h"

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
		using tree		= SplayTree<element_t, compare_t, allocator_t>;
		using base_tree = impl::BaseBST<element_t, compare_t, allocator_t, tree>;

		using alloc_traits			= base_tree::alloc_traits;
		using node_allocator_type	= base_tree::node_allocator_type;
		using node_alloc_traits		= base_tree::node_alloc_traits;
		using base_ptr				= base_tree::base_ptr;
		using const_base_ptr		= base_tree::const_base_ptr;
		using node_ptr				= base_tree::node_ptr;
		using const_node_ptr		= base_tree::const_node_ptr;

		friend class base_tree;

		constexpr static auto left		= base_tree::left;
		constexpr static auto right		= base_tree::right;
		constexpr static auto parent	= base_tree::parent;

	public:
		using allocator_type			= base_tree::allocator_type;
		using value_type				= base_tree::value_type;
		using node_type					= base_tree::node_type;
		using size_type					= base_tree::size_type;
		using difference_type			= base_tree::difference_type;
		using reference					= base_tree::reference;
		using const_reference			= base_tree::const_reference;
		using pointer					= base_tree::pointer;
		using const_pointer				= base_tree::const_pointer;
		using iterator					= base_tree::iterator;
		using const_iterator			= base_tree::const_iterator;
		using reverse_iterator			= base_tree::reverse_iterator;
		using const_reverse_iterator	= base_tree::const_reverse_iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty SplayTree.
		/// </para></summary> -------------------------------------------------
		constexpr SplayTree() 
			noexcept(std::is_nothrow_default_constructible_v<node_allocator_type>) :
			base_tree(),
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
			noexcept(std::is_nothrow_copy_constructible_v<node_allocator_type>) :
			base_tree(), 
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
			noexcept(std::is_nothrow_move_constructible_v<node_allocator_type>) :
			base_tree(std::move(other)),
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
		
		[[no_unique_address, msvc::no_unique_address]]
		node_allocator_type _allocator;

		template <class... Args>
		[[nodiscard]] node_ptr createNode(Args&&... args) {
			node_ptr n = node_alloc_traits::allocate(_allocator, 1);
			node_alloc_traits::construct(
				_allocator, n, std::in_place_t{}, std::forward<Args>(args)...
			);
			return n;
		}

		void destroyNode(base_ptr n) {
			node_alloc_traits::destroy(_allocator, static_cast<node_ptr>(n));
			node_alloc_traits::deallocate(_allocator, static_cast<node_ptr>(n), 1);
		}

		[[nodiscard]] size_type heightOfNode(const_base_ptr n) const noexcept {
			return this->heightAt(n);
		}

		// ---------------------------------------------------------------------
		// Splay tree will rotate accessed nodes to the root on all operations -
		// insert, delete, and search (unless const to preserve contract).

		iterator onInsert(base_ptr hint, const_reference element) {
			base_ptr result = this->insertAt(hint, element);
			splay(result);
			return iterator(this, result); 
		}

		template <class... Args>
		iterator onEmplace(base_ptr hint, Args&&... args) {
			base_ptr result = this->emplaceAt(hint, std::forward<Args>(args)...);
			splay(result);
			return iterator(this, result); 
		}

		void onRemove(base_ptr n) {
			if (n == this->_min)
				this->_min = this->inOrderSuccessorOf(n);
			if (n == this->_max)
				this->_max = this->inOrderPredecessorOf(n);

			std::pair<base_ptr, base_ptr> subtrees = split(n);
			join(subtrees);
			destroyNode(n);
			this->_size--;
		}

		void onAccessNode(base_ptr n) {
			splay(n);
		}

		void splay(base_ptr n) {
			if (n) {
				while (n->to(parent))
					rotateUp(n);
			}
		}

		void rotateUp(base_ptr n) {
			base_ptr parent_ptr = n->to(parent);
			base_ptr grandparent = parent_ptr ? parent_ptr->to(parent) : nullptr;

			if (this->isLeftChild(n)) {
				if (!grandparent)
					this->rightRotation(parent_ptr);
				else if (this->isLeftChild(parent_ptr)) {
					this->rightRotation(grandparent);
					this->rightRotation(parent_ptr);
				}
				else {
					this->rightRotation(parent_ptr);
					this->leftRotation(grandparent);
				}
			}
			else {
				if (!grandparent)
					this->leftRotation(parent_ptr);
				else if (this->isRightChild(parent_ptr)) {
					this->leftRotation(grandparent);
					this->leftRotation(parent_ptr);
				}
				else {
					this->leftRotation(parent_ptr);
					this->rightRotation(grandparent);
				}
			}
		}

		const std::pair<base_ptr, base_ptr> split(base_ptr n) {
			base_ptr predecessor = this->inOrderPredecessorOf(n);
			splay(n);

			if (n->to(left)) 
				n->to(left)->to(parent) = nullptr;
			if (n->to(right))
				n->to(right)->to(parent) = nullptr;

			this->_root = predecessor;
			return { predecessor, n->to(right) };
		}

		void join(const std::pair<base_ptr, base_ptr>& subtrees) {
			if (subtrees.first) {
				splay(subtrees.first);
				subtrees.first->to(right) = subtrees.second;
				if (subtrees.second)
					subtrees.second->to(parent) = subtrees.first;
			}
			else
				this->_root = subtrees.second;
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