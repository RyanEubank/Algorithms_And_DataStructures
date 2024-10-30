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

#include <initializer_list>
#include <istream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <utility>

#include "../concepts/collection.h"
#include "../concepts/sequential.h"
#include "../containers/DynamicArray.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Stack is a container adapter that implements a stack interfance
	/// from any sequential container.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of elements contained by the stack.
	/// </typeparam>
	/// <typeparam name="container">
	/// The type of the underlying container used by the stack.
	/// </typeparam> ----------------------------------------------------------
	template <
		class element_t, 
		sequential container_t = DynamicArray<element_t>
	>
	class Stack {
	public:

		using container			= container_t;
		using allocator_type	= container::allocator_type;
		using value_type		= container::value_type;
		using size_type			= container::size_type;
		using difference_type	= container::difference_type;
		using reference			= container::reference;
		using const_reference	= container::const_reference;
		using pointer			= container::pointer;
		using const_pointer		= container::const_pointer;

		// All stack iterators are constant to preserve stack ordering. 
		// Iterator order is reversed to start from the 'top' of the stack.
		using iterator					= container::const_reverse_iterator;
		using const_iterator			= container::const_reverse_iterator;
		using reverse_iterator			= container::const_iterator;
		using const_reverse_iterator	= container::const_iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty stack, and default constructs its internal
		/// container.
		/// </para></summary> -------------------------------------------------
		constexpr Stack() : _container() {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Container Constructor ~~~
		/// 
		///	<para>
		/// Constructs a stack from a copy of the provided container.
		/// </para></summary> 
		/// 
		/// <param name="c">
		/// The container to copy.
		/// </param> ----------------------------------------------------------
		explicit Stack(const container& c) : _container(c) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Container Constructor ~~~
		/// 
		///	<para>
		/// Constructs a stack by moving the provided container.
		/// </para></summary> 
		/// 
		/// <param name="c">
		/// The container to move.
		/// </param> ----------------------------------------------------------
		explicit Stack(container&& c) : _container(std::move(c)) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Allocator Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty stack passing the given allocator to the 
		/// stack's internal container.
		/// </para></summary> 
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the internal container.
		/// </param> ----------------------------------------------------------
		explicit Stack(const allocator_type& alloc) noexcept :
			_container(alloc)
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a stack with the a copy of the elements in the specified
		/// initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		Stack(std::initializer_list<value_type> init) :
			Stack(init.begin(), init.end()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a stack with the a copy of the elements from the
		/// given iterator/sentinel pair.
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
		/// The beginning of the range to copy from.
		/// </param>
		/// <param name="end">
		/// The end of the range to copy from.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator in_iterator, 
			std::sentinel_for<in_iterator> sentinel
		>
		Stack(
			in_iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : _container(begin, end, alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a stack with a copy of the elements from the given 
		/// range.
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="tag">
		/// Range construction tag to disabiguate this constructor from
		/// construction with an initializer list.
		/// </param>
		/// <param name="rg">
		/// The range to construct the stack with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range range>
		Stack(
			from_range_t tag,
			range&& rg,
			const allocator_type& alloc = allocator_type{}
		) : Stack(std::ranges::begin(rg), std::ranges::end(rg), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fill Constructor ~~~
		/// 
		/// <para>
		/// Constructs a stack with the specified size, constucting 
		/// elements with the given value for each (or default) and uses the 
		/// given allocator for the internal container (or default allocator).
		/// </para></summary>
		/// 
		/// <param name="size">
		/// The initial size of the stack.
		/// </param>
		/// <param name="value">
		/// The value to initialize every element to. Uses default value for
		/// the type if unspecified.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> ----------------------------------------------------------
		Stack(
			Size size,
			const_reference value = value_type{},
			const allocator_type& alloc = allocator_type{}
		) : _container(size, value, alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the size of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of elements contained by the stack.
		/// </returns>
		[[nodiscard]] size_type size() const {
			return _container.size();
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
			return _container.max_size();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the stack of all elements.
		/// </summary> --------------------------------------------------------
		void clear() {
			_container.clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the stack is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the stack has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const {
			return _container.isEmpty();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the top of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the top element in the 
		/// stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() noexcept {
			return _container.rbegin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the location after the 
		/// bottom element in the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() noexcept {
			return _container.rend();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the top element in 
		/// the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const noexcept {
			return _container.rbegin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the location after the 
		/// bottom element in the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const noexcept {
			return _container.rend();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the top of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the top element in 
		/// the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const_iterator pointing to the end of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the location after the 
		/// bottom element in the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the first element in 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the bottom of the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reverse_iterator rbegin() noexcept {
			return _container.begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the element after the 
		/// end of the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the element after the 
		/// top of stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reverse_iterator rend() noexcept {
			return _container.end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the first element in 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the bottom of the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept {
			return _container.begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the element after the 
		/// end of the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the element after the 
		/// top of stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator rend() const noexcept {
			return _container.end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the first element in 
		/// the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the bottom of the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a const reverse iterator pointing to the element after the 
		/// end of the reverse sequence.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reverse iterator to the element after the 
		/// top of stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reverse_iterator crend() const noexcept {
			return rend();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element at the top of the stack.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void push(const_reference element) {
			_container.insertBack(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the top of the stack.
		/// </summary> --------------------------------------------------------
		void pop() {
			_container.removeBack();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the top of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the element at the top of the stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference peek() {
			const_reference element = std::as_const(*this).peek();
			return const_cast<reference>(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the top of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the top of the 
		/// stack.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference peek() const {
			return _container.back();
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given Stacks.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first stack to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second stack to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(Stack& a, Stack& b) 
			noexcept(noexcept(swap(_container, _container))) 
		{
			swap(a._container, b._container);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The stack appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The stack appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given stack's underlying containers are equal,
		/// false otherwise.
		/// </returns> --------------------------------------------------------
		friend bool operator==(const Stack& lhs, const Stack& rhs) {
			return lhs._container == rhs._container;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The stack appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The stack appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given stacks share exact ordering based
		/// on contents. Always returns false for arrays of different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(const Stack& lhs, const Stack& rhs) {
			return lhs._container <=> rhs._container;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Output Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="charT">
		/// The type of the character stream returned by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being written to.
		/// </param>
		/// <param name="arr">
		/// The stack being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const Stack& stack
		) {
			os << stack._container;
			return os;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Input Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="charT">
		/// The type of the character stream returned by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being read from.
		/// </param>
		/// <param name="arr">
		/// The stack being read into.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_istream<charT>& operator>>(
			std::basic_istream<charT>& is,
			Stack& stack
		) {
			is >> stack._container;
			return is;
		}

	private:
		container _container;
	};

	static_assert(
		collection<Stack<int>>,
		"Stack does not implement the collection interface."
	);
}
