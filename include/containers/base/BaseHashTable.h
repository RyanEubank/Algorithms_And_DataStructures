/* =============================================================================
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
* ========================================================================== */

#pragma once

#include <cmath>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <utility>

#include "../../algorithms/stream.h"
#include "../../concepts/collection.h"
#include "../../util/CRTP.h"
#include "../../util/key_value_pair.h"

namespace collections {

	template <class T>
	struct table_traits {};

	template <class derived_t>
	class BaseHashTable : public CRTP<derived_t, BaseHashTable<derived_t>> {
	public:

		using value_type			= table_traits<derived_t>::value_type;
		using key_type				= table_traits<derived_t>::key_type;
		using mapped_type			= table_traits<derived_t>::mapped_type;
		using prehash_type			= table_traits<derived_t>::prehash_type;
		using hash_type				= table_traits<derived_t>::hash_type;
		using equality_type			= table_traits<derived_t>::equality_type;
		using allocator_type		= table_traits<derived_t>::allocator_type;
		using reference				= table_traits<derived_t>::reference;	
		using const_reference		= table_traits<derived_t>::const_reference;	
		using size_type				= table_traits<derived_t>::size_type;
		using difference_type		= table_traits<derived_t>::difference_type;
		using pointer				= table_traits<derived_t>::pointer;
		using const_pointer			= table_traits<derived_t>::const_pointer;
		using iterator				= table_traits<derived_t>::iterator;
		using const_iterator		= table_traits<derived_t>::const_iterator;
		using local_iterator		= table_traits<derived_t>::local_iterator;
		using const_local_iterator	= table_traits<derived_t>::const_local_iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the allocator managing memory for the container.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the allocator managing memory for the container.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] allocator_type allocator() const noexcept {
			return static_cast<allocator_type>(this->self()._array.allocator());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the prehashing function for the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the prehash function used by the table to map keys to the
		/// set of all (size_type) integers.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] prehash_type prehash_func() const noexcept {
			return _prehash;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the hash function for the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the hashing function used by the table to map prehashed keys
		/// to a valid index in the table.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] hash_type hash_func() const noexcept {
			return _hash;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the key equality function for the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the equality function used by the table to determine if two
		/// keys are equal.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] equality_type equals_func() const noexcept {
			return _equals;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the table is empty and contains no elements.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true is the table has zero elements, false otherwise.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] bool isEmpty() const noexcept {
			return !(this->self().size());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the capacity - the number of buckets, of the hash table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the capacity of the internal array, or total reserved 
		/// space in number of elements it can hold.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] size_type capacity() const noexcept {
			return this->self()._array.size();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the current load factor of the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the ratio of the table's capacity to its size or actual
		/// number of elements stored.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] float load_factor() const noexcept {
			float size = static_cast<float>(this->self().size());
			float capacity = static_cast<float>(this->capacity());
			return size / capacity;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the maximum allowed load factor for the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the maximum allowed load factor of the table before it must 
		/// dynamically resize.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] float max_load_factor() const noexcept {
			return _max_load_factor;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Sets the maximum load factor for the table.
		/// </summary> 
		/// 
		/// <param name="max">
		/// The new maximum load factor to set.
		/// </param> -----------------------------------------------------------
		void set_max_load_factor(float max) noexcept {
			_max_load_factor = max;
			if (load_factor() > _max_load_factor)
				resize(capacity() << 1);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Attempts to reserve enough space for the specified number of 
		/// elements. Effectively calls 
		/// resize(ceil(capacity / max_load_factor())).
		/// </summary>
		/// 
		/// <param name="capacity">
		/// The minimum number of elements to reserve capacity for.
		/// </param> -----------------------------------------------------------
		void reserve(size_type capacity) {
			//TODO custom ceil algorithm and float -> int conversion safety
			float ceiling = 
				std::ceil(static_cast<float>(capacity) / _max_load_factor);
			resize(static_cast<size_type>(ceiling)); 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Explicitly resizes and rehashes the hash table's internal array to 
		/// the specified capacity.
		/// </summary>
		/// 
		/// <param name="size">
		/// The number of buckets to reserve space for.
		/// </param> -----------------------------------------------------------
		void resize(size_type numBuckets) {
			tryRehash(_prehash, _hash, numBuckets);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Rehashes the hash table's internal array with the specified pre-hash
		/// function and number of buckets.
		/// </summary>
		/// 
		/// <param name="hash">
		/// The pre-hash function to use.
		/// </param> 
		/// 
		/// <param name="numBuckets">
		/// The new capacity of the hash table. The current table capacity is
		/// the default (no change),
		/// </param> -----------------------------------------------------------
		void rehash(prehash_type prehash, size_type numBuckets = 0) {
			tryRehash(prehash, _hash, numBuckets);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Rehashes the hash table's internal array with the specified hash
		/// function and number of buckets.
		/// </summary>
		/// 
		/// <param name="hash">
		/// The hash function to use.
		/// </param>
		/// 
		/// <param name="numBuckets">
		/// The new capacity of the hash table. The current table capacity is
		/// the default (no change),
		/// </param> -----------------------------------------------------------
		void rehash(hash_type hash, size_type numBuckets = 0) {
			tryRehash(_prehash, hash, numBuckets);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Rehashes and resizes the hash table's internal array with the 
		/// specified prehash / hash functions and number of buckets.
		/// function.
		/// </summary>
		/// 
		/// <param name="hash">
		/// The pre-hash function to use.
		/// </param>
		/// 
		/// <param name="hash">
		/// The hash function to use.
		/// </param>
		/// 
		/// <param name="numBuckets">
		/// The new capacity of the hash table. Zero is the default value, which
		/// is interpretted as the table's current capacity, or no change.
		/// </param> -----------------------------------------------------------
		void rehash(
			prehash_type prehash, 
			hash_type hash, 
			size_type numBuckets = 0
		) {
			tryRehash(prehash, hash, numBuckets);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cbegin() const noexcept {
			return this->self().begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the last element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator cend() const noexcept {
			return this->self().end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the bucket for the specified key.
		/// </summary>
		/// 
		/// <param name="key">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns the index of the bucket for the given key.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type bucket(key_type key) const {
			return hash(key, _prehash, _hash, capacity());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the bucket for the specified key.
		/// </summary>
		/// 
		/// <param name="key">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns the index of the bucket for the given key.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type bucket_size(size_type bucket) const {
			return std::distance(
				this->self().begin(bucket), this->self().end(bucket)
			);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the table for the given element.
		/// </summary>
		/// 
		/// <param name="key">
		/// The element or key to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the first matching key if it exists in the
		/// tree, otherwise end() is returned. 
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator find(key_type key) {
			size_type index = hash(key, _prehash, _hash, capacity());
			return this->self().findAt(index, key);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the table for the given element.
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
			return find(key) != this->self().end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the table for the given element.
		/// </summary>
		/// 
		/// <param name="key">
		/// The element to seach for.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const_iterator to the first matching key if it exists, 
		/// otherwise end() is returned. 
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator find(key_type key) const {
			size_type index = hash(key, _prehash, _hash, capacity());
			return this->self().findAt(index, key);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Searches the table for the first element that matches the 
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
			return find(key) != this->self().end();
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
			const requires table_traits<derived_t>::allow_duplicates
		{
			size_type index = hash(key, _prehash, _hash, capacity());
			const_local_iterator begin = this->self().begin(index);
			const_local_iterator end = this->self().end(index);
			size_type count = 0;

			while (begin != end) {
				if constexpr (table_traits<derived_t>::is_map) {
					if (_equals((begin++)->key(), key))
						++count;
				}
				else {
					if (_equals(*begin++, key))
						++count;
				}
			}

			return count;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the table.
		/// </summary>
		/// 
		/// <param name="element">
		/// Const lvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element, or the element
		/// preventing insertion if unsuccessful.
		/// </returns> ---------------------------------------------------------
		iterator insert(const_reference element) {
			return tryInsert(this->self().end(), element);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the table.
		/// </summary>
		/// 
		/// <param name="element">
		/// Rvalue reference to the element to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the inserted element, or the element
		/// preventing insertion if unsuccessful.
		/// </returns> ---------------------------------------------------------
		iterator insert(value_type&& element) {
			return tryInsert(this->self().end(), std::move(element));
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the table.
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
		/// preventing its insertion. Returns the end() iterator if 
		/// begin == end.
		/// </returns> ---------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		iterator insert(in_iterator begin, sentinel end) {
			iterator result = this->self().end();
			while (begin != end) 
				result = tryInsert(this->self().end(), *begin++);
			return result;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the table with position as a 
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
			return tryInsert(position, element);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given element into the table, with position as a
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
			return tryInsert(position, std::move(element));
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the given range into the table, with position as a 
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
			iterator result = this->self().end();
			while (begin != end) 
				result = tryInsert(position, *begin++);
			return result;
		}

		// ---------------------------------------------------------------------
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
		/// </returns> ---------------------------------------------------------
		iterator remove(const_iterator position) {
			return this->self().removeAt(position);
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
			return this->self().removeAll(begin, end);
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
			pointer temp = constructElement(arg1, std::forward<Args>(args)...);
			iterator result = tryInsert(this->self().end(), std::move(*temp));
			destroyElement(temp);
			return result;
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
			pointer temp = constructElement(std::forward<Args>(args)...);
			iterator result = tryInsert(position, std::move(*temp));
			destroyElement(temp);
			return result;
		}

		// ---------------------------------------------------------------------
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
		/// </param> -----------------------------------------------------------
		friend void swap(BaseHashTable& a, BaseHashTable& b) 
			noexcept(derived_t::swapMembers) 
		{
			a.swap(b);
		}

		// ---------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given hash table with this table.
		/// </summary>
		/// 
		/// <param name="a">
		/// The table to be swapped.
		/// </param> -----------------------------------------------------------
		void swap(BaseHashTable& other) 
			noexcept(derived_t::swapMembers) 
		{
			this->self().swapMembers(other);
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
		/// The table being read from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const BaseHashTable& table
		) {
			collections::stream(
				table.self().begin(), 
				table.self().end(), 
				table.self().size(), 
				os
			);

			return os;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		/// 
		/// <param name="lhs">
		/// The table appearing on the left side of the operator.
		/// </param>
		/// <param name="rhs">
		/// The table appearing on the right side of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given tables contains exactly the same elements, 
		/// and share set equality.
		/// </returns> ---------------------------------------------------------
		friend bool operator==(
			const BaseHashTable& lhs, 
			const BaseHashTable& rhs
		) noexcept {
			//TODO extract algorithms header method for set equality
			if (lhs.self().size() == rhs.self().size()) {
				for (const auto& element : rhs.self()) {
					if constexpr (table_traits<derived_t>::is_map) {
						if (!lhs.contains(element.key()))
							return false;
					}
					else {
						if (!lhs.contains(element))
							return false;
					}
				}
				return true;
			}
			else
				return false;
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
		/// The table being written to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			BaseHashTable& table
		) {
			value_type value{};
			size_type size = 0;
			is >> size;

			table.self().clear();

			for (size_type i = 0; i < size; ++i) {
				is >> value;
				table.insert(value);
			}

			return is;
		}

	protected:

		using alloc_t		= rebind<allocator_type, value_type>;
		using alloc_traits	= std::allocator_traits<alloc_t>;

		struct insertResult {
			iterator pos;
			bool success;
		};

		constexpr BaseHashTable() :
			_max_load_factor(table_traits<derived_t>::default_max_load_factor), 
			_prehash(), 
			_hash(),
			_equals() 
		{

		}

		float _max_load_factor;

		[[no_unique_address, msvc::no_unique_address]]
		prehash_type _prehash;

		[[no_unique_address, msvc::no_unique_address]]
		hash_type _hash;

		[[no_unique_address, msvc::no_unique_address]]
		equality_type _equals;

		template <class ...Args>
		pointer constructElement(Args&&... args) {
			allocator_type alloc = allocator();
			pointer ptr = alloc_traits::allocate(alloc, 1);
			alloc_traits::construct(alloc, ptr, std::forward<Args>(args)...);
			return ptr;
		}

		void destroyElement(pointer ptr) {
			allocator_type alloc = allocator();
			alloc_traits::destroy(alloc, ptr);
			alloc_traits::deallocate(alloc, ptr, 1);
		}

		[[nodiscard]] size_type hash(
			key_type key, 
			prehash_type prehash, 
			hash_type hash_func,
			size_type table_size
		) const {
			size_type result = prehash(key);
			return hash_func(result, table_size);
		}

		[[nodiscard]] size_type hash(
			const_reference element, 
			prehash_type prehash, 
			hash_type hash_func,
			size_type table_size
		) const requires table_traits<derived_t>::is_map {
			size_type result = prehash(element.key());
			return hash_func(result, table_size);
		}

		[[nodiscard]] bool isEqualKey(
			key_type element, 
			key_type key
		) const {
			return _equals(element, key);
		}

		[[nodiscard]] bool isEqualKey(
			const_reference e1, 
			const_reference e2
		) const requires table_traits<derived_t>::is_map {
			return _equals(e1.key(), e2.key());
		}

		[[nodiscard]] bool isEqualKey(
			const_reference element, 
			key_type key
		) const requires table_traits<derived_t>::is_map {
			return _equals(element.key(), key);
		}

		template <class T>
		[[nodiscard]] iterator tryInsert(const_iterator position, T&& element) {
			if constexpr (table_traits<derived_t>::allow_duplicates) 
				return insertEquivalent(position, std::forward<T>(element));
			else 
				return insertNoEquivalent(std::forward<T>(element));
		}

		template <class T>
		[[nodiscard]] iterator insertEquivalent(
			const_iterator position, 
			T&& element
		) {
			if (load_factor() >= _max_load_factor) {
				resize(capacity() << 1);
				return hashAndInsert(
					std::forward<T>(element), _prehash, _hash, capacity());
			}
			else if (position != this->self().end() && isEqualKey(element, *position))
				return this->self().insertAt(position, std::forward<T>(element));
			else {
				return hashAndInsert(
					std::forward<T>(element), _prehash, _hash, capacity());
			}
		}

		template <class T>
		[[nodiscard]] iterator insertNoEquivalent(T&& element) {
			if (load_factor() >= _max_load_factor) 
				resize(capacity() << 1);

			size_type index = hash(element, _prehash, _hash, capacity());
			iterator result = this->self().end();
			
			if constexpr (table_traits<derived_t>::is_map)
				result = this->self().findAt(index, element.key());
			else
				result = this->self().findAt(index, element);

			if (result == this->self().end()) 
				return this->self().insertAt(index, std::forward<T>(element));
			else
				return result;
		}

		template <class T>
		iterator hashAndInsert(
			T&& element, 
			prehash_type prehash, 
			hash_type hash_func, 
			size_type capacity
		) {
			size_type index = hash(element, prehash, hash_func, capacity);
			return this->self().insertAt(index, std::forward<T>(element));
		}

		void tryRehash(
			prehash_type prehash, 
			hash_type hash, 
			size_type numBuckets = 0
		) {
			if (numBuckets == 0)
				numBuckets = capacity();

			float expectedLoadFactor = 
				static_cast<float>(this->self().size()) / 
				static_cast<float>(numBuckets);

			if (expectedLoadFactor <= _max_load_factor) 
				this->self().rehashTable(numBuckets, prehash, hash);
		}
	};
}