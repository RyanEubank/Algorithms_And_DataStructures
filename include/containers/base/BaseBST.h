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

#include <concepts>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <type_traits>
#include <utility>

#include "../Node.h"
#include "../../adapters/TreeTraversalAdapters.h"
#include "../../adapters/Queue.h"
#include "../../algorithms/compare.h"
#include "../../algorithms/stream.h"
#include "../../concepts/map.h"
#include "../../util/CRTP.h"
#include "../../util/key_value_pair.h"


namespace collections::impl {

	template <
		class element_t,
		class compare_t, 
		class allocator_t,
		bool hasDuplicates,
		class derived_t
	> requires std::predicate<
		compare_t, 
		typename key_traits<element_t>::key_type, 
		typename key_traits<element_t>::key_type
	>
	class BaseBST : 
		public CRTP<
			derived_t, 
			BaseBST<element_t, compare_t, allocator_t, hasDuplicates, derived_t>
		> 
	{
	protected:

		template <bool isConst>
		class BinaryTreeIterator;

		using alloc_t		= rebind<allocator_t, element_t>;
		using alloc_traits	= std::allocator_traits<alloc_t>;

	public:

		using value_type		= element_t;
		using key_type			= key_traits<element_t>::key_type;
		using mapped_type		= key_traits<element_t>::mapped_type;

		using allocator_type	= allocator_t;
		using node_type			= Node<element_t, allocator_t, 3>;
		using size_type			= alloc_traits::size_type;
		using difference_type	= alloc_traits::difference_type;
		using pointer			= alloc_traits::pointer;
		using const_pointer		= alloc_traits::const_pointer;
		using reference			= value_type&;
		using const_reference	= const value_type&;

		using iterator					= BinaryTreeIterator<false>;
		using const_iterator			= BinaryTreeIterator<true>;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

		static constexpr bool allow_duplicates	= hasDuplicates;
		static constexpr bool is_map			= pair_type<element_t>;

	protected:

		using node_base				= node_type::base;
		using node_allocator_type	= rebind<allocator_t, node_type>;
		using node_alloc_traits		= std::allocator_traits<node_allocator_type>;
		using base_ptr				= node_type::base_ptr;
		using const_base_ptr		= node_type::const_base_ptr;
		using node_ptr				= node_type::node_ptr;
		using const_node_ptr		= node_type::const_node_ptr;

		constexpr static auto left = 0u;
		constexpr static auto right = 1u;
		constexpr static auto parent = 2u;

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the tree safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~BaseBST() {
			clear();
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
				base_ptr n = begin++._node;
				this->self().destroyNode(n);
			}

			_size = 0;
			_root = nullptr;
			_min = nullptr;
			_max = nullptr;
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

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the theoretical maximum size for the container.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the size limit of the container type.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] size_type max_size() const noexcept {
			return derived_t::node_alloc_traits::max_size(this->self()._allocator);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the root of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the element at the root of the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator root() noexcept {
			this->self().onAccessNode(_root);
			return iterator(this, _root);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the root of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const_iterator to the element at the root of the tree.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator root() const noexcept {
			return const_iterator(this, _root);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the minimum element in the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the leftmost element in the tree, or
		/// the smallest node.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator minimum() noexcept {
			this->self().onAccessNode(_min);
			return iterator(this, _min);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the minimum element in the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const_iterator to the leftmost element in the tree, or
		/// the smallest node.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator minimum() const noexcept {
			return const_iterator(this, _min);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the maximum element in the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the rightmost element in the tree, or
		/// the largest node.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator maximum() noexcept {
			this->self().onAccessNode(_max);
			return iterator(this, _max);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the maximum element in the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const_iterator to the rightmost element in the tree, or
		/// the largest node.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator maximum() const noexcept {
			return const_iterator(this, _max);
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
			return this->self().heightOfNode(position._node);
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
		/// <param name="key">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the requested element if it exists in the
		/// tree, otherwise end() is returned.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator find(key_type key) {
			TreeBoundResult lookup = lowerBound_(key);

			this->self().onAccessNode(lookup._location.parent());
			base_ptr bound = lookup.limit();

			if (bound) 
				return iterator(this, bound);
			else 
				return end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the given element.
		/// </summary>
		/// 
		/// <param name="key">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if an element matching the requested key is found.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool contains(key_type key) {
			return find(key) != end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the tree for the given element.
		/// </summary>
		/// 
		/// <param name="key">
		/// The element to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_iterator to the requested element if it exists in
		/// the tree, otherwise end() is returned. If duplicates exists it is
		/// undefined which will be returned.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator find(key_type key) const {
			TreeBoundResult lookup = lowerBound_(key);
			const_base_ptr bound = lookup._limit;

			if (bound) 
				return const_iterator(this, bound);
			else 
				return end();
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
		[[nodiscard]] bool contains(key_type key) const {
			return find(key) != end();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the first element not less than the given 
		/// key.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to get the bound for.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the requested bound if it exists, otherwise
		/// end() is returned.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] iterator lowerBound(key_type key) {
			TreeBoundResult lookup = lowerBound_(key);
			base_ptr result = lookup.limit();

			if (result)
				this->self().onAccessNode(result);
			else
				this->self().onAccessNode(_max);

			return iterator(this, result);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the first element not less than the given 
		/// key.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to get the bound for.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_iterator to the requested bound if it exists, 
		/// otherwise end() is returned.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_iterator lowerBound(key_type key) const {
			return const_iterator(this, lowerBound_(key)._limit);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the first element greater than the given 
		/// key.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to get the bound for.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the requested bound if it exists, otherwise
		/// end() is returned.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] iterator upperBound(key_type key) {
			TreeBoundResult lookup = upperBound_(key);
			base_ptr result = lookup.limit();

			if (result)
				this->self().onAccessNode(result);
			else
				this->self().onAccessNode(_max);

			return iterator(this, result);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the first element greater than the given 
		/// key.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to get the bound for.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_iterator to the requested element if it exists, 
		/// otherwise end() is returned.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const_iterator upperBound(key_type key) const {
			return const_iterator(this, upperBound_(key)._limit);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Counts the number of matching elements.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element or key to count.
		/// </param>
		/// 
		/// <returns>
		/// Returns the number of occurences of the given key.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] size_type count(key_type key) 
			const requires allow_duplicates 
		{
			return std::distance(lowerBound(key), upperBound(key)); //TODO - use custom implementation of distance.
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
			return this->self().onInsert(_root, element);
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
			return this->self().onInsert(_root, std::move(element));
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
				result = this->self().onInsert(_root, *begin++);
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
			return this->self().onInsert(position.node(), element);
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
			return this->self().onInsert(position.node(), std::move(element));
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
			auto result = iterator(this, position.node());
			while (begin != end) 
				result = this->self().onInsert(result.node(), *begin++);
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

		//TODO - implement remove by key and tests

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
				this->self().onRemove((begin++).node());
			return iterator(this, end.node());
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
			return this->self().onEmplace(
				_root, arg1, std::forward<Args>(args)...);
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
			return this->self().onEmplace(
				position.node(), std::forward<Args>(args)...);
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given trees.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first tree to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second tree to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(BaseBST& a, BaseBST& b) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			a.swap(b);
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given binary tree with this tree.
		/// </summary>
		/// 
		/// <param name="other">
		/// The tree to swap the current one with.
		/// </param> ----------------------------------------------------------
		void swap(BaseBST& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_swap::value;
			constexpr bool isAlwaysEqual = 
				alloc_traits::is_always_equal::value;
			bool isInstanceEqual = 
				this->self()._allocator == other.self()._allocator;

			if (isAlwaysEqual || isInstanceEqual)
				swapMembers(other);
			else if (willPropagate) {
				using std::swap;
				swap(this->self()._allocator, other.self()._allocator);
				swapMembers(other);
			}
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
		friend bool operator==(const BaseBST& lhs,const BaseBST& rhs) noexcept {
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
		friend auto operator<=>(const BaseBST& lhs, const BaseBST& rhs) 
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
			const BaseBST& tree
		) {
			collections::stream(tree.begin(), tree.end(), tree.size(), os);
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
		/// The tree being written to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			BaseBST& tree
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

		struct TreeLookup {
			const_base_ptr _parent = nullptr;
			Direction _direction = Direction::NONE;

			base_ptr parent() {
				return const_cast<base_ptr>(_parent);
			}
		};

		struct TreeBoundResult {
			const_base_ptr _limit = nullptr;
			TreeLookup _location = {};

			base_ptr limit() {
				return const_cast<base_ptr>(_limit);
			}
		};

		struct TreeInsertLocation {
			TreeLookup _location = {};
			bool isDuplicate = false;
		};

		// TODO make compare_t an empty member like allocator

		// TODO add constructors for tree types that take the comparison
		// function as an argument.

		size_type _size;
		base_ptr _root;
		base_ptr _min;
		base_ptr _max;

		constexpr BaseBST() noexcept :
			_root(nullptr),
			_min(nullptr),
			_max(nullptr),
			_size(0)
		{

		}

		BaseBST(BaseBST&& other) :
			_root(std::move(other._root)),
			_min(std::move(other._min)),
			_max(std::move(other._max)),
			_size(std::move(other._size))
		{
			other._root = nullptr;
			other._min = nullptr;
			other._max = nullptr;
			other._size = 0;
		}

		derived_t& copyAssign(const derived_t& other) {
			constexpr bool willPropagate = 
				alloc_traits::propagate_on_container_copy_assignment::value;
			constexpr bool isAlwaysEqual = 
				alloc_traits::is_always_equal::value;
			bool isInstanceEqual = 
				this->self()._allocator == other.self()._allocator;

			clear();

			if (!isAlwaysEqual && !isInstanceEqual && willPropagate) 
				this->self()._allocator = other.self()._allocator;

			insert(other.begin(), other.end());

			return this->self();
		}

		derived_t& moveAssign(derived_t&& other) 
			noexcept(alloc_traits::is_always_equal::value) 
		{
			constexpr bool willPropagate = 
			alloc_traits::propagate_on_container_move_assignment::value;
			constexpr bool isAlwaysEqual =
				alloc_traits::is_always_equal::value;
			bool isInstanceEqual = 
				this->self()._allocator == other.self()._allocator;

			clear();

			if (isAlwaysEqual || isInstanceEqual) 
				moveMembers(std::move(other));
			else if (willPropagate) {
				this->self()._allocator = std::move(other.self()._allocator);
				moveMembers(std::move(other));
			}
			else {
				auto begin = std::move_iterator(other.begin());
				auto end = std::move_iterator(other.end());
				insert(begin, end);
			}

			return this->self();
		}

		template <class... Args>
		base_ptr emplaceAt(base_ptr hint, Args&&... args) {
			TreeInsertLocation result;
			base_ptr n = this->self().createNode(std::forward<Args>(args)...);

			if constexpr (is_map)
				result = getInsertLocation(hint, n->value().key());
			else
				result = getInsertLocation(hint, n->value());

			if constexpr (hasDuplicates) {
				insertNode(result, n);
				return n;
			}
			else if (!result.isDuplicate) {
				insertNode(result, n);
				return n;
			}
			else {
				this->self().destroyNode(n);
				return result._location.parent();
			}
		}

		base_ptr insertAt(base_ptr hint, const_reference element) {
			TreeInsertLocation result;

			if constexpr (is_map)
				result = getInsertLocation(hint, element.key());
			else
				result = getInsertLocation(hint, element);

			if constexpr (allow_duplicates) {
				base_ptr n = this->self().createNode(element);
				insertNode(result, n);
				return n;
			}
			else if (!result.isDuplicate) {
				base_ptr n = this->self().createNode(element);
				insertNode(result, n);
				return n;
			}
			else 
				return result._location.parent();
		}

		base_ptr removeAt(base_ptr n) {
			base_ptr result = remove(n);
			this->self().destroyNode(n);
			this->_size--;
			return result;
		}

		size_type heightAt(const_base_ptr n) const noexcept {
			Queue<const_base_ptr> queue { n };
			size_type level = 0;
			size_type count = 0;

			while (!queue.isEmpty()) {
				level++;
				count = queue.size();

				while (count--) {
					const_base_ptr next = queue.front();
					queue.dequeue_front();

					if (next->to(left))
						queue.enqueue_back(next->to(left));
					if (next->to(right))
						queue.enqueue_back(next->to(right));
				}

			}

			return level - 1;
		}

		[[nodiscard]] TreeBoundResult lowerBound_(key_type key) const {
			const_base_ptr current = _root;
			const_base_ptr parent = nullptr;
			const_base_ptr bound = nullptr;
			Direction direction = Direction::NONE;

			while (current) {
				parent = current;

				if (compare(current->value(), key)) {
					current = current->to(right);
					direction = Direction::RIGHT;
				}
				else {
					direction = Direction::LEFT;
					bound = current;
					current = current->to(left);
				}
			}

			return { bound, { parent, direction } };
		}

		[[nodiscard]] TreeBoundResult upperBound_(key_type key) const {
			const_base_ptr current = _root;
			const_base_ptr parent = nullptr;
			const_base_ptr bound = nullptr;
			Direction direction = Direction::NONE;

			while (current) {
				parent = current;

				if (compare(key, current->value())) {
					bound = current;
					current = current->to(left);
					direction = Direction::LEFT;
				}
				else {
					direction = Direction::RIGHT;
					current = current->to(right);
				}
			}

			return { bound, { parent, direction } };
		}

		base_ptr leftRotation(base_ptr pivot) {
			base_ptr child = pivot->to(right);

			if (child->to(left))
				child->to(left)->to(parent) = pivot;

			pivot->to(right) = child->to(left);
			child->to(left) = pivot;

			onRotation(pivot, child);
			return child;
		}

		base_ptr rightRotation(base_ptr pivot) {
			base_ptr child = pivot->to(left);

			if (child->to(right))
				child->to(right)->to(parent) = pivot;

			pivot->to(left) = child->to(right);
			child->to(right) = pivot;	

			onRotation(pivot, child);
			return child;
		}

		[[nodiscard]] base_ptr inOrderSuccessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).inOrderSuccessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr inOrderSuccessorOf(const_base_ptr n) const {
			if (!n)
				return nullptr;

			if (n->to(right))
				return leftMostChildOf(n->to(right));
			else if (n->to(parent) && isLeftChild(n))
				return n->to(parent);
			else {
				while (isRightChild(n))
					n = n->to(parent);
				return n->to(parent);
			}
		}

		[[nodiscard]] base_ptr preOrderSuccessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).preOrderSuccessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr preOrderSuccessorOf(const_base_ptr n) const {
			if (!n)
				return nullptr;

			if (n->to(left))
				return n->to(left);
			else if (n->to(right))
				return n->to(right);
			else {
				auto root = rightMostAncestorOf(n);
				return root ? root->to(right) : nullptr;
			}
		}

		[[nodiscard]] base_ptr postOrderSuccessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).postOrderSuccessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr postOrderSuccessorOf(const_base_ptr n) const {
			if (!n)
				return nullptr;

			if (!n->to(parent) || isRightChild(n) || degree(n->to(parent)) == 1)
				return n->to(parent);

			return findNextLeftSubtree(n->to(parent)->to(right));
		}

		[[nodiscard]] base_ptr levelOrderSuccessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).levelOrderSuccessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr levelOrderSuccessorOf(const_base_ptr n) const {
			if (!n)
				return nullptr;

			Queue<const_base_ptr> queue{ _root };
			const_base_ptr next = nullptr;

			while (!queue.isEmpty()) {
				size_type count = queue.size();

				while (count--) {
					next = queue.front();

					if (next->to(left))
						queue.enqueue_back(next->to(left));
					if (next->to(right))
						queue.enqueue_back(next->to(right));

					queue.dequeue_front();

					if (next == n) 
						return queue.isEmpty() ? nullptr : queue.front();
				}
			}

			return n;
		}

		[[nodiscard]] base_ptr inOrderPredecessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).inOrderPredecessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr inOrderPredecessorOf(const_base_ptr n) const {
			if (!n)
				return lastNodeIn(traversal_order::IN_ORDER);

			if (n->to(left))
				return rightMostChildOf(n->to(left));
			else if (n->to(parent) && isRightChild(n))
				return n->to(parent);
			else {
				while (isLeftChild(n))
					n = n->to(parent);
				return n->to(parent);
			}

		}

		[[nodiscard]] base_ptr preOrderPredecessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).preOrderPredecessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr preOrderPredecessorOf(const_base_ptr n) const {
			if (!n)
				return lastNodeIn(traversal_order::PRE_ORDER);

			if (n->to(parent)->to(left) && isRightChild(n))
				return n->to(parent)->to(left);
			else
				return n->to(parent);
		}

		[[nodiscard]] base_ptr postOrderPredecessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).postOrderPredecessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr postOrderPredecessorOf(const_base_ptr n) const {
			if (!n)
				return lastNodeIn(traversal_order::POST_ORDER);

			if (n->to(right))
				return n->to(right);
			else if (n->to(left))
				return n->to(left);
			else {
				const_base_ptr root = leftMostAncestorOf(n);
				return root ? root->to(left) : nullptr;
			}
		}

