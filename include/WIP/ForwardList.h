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

#include "BaseList.h"
#include "../util/CollectionUtils.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// ForwardList is a container class that maintains a single linked list for 
	/// fast insertion/deletion at the front and implements the IList interface.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the ForwardList.
	/// </typeparam> ----------------------------------------------------------
	template <class element_t>
	class ForwardList final :
		public BaseList<ForwardList<element_t>, element_t>
	{
	private:

		struct _Node {
			element_t m_element;
			struct _Node* m_next;

			_Node(const element_t& element) :
				m_element(element),
				m_next(nullptr)
			{}
		};

		using Node = struct _Node;
		using E = element_t;

		Node* m_head;

		const Node* getNodePreviousTo(size_t index) const {
			Node* target = m_head;
			size_t currentIndex = 0;

			while (currentIndex++ < index - 1)
				target = target->m_next;

			return target;
		}

		Node* getNodePreviousTo(size_t index) {
			const Node* node = (std::as_const(*this).getNodePreviousTo(index));
			return const_cast<Node*>(node);
		}

		template <std::forward_iterator T>
		void copyList(T start, T end) {
			m_head = new Node(*(start++));
			Node* tail = m_head;

			while (start != end) {
				tail->m_next = new Node(*start++);
				tail = tail->m_next;
			}
		}

	public:

		template <bool isConst>
		class ForwardListIterator;

		using iterator = ForwardListIterator<false>;
		using const_iterator = ForwardListIterator<true>;

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ForwardList.
		/// </para></summary> -------------------------------------------------
		ForwardList() : m_head(nullptr) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		/// <para>
		/// Constructs a deep copy of the specified ForwardList.
		/// </para></summary> 
		/// 
		/// <param name="copy">
		/// The ForwardList to be copied.
		/// </param> ----------------------------------------------------------
		ForwardList(const ForwardList& copy) : ForwardList() {
			copyList(copy.begin(), copy.end());
			this->m_size = copy.m_size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Move Constructor ---
		/// 
		/// <para>
		/// Constructs a ForwardList by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ForwardList to be moved into this one.
		/// </param> ----------------------------------------------------------
		ForwardList(ForwardList&& other) noexcept : ForwardList() {
			swap(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a ForwardList with the a copy of the elements in the 
		/// specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		ForwardList(std::initializer_list<E> init) :
			ForwardList(init.begin(), init.size()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a ForwardList with the a copy of the elements in the 
		/// specified array.
		/// </summary>
		/// 
		/// <param name="array">
		/// The generic array to copy elements from.
		/// </param>
		/// 
		/// <param name="size">
		/// The size of the array being copied.
		/// </param> ----------------------------------------------------------
		ForwardList(const E* array, size_t size) : ForwardList() {
			copyList(array, array + size);
			this->m_size = size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the ForwardList safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~ForwardList() {
			clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified ForwardList to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ForwardList to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this ForwardList with the copied data.
		/// </returns> --------------------------------------------------------
		ForwardList& operator=(ForwardList other) {
			swap(*this, other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Index Operator ---
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// ForwardList.
		/// </returns> --------------------------------------------------------
		E& operator[](size_t index) {
			const E& element = std::as_const(*this)[index];
			return const_cast<E&>(element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// --- Index Operator ---
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const reference to the element at the specified index 
		/// in the ForwardList.
		/// </returns> --------------------------------------------------------
		const E& operator[](size_t index) const {
			this->validateIndex(index);

			if (index == 0)
				return m_head->m_element;

			const Node* previous = getNodePreviousTo(index);
			return previous->m_next->m_element;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes and deletes all nodes from the ForwardList emptying its 
		/// contents.
		/// </summary> --------------------------------------------------------
		void clear() {
			while (m_head) {
				Node* next = m_head->m_next;
				delete m_head;
				m_head = next;
			}
			m_head = nullptr;
			this->m_size = 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the ForwardList at the given index.
		/// Throws an exception if the index in invalid.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted.
		/// </param>
		/// 
		/// <param name="index">
		/// The index to insert the element at.
		/// </param> ----------------------------------------------------------
		void insertAt(const E& element, size_t index) {
			this->validateInsert(index);
			Node* newNode = new Node(element);

			if (this->isEmpty()) 
				m_head = newNode;
			else if (index == 0) {
				newNode->m_next = m_head;
				m_head = newNode;
			}
			else {
				Node* previous = getNodePreviousTo(index);
				newNode->m_next = previous->m_next;
				previous->m_next = newNode;
			}

			this->m_size++;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element at the given index. Throws an exception if the 
		/// index in invalid or ForwardList is empty.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to be removed.
		/// </param> ----------------------------------------------------------
		void removeAt(size_t index) {
			this->validateIndex(index);

			if (index == 0) {
				Node* next = m_head->m_next;
				delete m_head;
				m_head = next;
			}
			else {
				Node* previous = getNodePreviousTo(index);
				Node* temp = previous->m_next;
				previous->m_next = temp->m_next;
				delete temp;
			}

			this->m_size--;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the ForwardList.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the first element in the 
		/// ForwardList.
		/// </returns> --------------------------------------------------------
		iterator begin() {
			return ForwardListIterator<false>(m_head);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the ForwardList.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the location after the last 
		/// element in the ForwardList.
		/// </returns> --------------------------------------------------------
		iterator end() {
			return ForwardListIterator<false>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the 
		/// ForwardList.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element in 
		/// the ForwardList.
		/// </returns> --------------------------------------------------------
		const_iterator begin() const {
			return ForwardListIterator<true>(m_head);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the ForwardList.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the location after  
		/// the last element in the ForwardList.
		/// </returns> --------------------------------------------------------
		const_iterator end() const {
			return ForwardListIterator<true>();
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given ForwardLists.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first list to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second list to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(ForwardList<E>& a, ForwardList<E>& b) noexcept {
			using std::swap;
			swap(a.m_size, b.m_size);
			swap(a.m_head, b.m_head);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The ForwardList appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The ForwardList appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given ForwardLists share deep equality based
		/// on contents and size.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const ForwardList<E>& lhs,
			const ForwardList<E>& rhs
		) {
			return collections::
				isLexicographicallyEqual<ForwardList<E>>(rhs, lhs);
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
		/// The ForwardList being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream written with the given ForwardList.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const ForwardList<E>& list
		) {
			os << "Forward List<" << typeid(E).name() << ">"
				<< " (size = " << list.size() << ")"
				<< std::endl;

			list.writeToStream(os);
			return os;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ForwardListIterator is a class that implements forward iteration
		/// over a ForwardList.
		/// </summary>
		///
		/// <typeparam name="element_t">
		/// The type of the elements iterated over by the ForwardListIterator.
		/// </typeparam> ------------------------------------------------------
		template <bool isConst>
		class ForwardListIterator {
		private:

			using E = element_t;
			using Node = ForwardList<E>::Node;

			Node* m_current;

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a ForwardListIterator pointing to the specified node 
			/// in a ForwardList.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> ------------------------------------------------------
			ForwardListIterator(Node* node) : m_current(node) {}

			friend class ForwardList<E>;

		public:

			using value_type = std::conditional_t<isConst, const E, E>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty ForwardListIterator.
			/// </para></summary> ---------------------------------------------
			ForwardListIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given ForwardListIterator.
			/// </para></summary> ---------------------------------------------
			ForwardListIterator(const ForwardListIterator& copy) = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// Destructs the ForwardListIterator.
			///	</summary> ----------------------------------------------------
			~ForwardListIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			/// Sets the left side forward list equal to the right side.
			/// </para></summary>
			///
			/// <param name="other">
			/// The ForwardListIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this ForwardListIterator with the copied data.
			/// </returns> ----------------------------------------------------
			ForwardListIterator& operator=(const ForwardListIterator& other) 
				= default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given ForwardListIterator for implicit 
			/// conversion from non-const version to a const 
			/// ForwardListIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const ForwardListIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided ForwardListIterator to copy.
			/// </typeparam> --------------------------------------------------
			template <
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>>
			ForwardListIterator(ForwardListIterator<wasConst> copy)
				: m_current(copy.m_current) {}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the iterator
			/// in its current state.
			///	</returns> ----------------------------------------------------
			reference operator*() const {
				return m_current->m_element;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> ----------------------------------------------------
			ForwardListIterator& operator++() {
				m_current = m_current->m_next;
				return *this;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy of
			/// the iterator before updating.
			///	</returns> ----------------------------------------------------
			ForwardListIterator operator++(int) {
				auto copy = *this;
				m_current = m_current->m_next;
				return copy;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The ForwardListIterator appearing on the left-hand side of the 
			/// '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The ForwardListIterator appearing on the right-hand side of the 
			/// '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the ForwardListIterators are both pointing to
			/// the same element, false otherwise.
			///	</returns> ----------------------------------------------------
			friend bool operator==(
				const ForwardListIterator& lhs,
				const ForwardListIterator& rhs
			) {
				return lhs.m_current == rhs.m_current;
			}
		};
	};
}