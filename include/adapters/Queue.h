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
#include "../containers/LinkedList.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Queue is a container adapter that implements a queue interfance
	/// from any sequential container.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of elements contained by the queue.
	/// </typeparam>
	/// <typeparam name="container">
	/// The type of the underlying container used by the queue.
	/// </typeparam> ----------------------------------------------------------
	template <
		class element_t, 
		sequential_collection container_t = LinkedList<element_t>
	>
	class Queue {
	public:

		using container			= container_t;
		using allocator_type	= typename container::allocator_type;
		using value_type		= typename container::value_type;
		using size_type			= typename container::size_type;
		using reference			= typename container::reference;
		using const_reference	= typename container::const_reference;
		using pointer			= typename container::pointer;
		using const_pointer		= typename container::const_pointer;

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty queue, and default constructs its internal
		/// container.
		/// </para></summary> -------------------------------------------------
		constexpr Queue() : _container() {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Container Constructor ~~~
		/// 
		///	<para>
		/// Constructs a queue from a copy of the provided container.
		/// </para></summary> 
		/// 
		/// <param name="c">
		/// The container to copy.
		/// </param> ----------------------------------------------------------
		explicit Queue(const container& c) : _container(c) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Container Constructor ~~~
		/// 
		///	<para>
		/// Constructs a queue by moving the provided container.
		/// </para></summary> 
		/// 
		/// <param name="c">
		/// The container to move.
		/// </param> ----------------------------------------------------------
		explicit Queue(container&& c) : _container(std::move(c)) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Allocator Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty queue passing the given allocator to the 
		/// queue's internal container.
		/// </para></summary> 
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the internal container.
		/// </param> ----------------------------------------------------------
		explicit Queue(const allocator_type& alloc) noexcept :
			_container(alloc)
		{

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a queue with the a copy of the elements in the specified
		/// initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		Queue(std::initializer_list<value_type> init) :
			Queue(init.begin(), init.end()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a queue with the a copy of the elements from the
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
			std::input_iterator iterator, 
			std::sentinel_for<iterator> sentinel
		>
		Queue(
			iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : _container(begin, end, alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a queue with a copy of the elements from the given 
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
		Queue(
			from_range_t tag,
			range&& rg,
			const allocator_type& alloc = allocator_type{}
		) : Queue(std::ranges::begin(rg), std::ranges::end(rg), alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fill Constructor ~~~
		/// 
		/// <para>
		/// Constructs a queue with the specified size, constucting 
		/// elements with the given value for each (or default) and uses the 
		/// given allocator for the internal container (or default allocator).
		/// </para></summary>
		/// 
		/// <param name="size">
		/// The initial size of the queue.
		/// </param>
		/// <param name="value">
		/// The value to initialize every element to. Uses default value for
		/// the type if unspecified.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the internal container. Default 
		/// constructs the allocator if unspecified.
		/// </param> ----------------------------------------------------------
		Queue(
			Size size,
			const_reference value = value_type{},
			const allocator_type& alloc = allocator_type{}
		) : _container(size, value, alloc) {

		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the size of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of elements contained by the queue.
		/// </returns>
		[[nodiscard]] size_type size() const {
			return _container.size();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the queue of all elements.
		/// </summary> --------------------------------------------------------
		void clear() {
			_container.clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the queue is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the queue has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const {
			return _container.isEmpty();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element at the front of the queue.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void enqueue_front(const_reference element) {
			_container.insertFront(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the front of the queue.
		/// </summary> --------------------------------------------------------
		void dequeue_front() {
			_container.removeFront();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the front of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the element at the front of the queue.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference front() {
			const_reference element = std::as_const(*this).front();
			return const_cast<reference>(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the front of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the front of the 
		/// queue.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference front() const {
			return _container.front();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element at the back of the queue.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void enqueue_back(const_reference element) {
			_container.insertBack(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the back of the queue.
		/// </summary> --------------------------------------------------------
		void dequeue_back() {
			_container.removeBack();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the back of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the element at the back of the queue.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] reference back() {
			const_reference element = std::as_const(*this).back();
			return const_cast<reference>(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the back of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the back of the 
		/// queue.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_reference back() const {
			return _container.back();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The queue appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The queue appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given queue's underlying containers are equal,
		/// false otherwise.
		/// </returns> --------------------------------------------------------
		friend bool operator==(const Queue& lhs, const Queue& rhs) {
			return lhs._container == rhs._container;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The queue appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The queue appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given queues share exact ordering based
		/// on contents. Always returns false for arrays of different size.
		/// </returns> --------------------------------------------------------
		friend auto operator<=>(const Queue& lhs, const Queue& rhs) {
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
		/// The queue being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const Queue& queue
		) {
			os << queue._container;
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
		/// The queue being read into.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_istream<charT>& operator>>(
			std::basic_istream<charT>& is,
			Queue& queue
		) {
			is >> queue._container;
			return is;
		}

	private:
		container _container;
	};

	static_assert(
		collection<Queue<int>>,
		"Queue does not implement the collection interface."
	);
}
