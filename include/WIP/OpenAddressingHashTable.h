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

#include "BaseHashTable.h"
#include "../util/ArrayUtils.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// OpenAddressingHashTable is a container class that implements a hash 
	/// table structure using open addressing for hash collision resolution. 
	/// It inherits from BaseHashTable to implement the IHashTable interface.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the HashTable.
	/// </typeparam> ----------------------------------------------------------
	template <class element_t>
	class OpenAddressingHashTable final : public BaseHashTable<
		OpenAddressingHashTable<element_t>, element_t>
	{
	private:

		using base = BaseHashTable<
			OpenAddressingHashTable<element_t>, element_t>;
		using E = element_t;
		using HashFunction = base::HashFunction;

		Node* m_array;

	public:

		template <bool isConst>
		class OpenAddressingTableIterator;

		using iterator = OpenAddressingTableIterator<false>;
		using const_iterator = OpenAddressingTableIterator<true>;

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty OpenAddressingHashTable with the specified 
		/// capacity, or default capacity if not provided.
		/// </para></summary>
		/// 
		/// <param name="capacity">
		/// The initial capacity to allocate to the hash table.
		/// </param> 
		/// <param name="func">
		/// The hash function to use for hashing elements.
		/// </param> ----------------------------------------------------------
		OpenAddressingHashTable(
			size_t capacity = base::DEFAULT_CAPACITY,
			HashFunction func = std::hash<E>()
		) : base(capacity, func), m_array(nullptr) {
			this->validateResize(capacity);
			m_array = new Node[this->m_capacity];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Constructor ~~~
		/// 
		/// <para>
		/// Constructs a deep copy of the specified OpenAddressingHashTable.
		/// </para></summary>
		/// 
		/// <param name="copy">
		/// The OpenAddressingHashTable to be copied.
		/// </param> ----------------------------------------------------------
		OpenAddressingHashTable(const OpenAddressingHashTable& copy) :
			OpenAddressingHashTable(copy.m_capacity, copy.m_hashFunc) 
		{
			collections::copyArray(copy.m_array, m_array, this->m_capacity);
			this->m_size = copy.m_size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Constructor ~~~
		/// 
		/// <para>
		/// Constructs a OpenAddressingHashTable by moving the data from the 
		/// provided object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The OpenAddressingHashTable to be moved into this one.
		/// </param> ----------------------------------------------------------
		OpenAddressingHashTable(OpenAddressingHashTable&& other) noexcept :
			OpenAddressingHashTable()
		{
			swap(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a OpenAddressingHashTable with the a copy of the 
		/// elements in the specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> 
		/// <param name="capacity">
		/// The capacity of the internal array for the table.
		/// </param>
		/// <param name="func">
		/// The hash function to use for hashing elements.
		/// </param> ----------------------------------------------------------
		OpenAddressingHashTable(
			std::initializer_list<E> init,
			size_t capacity = base::DEFAULT_CAPACITY,
			HashFunction func = std::hash<E>()
		) : OpenAddressingHashTable(
			init.begin(), init.size(), capacity, func) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a OpenAddressingHashTable with the a copy of the 
		/// elements in the specified array.
		/// </summary>
		/// 
		/// <param name="array">
		/// The generic array to copy elements from.
		/// </param>
		/// <param name="size">
		/// The size of the array being copied.
		/// </param>
		/// <param name="capacity">
		/// The capacity of the internal array for the table.
		/// </param>
		/// <param name="func">
		/// The hash function to use for hashing elements.
		/// </param> ----------------------------------------------------------
		OpenAddressingHashTable(
			const E* array,
			size_t size,
			size_t capacity = base::DEFAULT_CAPACITY,
			HashFunction func = std::hash<E>()
		) : OpenAddressingHashTable(capacity, func) {
			for (const E* e = array; e != array + size; e++)
				this->insert(*e);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the OpenAddressingHashTable safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~OpenAddressingHashTable() {
			delete[] m_array;
			m_array = nullptr;
			this->m_size = 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified OpenAddressingHashTable 
		/// to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The OpenAddressingHashTable to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this OpenAddressingHashTable with the copied data.
		/// </returns> --------------------------------------------------------
		OpenAddressingHashTable& operator=(OpenAddressingHashTable other) {
			swap(*this, other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Clears the hash table of all its elements but maintains its current
		/// size.
		/// </summary> --------------------------------------------------------
		void clear() {
			for (size_t i = 0; i < this->m_capacity; i++)
				m_array[i].m_isOccupied = false;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the hash table, if possible. Throws
		/// an exception if hash collision cannot be resolved.
		/// </summary>
		/// 
		/// <param name="element">
		/// - The element to be inserted.
		/// </param> ----------------------------------------------------------
		void insert(const E& element) {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element from the has table if it exists.
		/// </summary>
		/// 
		/// <param name="element">
		/// - The element to be removed.
		/// </param> ----------------------------------------------------------
		void remove(const E& element) {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the hash table contains the specified element.
		/// </summary>
		/// 
		/// <param name="element">
		/// - The element to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the element is found, false otherwise.
		/// </returns> --------------------------------------------------------
		bool contains(const E& element) const {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Resizes the hash table and rehashes its existing elements adding
		/// them back to the newly sized table.
		/// </summary>
		/// 
		/// <param name="capacity">
		/// - The new capacity of the hash table.
		/// </param> ----------------------------------------------------------
		void resize(size_t capacity) {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the first element in the 
		/// hash table.
		/// </returns> --------------------------------------------------------
		iterator begin() {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the location after the last 
		/// element in the hash table.
		/// </returns> --------------------------------------------------------
		iterator end() {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the 
		/// hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element in 
		/// the hash table.
		/// </returns> --------------------------------------------------------
		const_iterator begin() const {
			
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the location after the 
		/// last element in the hash table.
		/// </returns> --------------------------------------------------------
		const_iterator end() const {
			
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given hash tables.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first table to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second table to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(
			OpenAddressingHashTable<E>& a,
			OpenAddressingHashTable<E>& b
		) noexcept {
			using std::swap;
			swap(a.m_capacity, b.m_capacity);
			swap(a.m_size, b.m_size);
			swap(a.m_array, b.m_array);
			a.rehash(a.m_hashFunc);
			b.rehash(b.m_hashFunc);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The hash table appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The hash table appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given tables share deep equality based
		/// on capacity/size and have equivalent sets of elements.
		/// </returns> --------------------------------------------------------
		friend bool operator==(
			const OpenAddressingHashTable<E>& lhs,
			const OpenAddressingHashTable<E>& rhs
		) {
			bool isCapacityEqual = (lhs.m_capacity == rhs.m_capacity);
			bool isSizeEqual = (lhs.m_size == rhs.m_size);

			if (isCapacityEqual && isSizeEqual) {
				for (const auto& e : lhs) {
					if (!rhs.contains(e))
						return false;
				}
				return true;
			}

			return false;
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
		/// The hash table being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream written with the given hash table.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const OpenAddressingHashTable<E>& table
			) {
			os << "OpenAddressingHashTable<" << typeid(E).name() << ">"
				<< " (size = " << table.size() << ", "
				<< "capacity = " << table.capacity() << ", "
				<< "loadFactor = " << table.loadFactor() << ")"
				<< std::endl;

			table.writeToStream(os);
			return os;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// OpenAddressingTableIterator is a class that implements forward 
		/// iteration over an open addressing-type hash table.
		/// </summary>
		///
		/// <typeparam name="element_t">
		/// The type of the elements iterated over by the 
		/// OpenAddressingTableIterator.
		/// </typeparam> ------------------------------------------------------
		template <bool isConst>
		class OpenAddressingTableIterator {
		private:

			using E = element_t;

			Node* m_array;
			size_t m_index;

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a OpenAddressingTableIterator pointing to the 
			/// start of the specified array.
			///	</summary>
			///
			/// <param name="node">
			/// The array the iterator will point to.
			/// </param> ------------------------------------------------------
			explicit OpenAddressingTableIterator(Node* arr)
				: m_array(arr), m_index(0)
			{
				advanceToNextValidSlot();
			}

			void advanceToNextValidSlot() {
				while (!m_array[m_index++].m_isOccupied) {}
			}

			friend class OpenAddressingHashTable<E>;

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
			/// Constructs an empty OpenAddressingTableIterator.
			/// </para></summary> ---------------------------------------------
			OpenAddressingTableIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given OpenAddressingTableIterator.
			/// </para></summary> ---------------------------------------------
			OpenAddressingTableIterator(const OpenAddressingTableIterator& copy)
				= default;

			// ----------------------------------------------------------------
			/// <summary>
			/// Destructs the ChainingHashTableIterator.
			///	</summary> ----------------------------------------------------
			~OpenAddressingTableIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The OpenAddressingTableIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this OpenAddressingTableIterator with the copied data.
			/// </returns> ----------------------------------------------------
			OpenAddressingTableIterator& operator=(
				const OpenAddressingTableIterator& other
			) = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given OpenAddressingTableIterator for 
			/// implicit conversion from non-const version to a const 
			/// OpenAddressingTableIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const OpenAddressingTableIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided OpenAddressingTableIterator to 
			/// copy.
			/// </typeparam> --------------------------------------------------
			template<
				bool wasConst,
				class = std::enable_if_t<isConst && !wasConst>>
			OpenAddressingTableIterator(
				OpenAddressingTableIterator<wasConst> copy
			) : m_array(copy.m_array) {}

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
				return m_array[m_index].m_value;
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
			OpenAddressingTableIterator& operator++() {
				advanceToNextValidSlot();
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
			OpenAddressingTableIterator operator++(int) {
				auto copy = *this;
				advanceToNextValidSlot();
				return copy;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The ChainingHashTableIterator appearing on the left-hand side 
			/// of the '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The ChainingHashTableIterator appearing on the right-hand side 
			/// of the '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the ChainingHashTableIterators are both 
			/// pointing to the same element, false otherwise.
			///	</returns> ----------------------------------------------------
			friend bool operator==(
				const OpenAddressingTableIterator& lhs,
				const OpenAddressingTableIterator& rhs
			) {
				return lhs.m_array == rhs.m_array &&
					lhs.m_index == rhs.m_index;
			}
		};
	};
}