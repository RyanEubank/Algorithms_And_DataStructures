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
#include "BaseCollection.h"
#include "../interfaces/IHashTable.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// BaseHashTable is a base class for hash tables that maintain a dynamic
	/// array and hashes elements to index them for constant time
	/// access. HashTable also implements the basic ICollection interface.
	/// </summary>
	///
	/// <typeparam name="element_t">
	/// The type of the elements contained by the HashTable.
	/// </typeparam> ----------------------------------------------------------
	template <
		class derived, 
		Hashable key_t,
		class value_t
	>
	class BaseHashTable :
		public BaseCollection<derived, value_t>,
		public IHashTable<derived, key_t, value_t>
	{
	protected:

		using K = key_t;
		using V = value_t;
		using HashFunction = std::function<size_t(K)>;

		HashFunction m_hashFunc;
		size_t m_capacity;

		size_t hash(const K& key) const {
			return m_hashFunc(key) % m_capacity;
		}

		BaseHashTable(
			size_t capacity = derived::DEFAULT_CAPACITY,
			HashFunction func = std::hash<K>()
		) : m_capacity(capacity), m_hashFunc(func) {}

		static constexpr auto INVALID_CAPACITY =
			"Invalid capacity: hash table cannot be 0 size.\n";

		void validateResize(size_t capacity) {
			if (capacity == 0)
				throw std::exception(INVALID_CAPACITY);
		}

	public:

		static constexpr auto DEFAULT_CAPACITY = 11;

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the capacity of the hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the capacity of the underlying array for the hash table as
		/// a size_t.
		/// </returns> --------------------------------------------------------
		size_t capacity() const {
			return m_capacity;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the load factor of the hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the load factor, or ratio between the number of elements
		/// and the table size, as a float.
		/// </returns> --------------------------------------------------------
		float loadFactor() const {
			return (float)this->m_size / (float)m_capacity;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Rehashes existsing elements from the table with the new hash 
		/// function placing them back into the table.
		/// </summary>
		/// 
		/// <param name="func">
		/// - A function pointer to the new hash function taking a const 
		///   reference to the element as a parameter and returning an unsigned 
		///   integer as the new hash index into the table.
		/// </param> ----------------------------------------------------------
		void rehash(HashFunction func) {
			resizeAndRehash(m_capacity, func);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Resizes the hash table and rehashes its existing elements with the 
		/// given hash function placing them into the resized table.
		/// </summary>
		/// 
		/// <param name="capacity">
		/// - The new capacity off the table.
		/// </param>
		/// <param name="func">
		/// - A function pointer to the new hash function taking a const 
		///   reference to the element as a parameter and returning an unsigned 
		///   integer as the new hash index into the table.
		/// </param> ----------------------------------------------------------
		void resizeAndRehash(size_t capacity, HashFunction func) {
			m_hashFunc = func;
			THIS_DERIVED->resize(capacity);
		}
	};
}