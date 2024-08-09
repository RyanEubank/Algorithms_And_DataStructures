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
#include "StaticArray.h"
#include "DynamicArray.h"
#include "../util/CRTP.h"

namespace collections {

	template <class element_t, class allocator_t, int N>
	class Node;

	// -------------------------------------------------------------------------
	/// <summary>
	/// The base class for data structure nodes. Base nodes do not themselves
	/// store a value, they only store pointers to other nodes, or the edges in
	/// the structure.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of the element contained by the nodes pointed to.
	/// </typeparam>
	/// 
	/// <typeparam name="allocator_t">
	/// The allocator type used to define the node's pointer type using 
	/// allocator traits.
	/// </typeparam>
	/// 
	/// <typeparam name="N">
	/// The maximum number of edges allowed for the node. This template 
	/// parameter must be a non-zero value. A negative value here, (N < 0), 
	/// indicates that the node has dynamic size/number of edges.
	/// </typeparam> -----------------------------------------------------------
	template <class element_t, class allocator_t, int N>
	class NodeBase : public CRTP<
		Node<element_t, allocator_t, N>, 
		NodeBase<element_t, allocator_t, N>
	> {
	protected:
		using alloc_t			= rebind<allocator_t, element_t>;
		using alloc_traits		= std::allocator_traits<alloc_t>;
		using node_alloc_t		= rebind<allocator_t, NodeBase>;
		using node_alloc_traits = std::allocator_traits<node_alloc_t>;

	public:

		using value_type		= element_t;
		using allocator_type	= allocator_t;
		using reference			= value_type&;
		using const_reference	= const value_type&; 

		using pointer			= alloc_traits::pointer;
		using const_pointer		= alloc_traits::const_pointer;
		using node_ptr			= node_alloc_traits::pointer;
		using const_node_ptr	= node_alloc_traits::const_pointer;

	private:

		using static_array	= StaticArray<node_ptr, N>;
		using dynamic_array	= DynamicArray<node_ptr, allocator_type>;
		using edges = std::conditional_t<(N > 0), static_array, dynamic_array>;

		edges _edges;

		static_assert(N != 0, "Number of edges N must be non-zero");

	public:

		// deleted to prevent rval binding to const lval with access operations.
		constexpr const value_type&& operator*() const&& = delete;
		constexpr const value_type&& value() const&& = delete;

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the address of the value stored in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns a pointer to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr pointer operator->() noexcept { 
			return std::addressof(this->self()._value); 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the address of the value stored in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns a const_pointer to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr const_pointer operator->() const noexcept {
			return std::addressof(this->self()._value);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the value contained in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns an lvalue reference to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr reference operator*() & noexcept { 
			return this->self()._value; 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the value contained in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns a const lvalue reference to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr const_reference operator*() const& noexcept { 
			return this->self()._value; 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the value contained in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns an rvalue reference to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr value_type&& operator*() && noexcept { 
			return std::move(this->self()._value); 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the value contained in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns an lvalue reference to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr reference value() & noexcept { 
			return this->self()._value; 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the value contained in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns a const lvalue reference to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr const_reference value() const& noexcept { 
			return this->self()._value; 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the value contained in the node.
		/// </summary> 
		/// 
		/// <returns>
		/// Returns an rvalue reference to the node value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr value_type&& value() && noexcept { 
			return std::move(this->self()._value); 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the edge connecting this node to another at the given index.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the edge to access.
		/// </param>
		/// 
		/// <returns>
		/// Returns a pointer to the node at the indexed edge.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr node_ptr& to(size_t index) {
			return _edges[index];
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the edge connecting this node to another at the given index.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the edge to access.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_pointer to the node at the indexed edge.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr const node_ptr& to(size_t index) const {
			return _edges[index];
		}
	};
	
	// -------------------------------------------------------------------------
	/// <summary>
	/// Type for data structure nodes that hold actual values. The node value
	/// is not initialized on construction, and can be deffered until later.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of the element contained by the nodes pointed to.
	/// </typeparam>
	/// 
	/// <typeparam name="allocator_t">
	/// The allocator type used to define the node's pointer type using 
	/// allocator traits. Can be used to construct the value via the node's
	/// construct method. Node themselves do not store an instance of the
	/// allocator.
	/// </typeparam>
	/// 
	/// <typeparam name="N">
	/// The maximum number of edges allowed for the node. This template 
	/// parameter must be a non-zero value. A negative value here, (N < 0), 
	/// indicates that the node has dynamic size/number of edges.
	/// </typeparam> -----------------------------------------------------------
	template <class element_t, class allocator_t, int N>
	class Node : public NodeBase<element_t, allocator_t, N> {
	public:
		using base = NodeBase<element_t, allocator_t, N>;

	private:
		using alloc_traits	= base::alloc_traits;

	public:
		using value_type		= base::value_type;
		using allocator_type	= base::allocator_type;
		using reference			= base::reference;
		using const_reference	= base::const_reference;
		using pointer			= base::node_ptr;
		using const_pointer		= base::const_node_ptr;
		using node_ptr			= base::node_ptr;
		using const_node_ptr	= base::const_node_ptr;

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs node without initializing its value. 
		/// </summary> ---------------------------------------------------------
		constexpr Node() noexcept {}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs node and its value in place. 
		/// </summary>
		/// 
		/// <param name="tag">
		/// Disambiguation tag to for constructing nodes and values in place.
		/// </param>
		/// 
		/// <param name="args">
		/// The arguments to construct the value with.
		/// </param> -----------------------------------------------------------
		template <class ...Args> 
			requires std::constructible_from<value_type, Args...>
		constexpr Node(std::in_place_t tag,  Args&& ...args) 
			noexcept(std::is_nothrow_constructible_v<value_type, Args...>) :
			_value(std::forward<Args>(args)...)
		{
		
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs in place with optional overload for constructing objects
		/// with an initializer list. Constructs the allocator with the
		/// provided reference.
		/// </summary>
		/// 
		/// <param name="tag">
		/// Disambiguation tag to for constructing nodes and values in place.
		/// </param>
		/// 
		/// <param name="list">
		/// The initializer list to construct the value with.
		/// </param>
		/// 
		/// <param name="args">
		/// Any remaining arguments to construct the value with.
		/// </param> -----------------------------------------------------------
		template <class T, class ...Args> requires std::constructible_from<
			value_type,
			std::initializer_list<T>, 
			Args...
		>
		constexpr Node(
			std::in_place_t,
			std::initializer_list<T> list, 
			Args&& ...args
		) noexcept(std::is_nothrow_constructible_v<
			value_type, 
			std::initializer_list<T>&, 
			Args...>
		) : _value(list, std::forward<Args>(args)...) {
		
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constrained destructor for trivially destructible values.
		/// </summary> ---------------------------------------------------------
		constexpr ~Node() requires(std::is_trivially_destructible_v<value_type>) 
			= default;

		// ---------------------------------------------------------------------
		/// <summary>
		/// Destructor for all non trivial types.
		/// </summary> ---------------------------------------------------------
		constexpr ~Node() {}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs the node value in place.
		/// </summary>
		///
		/// <param name="alloc">
		/// The allocator instance used to construct the value with.
		/// </param>
		/// 
		/// <param name="args">
		/// The arguments to construct the value with.
		/// </param> -----------------------------------------------------------
		template <class ...Args> 
			requires std::constructible_from<value_type, Args...>
		constexpr reference construct(
			const allocator_type& alloc, 
			Args&& ...args
		) noexcept(std::is_nothrow_constructible_v<value_type, Args...>) {
			alloc_traits::construct(
				alloc, 
				std::addressof(_value),
				std::forward<Args>(args)...
			);
			return _value;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Optional overload to construct the node value in place with an
		/// initializer list.
		/// </summary>
		///
		/// <param name="list">
		/// The initializer list to construct the value with.
		/// </param>
		/// 
		/// <param name="alloc">
		/// The allocator instance used to construct the value with.
		/// </param>
		/// 
		/// <param name="args">
		/// The arguments to construct the value with.
		/// </param> -----------------------------------------------------------
		template <class T, class ...Args> requires std::constructible_from<
			value_type, 
			std::initializer_list<T>,
			Args...
		>
		constexpr reference construct(
			std::initializer_list<T> list,
			const allocator_type& alloc,
			Args&& ...args
		) noexcept(std::is_nothrow_constructible_v<
			value_type, 
			std::initializer_list<T>, 
			Args...>
		) {
			alloc_traits::construct(
				alloc, 
				std::addressof(_value),
				list,
				std::forward<Args>(args)...
			);
			return _value;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Destroys the value contained by the node. Accessing the node value
		/// after destruction is undefined behavior.
		/// </summary> 
		/// 
		/// <param name="alloc">
		/// The allocator instance used to deconstruct the value with.
		/// </param> -----------------------------------------------------------
		constexpr void destroy(const allocator_type& alloc) 
			noexcept(std::is_nothrow_destructible_v<value_type>) 
		{
			if constexpr (!std::is_trivially_destructible_v<value_type>) 
				alloc_traits::destroy(alloc, std::addressof(_value));
		}

	private:

		union {
			[[no_unique_address, msvc::no_unique_address]] value_type _value;
		};

		friend base;
	};
}

