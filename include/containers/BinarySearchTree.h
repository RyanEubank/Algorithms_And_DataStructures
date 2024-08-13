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
	class BinarySearchTree : public impl::BaseBST<
		element_t,
		compare_t,
		allocator_t,
		BinarySearchTree<element_t, compare_t, allocator_t>>
	{
	private:
		using tree	= BinarySearchTree<element_t, compare_t, allocator_t>;
		using base	= impl::BaseBST<element_t, compare_t, allocator_t, tree>;

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
		/// Constructs an empty BinarySearchTree.
		/// </para></summary> -------------------------------------------------
		constexpr BinarySearchTree() 
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
		/// Constructs an empty BinarySearchTree.
		/// </para></summary>
		/// <param name="alloc">
		/// The allocator instance used by the tree.
		/// </param> ----------------------------------------------------------
		constexpr explicit BinarySearchTree(const allocator_type& alloc) 
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
		BinarySearchTree(BinarySearchTree&& other)
			noexcept(std::is_nothrow_move_constructible_v<node_allocator_t>) :
			base(std::move(other)),
			_allocator(std::move(other._allocator))
		{
			
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
			this->insert(begin, end);
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
			return this->copyAssign(other);
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
			return this->moveAssign(std::move(other));
		}

	private:
		
		node_allocator_t _allocator;

		template <class... Args>
		[[nodiscard]] node* createNode(Args&&... args) {
			node* n = node_alloc_traits::allocate(_allocator, 1);
			node_alloc_traits::construct(
				_allocator, n, std::forward<Args>(args)...
			);
			return n;
		}

		void destroyNode(node* n) {
			node_alloc_traits::destroy(_allocator, n);
			node_alloc_traits::deallocate(_allocator, n, 1);
		}

		[[nodiscard]] size_type heightOfNode(const node* n) const noexcept {
			return this->heightAt(n);
		}
	
		// ---------------------------------------------------------------------
		// Basic binary search tree does no extra work after insertion, removal,
		// or searching and accessing its elements.

		iterator onInsert(node* hint, const_reference element) {
			node* result = this->insertAt(hint, element);
			return iterator(this, result); 
		}

		template <class... Args>
		iterator onEmplace(node* hint, Args&&... args) {
			node* result = this->emplaceAt(hint, std::forward<Args>(args)...);
			return iterator(this, result); 
		}

		void onRemove(node* n) {
			this->removeAt(n);
		}

		iterator onSearch(const_reference key) {
			node* n = this->search(key).get();
			return iterator(this, n);
		}

		iterator onAccessNode(node* n) {
			return iterator(this, n);
		}
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