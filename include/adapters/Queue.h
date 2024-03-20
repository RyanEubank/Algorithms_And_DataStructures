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

#include "../interfaces/IQueue.h"
#include "../containers/LinkedList.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Queue is a container adapter that implements the IQueue interface
	/// from any container that supports list operations from the ListConcept
	/// interface.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of elements contained by the queue.
	/// </typeparam>
	/// <typeparam name="container">
	/// The type of the underlying container used by the queue.
	/// </typeparam> ----------------------------------------------------------
	template <class element_t, class container = LinkedList<element_t>>
		requires ListConcept<container, element_t>
	class Queue : IQueue<Queue<element_t, container>, element_t> {
	private:

		using E = element_t;

		container m_container;

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty queue with zero size.
		/// </para></summary> -------------------------------------------------
		Queue() : m_container() {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a Queue with the a copy of the elements in the specified
		/// initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		Queue(std::initializer_list<E> init) 
			: Queue(init.begin(), init.size()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a Queue with the a copy of the elements in the specified
		/// array.
		/// </summary>
		/// 
		/// <param name="array">
		/// The generic array to copy elements from.
		/// </param>
		/// 
		/// <param name="size">
		/// The size of the array being copied.
		/// </param> ----------------------------------------------------------
		Queue(const E* array, size_t size) : m_container(array, size) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Virtual destructor allowing for deleting derived types from base.
		/// </para></summary> -------------------------------------------------
		~Queue() = default;

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the size of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of elements contained by the queue as a size_t.
		/// </returns>
		size_t size() const {
			return m_container.size();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the queue of all elements.
		/// </summary> --------------------------------------------------------
		void clear() {
			m_container.clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the queue is empty: contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the queue has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		bool isEmpty() const {
			return size() == 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the given element is contained by the given queue.
		/// </summary>
		/// 
		/// <param name="collection">
		/// The queue to be searched.
		/// </param>
		/// <param name="element">
		/// The element to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the queue contains the element, false otherwise.
		/// </returns> --------------------------------------------------------
		bool contains(const element_t & element) const {
			return m_container.contains(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element at the front of the queue.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void enqueue(const E& element) {
			m_container.insertLast(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the front of the queue.
		/// </summary> --------------------------------------------------------
		void dequeue() {
			m_container.removeFirst();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the front of the queue.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the element at the front of the queue.
		/// </returns> --------------------------------------------------------
		E& peek() {
			const E& element = std::as_const(*this).peek();
			return const_cast<E&>(element);
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
		const E& peek() const {
			return m_container.first();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The Queue appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The Queue appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given Queue's underlying containers are equal,
		/// false otherwise.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const Queue<E, container>& lhs, 
			const Queue<E, container>& rhs
		) {
			return collections::isLexicographicallyEqual(
				lhs.m_container, rhs.m_container
			);
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
		/// The Queue being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream written with the given Queue.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const Queue<E, container>& queue
		) {
			os << "Queue<" << typeid(E).name() << ", " 
				<< typeid(container).name() << ">"
				<< " (size = " << queue.size() << ")" << std::endl;

			queue.m_container.writeToStream(os);
			return os;
		}
	};
}
