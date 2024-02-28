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
#include "ForwardList.h"
#include "../util/ArrayUtils.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// ChainingHashTable is a container class that implements a hash table
	/// structure using seperate chaining for hash collision resolution. It
	/// inherits from BaseHashTable to implement the IHashTable interface.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the HashTable.
	/// </typeparam> ----------------------------------------------------------
	template <
		Hashable key_t,
		class value_t,
		class chain_t = ForwardList<std::pair<key_t, value_t>>
	> requires CollectionConcept<chain_t, std::pair<key_t, value_t>>
	class ChainingHashTable final : public BaseHashTable
		<ChainingHashTable<key_t, value_t, chain_t>, key_t, value_t> {
	private:

		using base = BaseHashTable<
			ChainingHashTable<key_t, value_t, chain_t>, key_t, value_t>;

		using K = base::K;
		using V = base::V;
		using HashFunction = base::HashFunction;

		chain_t** m_array;

		chain_t** createNewTable(size_t capacity) {
			chain_t** table = new chain_t*[capacity];
			for (size_t i = 0; i < capacity; i++)
				table[i] = nullptr;
			return table;
		}

		void insertIntoTable(
			const K& key,
			const V& value,
			chain_t** table
		) {
			size_t index = this->hash(key);
			if (table[index] == nullptr)
				table[index] = new chain_t;
			table[index]->insert(std::pair(key, value));
		}

		void deleteTable(chain_t** table, size_t capacity) {
			if (table != nullptr) {
				for (size_t i = 0; i < capacity; i++)
					delete table[i];
				delete[] table;
			}
		}

	public:

		template <bool isConst>
		class ChainingHashTableIterator;

		using iterator = ChainingHashTableIterator<false>;
		using const_iterator = ChainingHashTableIterator<true>;

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty ChainingHashTable with the specified capacity, 
		/// or default capacity if not provided.
		/// </para></summary>
		/// 
		/// <param name="capacity">
		/// The initial capacity to allocate to the hash table.
		/// </param> 
		/// <param name="func">
		/// The hash function to use for hashing elements.
		/// </param> ----------------------------------------------------------
		ChainingHashTable(
			size_t capacity = base::DEFAULT_CAPACITY,
			HashFunction func = std::hash<K>()
		) : base(capacity, func), m_array(nullptr) {
			this->validateResize(capacity);
			m_array = createNewTable(capacity);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Constructor ~~~
		/// 
		/// <para>
		/// Constructs a deep copy of the specified ChainingHashTable.
		/// </para></summary>
		/// 
		/// <param name="copy">
		/// The ChainingHashTable to be copied.
		/// </param> ----------------------------------------------------------
		ChainingHashTable(const ChainingHashTable& copy) :
			ChainingHashTable(copy.m_capacity, copy.m_hashFunc)
		{
			if (copy.m_array != nullptr) {
				for (size_t i = 0; i < this->m_capacity; i++) {
					if (copy.m_array[i] != nullptr) 
						m_array[i] = new chain_t(*copy.m_array[i]);
				}
			}
			this->m_size = copy.m_size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ChainingHashTable by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ChainingHashTable to be moved into this one.
		/// </param> ----------------------------------------------------------
		ChainingHashTable(ChainingHashTable&& other) noexcept : 
			ChainingHashTable() 
		{
			swap(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a ChainingHashTable with the a copy of the elements in 
		/// the specified initialization list.
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
		ChainingHashTable(
			std::initializer_list<std::pair<K, V>> init,
			size_t capacity = base::DEFAULT_CAPACITY,
			HashFunction func = std::hash<K>()
		) : ChainingHashTable(
			init.begin(), init.size(), capacity, func
		) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a ChainingHashTable with the a copy of the elements in 
		/// the specified array.
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
		ChainingHashTable(
			const std::pair<K, V>* array,
			size_t size,
			size_t capacity = base::DEFAULT_CAPACITY,
			HashFunction func = std::hash<K>()
		) : ChainingHashTable(capacity, func) {
			for (const V* e = array; e != array + size; e++)
				this->insert(*e);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destructs the ChainingHashTable safely releasing its memory.
		/// </para></summary> -------------------------------------------------
		~ChainingHashTable() {
			deleteTable(m_array, this->m_capacity);
			m_array = nullptr;
			this->m_size = 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified ChainingHashTable to this 
		/// one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The ChainingHashTable to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this ChainingHashTable with the copied data.
		/// </returns> --------------------------------------------------------
		ChainingHashTable& operator=(ChainingHashTable other) {
			swap(*this, other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Clears the hash table of all its elements but maintains its current
		/// size.
		/// </summary> --------------------------------------------------------
		void clear() {
			for (size_t i = 0; i < this->m_capacity; i++) {
				if (m_array[i] != nullptr)
					m_array[i]->clear();
			}
			this->m_size = 0;
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
		void insert(const K& key, const V& value) {
			insertIntoTable(key, value, m_array);
			this->m_size++;
			if (this->loadFactor() > 1.0f)
				resize(this->m_capacity * 2.0f);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the element from the has table if it exists.
		/// </summary>
		/// 
		/// <param name="element">
		/// - The element to be removed.
		/// </param> ----------------------------------------------------------
		void remove(const K& key) {
			size_t index = this->hash(key);
			if (m_array[index] != nullptr) {
				for (const auto& it : *m_array[index]) {
					if (it.first == key) {
						m_array[index]->remove(it);
						break;
					}
				}
			}
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
		bool contains(const K& key) const {
			size_t index = this->hash(key);
			if (m_array[index] != nullptr) {
				for (const auto& e : *m_array[index]) {
					if (e.first == key)
						return true;
				}
			}
			return false;
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
			chain_t** copy = createNewTable(capacity);

			size_t oldCapacity = this->m_capacity;
			this->m_capacity = capacity;

			for (const auto& e : *this) 
				insertIntoTable(e.first, e.second, copy);

			deleteTable(m_array, oldCapacity);
			m_array = copy;
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
			return ChainingHashTableIterator<false>(m_array);
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
			return ChainingHashTableIterator<false>(m_array + this->m_capacity);
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
			return ChainingHashTableIterator<true>(m_array);
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
			return ChainingHashTableIterator<true>(m_array + this->m_capacity);
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
			ChainingHashTable<K, V, chain_t>& a, 
			ChainingHashTable<K, V, chain_t>& b
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
			const ChainingHashTable<K, V, chain_t>& lhs,
			const ChainingHashTable<K, V, chain_t>& rhs
		) {
			bool isCapacityEqual = (lhs.m_capacity == rhs.m_capacity);
			bool isSizeEqual = (lhs.m_size == rhs.m_size);
			
			if (isCapacityEqual && isSizeEqual) {
				for (const auto& e : lhs) {
					if (!rhs.contains(e.first))
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
			const ChainingHashTable<K, V, chain_t>& table
		) {
			os << "ChainingHashTable<" 
				<< typeid(K).name() << " " << typeid(V).name << ">"
				<< " (size = " << table.size() << ", "
				<< "capacity = " << table.capacity() << ", "
				<< "loadFactor = " << table.loadFactor() << ")"
				<< std::endl;

			table.writeToStream(os);
			return os;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ChainingHashTableIterator is a class that implements forward 
		/// iteration over a chaining hash table.
		/// </summary>
		///
		/// <typeparam name="element_t">
		/// The type of the elements iterated over by the 
		/// ChainingHashTableIterator.
		/// </typeparam> ------------------------------------------------------
		template <bool isConst>
		class ChainingHashTableIterator {
		private:

			using E = value_t;
			using itr = typename chain_t::iterator;

			chain_t** m_array;
			itr m_begin;
			itr m_end;

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a ChainingHashTableIterator pointing to the 
			/// start of the specified array.
			///	</summary>
			///
			/// <param name="arr">
			/// The array the iterator will point to.
			/// </param> ------------------------------------------------------
			explicit ChainingHashTableIterator(chain_t** arr)
				: m_array(arr), m_begin(), m_end()
			{
				advanceToNextValidChain();
			}

			void advanceToNextValidChain() {
				while (*m_array == nullptr || *m_array->isEmpty()) 
					m_array++;
				m_begin = *m_array->begin();
				m_end = *m_array->end();
			}

			void advanceIterator() {
				if (m_begin == m_end)
					advanceToNextValidChain();
				else
					m_begin++;
			}

			friend class ChainingHashTable<K, V, chain_t>;

		public:

			using pair = std::pair<K, V>;
			using value_type = std::conditional_t<isConst, const pair, pair>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty ChainingHashTableIterator.
			/// </para></summary> ---------------------------------------------
			ChainingHashTableIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given ChainingHashTableIterator.
			/// </para></summary> ---------------------------------------------
			ChainingHashTableIterator(const ChainingHashTableIterator& copy) 
				= default;

			// ----------------------------------------------------------------
			/// <summary>
			/// Destructs the ChainingHashTableIterator.
			///	</summary> ----------------------------------------------------
			~ChainingHashTableIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The ChainingHashTableIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this ChainingHashTableIterator with the copied data.
			/// </returns> ----------------------------------------------------
			ChainingHashTableIterator& operator=(
				const ChainingHashTableIterator& other
			) = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given ChainingHashTableIterator for 
			/// implicit conversion from non-const version to a const 
			/// ChainingHashTableIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const ChainingHashTableIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided ChainingHashTableIterator to 
			/// copy.
			/// </typeparam> --------------------------------------------------
			template<
				bool wasConst,
				class = std::enable_if_t<isConst && !wasConst>>
			ChainingHashTableIterator(ChainingHashTableIterator<wasConst> copy)
				: m_array(copy.m_array) {}

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
				return *m_begin;
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
			ChainingHashTableIterator& operator++() {
				advanceIterator();
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
			ChainingHashTableIterator operator++(int) {
				auto copy = *this;
				advanceIterator();
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
				const ChainingHashTableIterator& lhs,
				const ChainingHashTableIterator& rhs
			) {
				return lhs.m_array == rhs.m_array &&
					   lhs.m_begin == rhs.m_begin &&
					   lhs.m_end == rhs.m_end;
			}
		};
	};
}