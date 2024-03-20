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

#include "../interfaces/IStack.h"
#include "../containers/DynamicArray.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Stack is a container adapter that implements the IStack interface
	/// from any container that supports list operations from the ListConcept
	/// interface.
	/// </summary>
	/// 
	/// <typeparam name="element_t">
	/// The type of elements contained by the stack.
	/// </typeparam>
	/// <typeparam name="container">
	/// The type of the underlying container used by the stack.
	/// </typeparam> ----------------------------------------------------------
	template <class element_t, class container = DynamicArray<element_t>>
		requires ListConcept<container, element_t>
	class Stack : public IStack<Stack<element_t, container>, element_t> {
	private:

		using E = element_t;

		container m_container;

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty stack with zero size.
		/// </para></summary> -------------------------------------------------
		Stack() : m_container() {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a stack with the a copy of the elements in the specified
		/// initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		Stack(std::initializer_list<E> init) :
			Stack(init.begin(), init.size()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a stack with the a copy of the elements in the specified
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
		Stack(const E* array, size_t size) : m_container(array, size) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Virtual destructor allowing for deleting derived types from base.
		/// </para></summary> -------------------------------------------------
		~Stack() = default;

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the size of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of elements contained by the stack as a size_t.
		/// </returns>
		size_t size() const {
			return m_container.size();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Empties and clears the stack of all elements.
		/// </summary> --------------------------------------------------------
		void clear() {
			m_container.clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the stack is empty: contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the stack has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		bool isEmpty() const {
			return size() == 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the given element is contained by the given stack.
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
		/// Returns true if the stack contains the element, false otherwise.
		/// </returns> --------------------------------------------------------
		bool contains(const element_t& element) const {
			return m_container.contains(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element at the top of the stack.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param> ----------------------------------------------------------
		void push(const E& element) {
			m_container.insertLast(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the front of the stack.
		/// </summary> --------------------------------------------------------
		void pop() {
			m_container.removeLast();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the element at the top of the stack.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the element at the top of the stack.
		/// </returns> --------------------------------------------------------
		E& peek() {
			const E& element = std::as_const(*this).peek();
			return const_cast<E&>(element);
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
		const E& peek() const {
			return m_container.last();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Transfers the contents of the calling stack to the specified stack
		/// in bottom to top (FIFO) order. The calling stack will be empty
		/// after this method completes.
		/// </summary>
		/// 
		/// <param name="other">
		/// The other stack to transfer this stack's contents to.
		/// </param> ----------------------------------------------------------
		void placeOnTopOf(Stack<E>& other) {
			for (auto& e : m_container)
				other.push(e);
			clear();
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
		/// Returns true if the given Stack's underlying containers are equal,
		/// false otherwise.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const Stack<E, container>& lhs,
			const Stack<E, container>& rhs
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
		/// Returns the output stream written with the given Stack.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const Stack<E, container>& stack
		) {
			os << "Stack<" << typeid(E).name() << ", "
				<< typeid(container).name() << ">"
				<< " (size = " << stack.size() << ")" << std::endl;

			stack.m_container.writeToStream(os);
			return os;
		}
	};
}