		[[nodiscard]] base_ptr levelOrderPredecessorOf(const_base_ptr n) {
			const_base_ptr result = std::as_const(*this).levelOrderPredecessorOf(n);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr levelOrderPredecessorOf(const_base_ptr n) const {
			if (!n)
				return lastNodeIn(traversal_order::LEVEL_ORDER);
			if (n == _root)
				return nullptr;

			Queue<const_base_ptr> queue{ _root };
			Queue<const_base_ptr> reverse{};
			const_base_ptr next = nullptr;

			while (!queue.isEmpty()) {
				size_type count = queue.size();

				while (count--) {
					next = queue.front();
					queue.dequeue_front();

					if (next == n) 
						return reverse.back();

					if (next->to(left))
						queue.enqueue_back(next->to(left));
					if (next->to(right))
						queue.enqueue_back(next->to(right));

					reverse.enqueue_back(next);
				}
			}

			return n;
		}

		[[nodiscard]] static bool isLeftChild(const_base_ptr n) {
			return n->to(parent) && n == n->to(parent)->to(left);
		}

		[[nodiscard]] static bool isRightChild(const_base_ptr n) {
			return n->to(parent) && n == n->to(parent)->to(right);
		}

		[[nodiscard]] static size_type degree(const_base_ptr n) {
			// seperate method required because node.degree() counts parent node
			return n->degree() - (n->to(parent) ? 1 : 0);
		}

		[[nodiscard]] static bool isLeaf(const_base_ptr n) {
			return degree(n) == 0;
		}

	private:

		void moveMembers(BaseBST&& other) noexcept {
			_root = std::move(other._root);
			_min = std::move(other._min);
			_max = std::move(other._max);
			_size = std::move(other._size);
			other._root = nullptr;
			other._min = nullptr;
			other._max = nullptr;
			other._size = 0;
		};

		void swapMembers(BaseBST& other) noexcept {
			using std::swap;
			swap(_root, other._root);
			swap(_min, other._min);
			swap(_max, other._max);
			swap(_size, other._size);
		};

		// ----------------------- INSERTION HELPERS ----------------------- //

		void insertNode(TreeInsertLocation info, base_ptr n) {
			TreeLookup location = info._location;
			base_ptr insert_parent = location.parent();

			if (insert_parent) {
				n->to(parent) = insert_parent;

				if (location._direction == Direction::LEFT) {
					insert_parent->to(left) = n;
					if (insert_parent == _min)
						_min = n;
				}
				else {
					insert_parent->to(right) = n;
					if (insert_parent == _max)
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

		[[nodiscard]] TreeInsertLocation getInsertLocation(
			base_ptr hint,
			key_type key
		) {
			if (!_root)
				return {{ nullptr, Direction::NONE }, false};
			else if (compare(key, _min->value()))
				return { { _min, Direction::LEFT }, false };
			else if (!compare(_min->value(), key)) 
				return { { _min, Direction::LEFT }, true };
			else if (compare(_max->value(), key))
				return { { _max, Direction::RIGHT }, false };
			else if (!compare(key, _max->value()))
				return { lowerBound_(key)._location, true };
			else if (compare(key, hint->value()))
				return checkInsertHintPredecessor(hint, key);
			else if (compare(hint->value(), key))
				return checkInsertHintSuccessor(hint, key);
			else
				return { lowerBound_(key)._location, true };
		}

		[[nodiscard]] TreeInsertLocation checkInsertHintPredecessor(
			base_ptr hint,
			key_type key
		) {
			base_ptr prev = inOrderPredecessorOf(hint);

			if (compare(prev->value(), key)) {
				if (prev->to(right))
					return { { hint, Direction::LEFT }, false };
				else
					return { { prev, Direction::RIGHT }, false };
			}

			return findInsertBound(key);
		}

		[[nodiscard]] TreeInsertLocation checkInsertHintSuccessor(
			base_ptr hint,
			key_type key
		) {
			base_ptr next = inOrderSuccessorOf(hint);

			if (!next || compare(key, next->value())) {
				if (hint->to(right))
					return { next, Direction::LEFT };
				else
					return { hint, Direction::RIGHT };
			}

			return findInsertBound(key);
		}

		[[nodiscard]] TreeInsertLocation findInsertBound(key_type key) {
			if constexpr (allow_duplicates)
				return { lowerBound_(key)._location, true };
			else {
				TreeBoundResult bound = lowerBound_(key);
				bool isDuplicate =
					bound._limit && !compare(key, bound._limit->value());
				return { bound._location, isDuplicate };
			}
		}

		// ----------------------- DELETION HELPERS ------------------------ //

		base_ptr remove(base_ptr n) {
			size_type degree = this->degree(n);

			if (degree == 0) 
				return removeDegree0(n);
			else if (degree == 1)
				return removeDegree1(n);
			else
				return removeDegree2(n);
		}

		base_ptr removeDegree0(base_ptr n) {
			updateLinksOnRemove(n, nullptr);
			return n->to(parent);
		}

		base_ptr removeDegree1(base_ptr n) {
			base_ptr replacement = n->to(left) ? n->to(left) : n->to(right);
			updateLinksOnRemove(n, replacement);
			return replacement;
		}

		base_ptr removeDegree2(base_ptr n) {
			base_ptr replacement = const_cast<base_ptr>(inOrderPredecessorOf(n));
			base_ptr result = remove(replacement);

			updateLinksOnRemove(n, replacement);

			replacement->to(left) = n->to(left);
			if (replacement->to(left))
				replacement->to(left)->to(parent) = replacement;

			replacement->to(right) = n->to(right);
			if (replacement->to(right))
				replacement->to(right)->to(parent) = replacement;

			if (result == n)
				result = replacement;

			return result;
		}

		void updateLinksOnRemove(base_ptr n, base_ptr replacement) {
			swapChild(n, replacement);

			if (_min == n)
				_min = replacement ? replacement : n->to(parent);
			if (_max == n)
				_max = replacement ? replacement : n->to(parent);
		}

		// ----------------------------- UTILS ----------------------------- //

		void swapChild(base_ptr child, base_ptr replacement) {
			if (isLeftChild(child))
				child->to(parent)->to(left) = replacement;
			else if (isRightChild(child))
				child->to(parent)->to(right) = replacement;
			else
				_root = replacement;

			if (replacement)
				replacement->to(parent) = child->to(parent);
		}

		void onRotation(base_ptr pivot, base_ptr child) {
			swapChild(pivot, child);
			pivot->to(parent) = child;
		}

		[[nodiscard]] static bool compare(
			key_type e1, 
			key_type e2
		) {
			return compare_t{}(e1, e2);
		}

		template <class T = const_reference> 
			requires (!std::same_as<const_reference, key_type>)
		[[nodiscard]] static bool compare(
			const_reference e1, 
			key_type e2
		) {
			return compare(e1.key(), e2);
		}

		template <class T = const_reference> 
			requires (!std::same_as<const_reference, key_type>)
		[[nodiscard]] static bool compare(
			key_type e1, 
			const_reference e2
		) {
			return compare(e1, e2.key());
		}

		// ----------------------- TRAVERSAL HELPERS ----------------------- //

		[[nodiscard]] static const_base_ptr leftMostChildOf(const_base_ptr n) {
			while (n->to(left))
				n = n->to(left);
			return n;
		}

		[[nodiscard]] static const_base_ptr rightMostChildOf(const_base_ptr n) {
			while (n->to(right))
				n = n->to(right);
			return n;
		}

		[[nodiscard]] static const_base_ptr leftMostAncestorOf(const_base_ptr n) {
			while (n->to(parent) && (!n->to(parent)->to(left) || isLeftChild(n))) 
				n = n->to(parent);

			return n->to(parent);
		}

		[[nodiscard]] static const_base_ptr rightMostAncestorOf(const_base_ptr n) {
			while (n->to(parent) && (!n->to(parent)->to(right) || isRightChild(n))) 
				n = n->to(parent);

			return n->to(parent);
		}

		[[nodiscard]] static const_base_ptr findNextLeftSubtree(const_base_ptr n) {
			while (n && !isLeaf(n))
				n = n->to(left) ? n->to(left) : n->to(right);
			return n;
		}

		[[nodiscard]] static const_base_ptr findNextRightSubtree(const_base_ptr n) {
			while (n && !isLeaf(n))
				n = n->to(right) ? n->to(right) : n->to(left);
			return n;
		}

		// ----------------------- TRAVERSAL_METHODS ----------------------- //

		[[nodiscard]] base_ptr firstNodeIn(traversal_order order) {
			const_base_ptr result = std::as_const(*this).firstNodeIn(order);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr firstNodeIn(traversal_order order) const {
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

		[[nodiscard]] base_ptr lastNodeIn(traversal_order order) {
			const_base_ptr result = std::as_const(*this).lastNodeIn(order);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr lastNodeIn(traversal_order order) const {
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

		[[nodiscard]] const_base_ptr lastNodeLevelOrder() const {
			Queue<const_base_ptr> queue { _root };
			const_base_ptr n = nullptr;

			while (!queue.isEmpty()) {
				size_type count = queue.size();

				while (count--) {
					n = queue.front();

					if (n->to(left))
						queue.enqueue_back(n->to(left));
					if (n->to(right))
						queue.enqueue_back(n->to(right));

					queue.dequeue_front();
				}
			}

			return n;
		}

		[[nodiscard]] base_ptr successorOf(const_base_ptr n, traversal_order order) {
			const_base_ptr result =  std::as_const(*this).successorOf(n, order);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr successorOf(
			const_base_ptr n, 
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

		[[nodiscard]] base_ptr predecessorOf(const_base_ptr n, traversal_order order) {
			const_base_ptr result = std::as_const(*this).predecessorOf(n, order);
			return const_cast<base_ptr>(result);
		}

		[[nodiscard]] const_base_ptr predecessorOf(
			const_base_ptr n, 
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

		// ----------------------------------------------------------------
		/// <summary>
		/// BinaryTreeIterator is a class that implements bidirectional 
		/// iteration over a binary tree.
		/// </summary>
		///
		/// <typeparam name="isConst">
		/// Whether the iterator is a const_iterator (iterates over const 
		/// elements) or not.
		/// </typeparam> --------------------------------------------------
		template <bool isConst>
		class BinaryTreeIterator { //TODO extract binary tree iterator as an adpater to a node type interface so it can e adapted to other 'nodes' like heap iterators
		private:

			using pNode = std::conditional_t<isConst, const_base_ptr, base_ptr>;

			const BaseBST* _tree; // TODO make tree a const reference instead of a pointer.
			pNode _node;
			traversal_order _order;

			explicit BinaryTreeIterator(
				const BaseBST* tree,
				pNode n, 
				traversal_order order = traversal_order::IN_ORDER
			) : _tree(tree), _node(n), _order(order) {

			}

			void increment() {
				const_base_ptr successor = _tree->successorOf(_node, _order);
				_node = const_cast<base_ptr>(successor);
			}

			void decrement() {
				const_base_ptr prev = _tree->predecessorOf(_node, _order);
				_node = const_cast<base_ptr>(prev);
			}

			[[nodiscard]] constexpr base_ptr node() const {
				return const_cast<base_ptr>(_node);
			}

			friend derived_t;
			friend class BaseBST;

		public:

			// set values must be const to preserve ordering
			using value_type = std::conditional_t<
				is_map, element_t, const element_t
			>;

			using difference_type = std::ptrdiff_t;

			using pointer = std::conditional_t<
				isConst, const value_type*, value_type*
			>;

			using reference = std::conditional_t<
				isConst, const value_type&, value_type&
			>;

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
			/// Returns a reference to the element pointed to by the 
			/// iterator in its current state.
			///	</returns> ----------------------------------------------------
			reference operator*() const {
				return _node->value();
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Arrow Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a pointer to the element pointed to by the iterator in 
			/// its current state.
			///	</returns> ----------------------------------------------------
			pointer operator->() const {
				return &_node->value();
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the 
			/// iterator after updating.
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
			/// Moves the iterator to the next element and returns a copy 
			/// of the iterator before updating.
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
			/// Moves the iterator to the previous element and returns a 
			/// copy of the iterator before updating.
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
			///	</returns> ----------------------------------------------------
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
