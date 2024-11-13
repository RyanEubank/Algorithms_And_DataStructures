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

#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "ForwardList.h"
#include "LinkedList.h"
#include "base/BaseHashTable.h"
#include "../algorithms/search.h"
#include "../concepts/associative.h"
#include "../concepts/collection.h"
#include "../concepts/iterable.h" 
#include "../concepts/map.h"
#include "../concepts/positional.h"

namespace collections {

	template <
		class element_t, 
		class prehash_t, 
		class hash_t, 
		class equality_t, 
		class allocator_t, 
		class chain_t, 
		bool hasDuplicates
	>
	class ChainingHashTable;

	template <
		class element_t, 
		class prehash_t, 
		class hash_t, 
		class equality_t, 
		class allocator_t, 
		class chain_t, 
		bool hasDuplicates
	>
	struct table_traits<
		ChainingHashTable<element_t, prehash_t, hash_t, equality_t, allocator_t, chain_t, hasDuplicates>
	> {
	private:

		using table_type = ChainingHashTable<
			element_t, 
			prehash_t, 
			hash_t, 
			equality_t, 
			allocator_t, 
			chain_t, 
			hasDuplicates
		>;

		template<bool isConst>
		class ChainingTableIterator;

		using alloc_t			= rebind<allocator_t, element_t>;
		using alloc_traits		= std::allocator_traits<alloc_t>;

		template <class T>
		struct chain_traits {
			using iterator				= ChainingTableIterator<false>;
			using const_iterator		= ChainingTableIterator<true>;
			using local_iterator		= chain_t::iterator;
			using const_local_iterator	= chain_t::const_iterator;
			using bucket_type			= chain_t;
		};

		template <class T> requires std::_Is_specialization_v<T, ForwardList>
		struct chain_traits<T> {
			using iterator				= T::stable_iterator;
			using const_iterator		= T::const_stable_iterator;
			using local_iterator		= iterator;
			using const_local_iterator	= const_iterator;
			using bucket_type			= std::pair<iterator, iterator>;
		};

		template <class T> requires std::_Is_specialization_v<T, LinkedList>
		struct chain_traits<T> {
			using iterator				= T::iterator;
			using const_iterator		= T::const_iterator;
			using local_iterator		= iterator;
			using const_local_iterator	= const_iterator;
			using bucket_type			= std::pair<iterator, iterator>;
		};

		friend class BaseHashTable<table_type>;

	public:

		using value_type			= element_t;
		using key_type				= key_traits<value_type>::key_type;
		using mapped_type			= key_traits<value_type>::mapped_type;
		using prehash_type			= prehash_t;
		using hash_type				= hash_t;
		using equality_type			= equality_t;
		using allocator_type		= allocator_t;
		using chain_type			= chain_t;
		using reference				= value_type&;	
		using const_reference		= const value_type&;	
		using size_type				= alloc_traits::size_type;
		using difference_type		= alloc_traits::difference_type;
		using pointer				= alloc_traits::pointer;
		using const_pointer			= alloc_traits::const_pointer;

		using iterator				= chain_traits<chain_type>::iterator;
		using const_iterator		= chain_traits<chain_type>::const_iterator;
		using local_iterator		= chain_traits<chain_type>::local_iterator;
		using const_local_iterator	= chain_traits<chain_type>::const_local_iterator;
		using bucket_type			= chain_traits<chain_type>::bucket_type;

		static constexpr bool allow_duplicates			= hasDuplicates;
		static constexpr bool is_map					= pair_type<value_type>;
		static constexpr size_type default_capacity		= 8;
		static constexpr float default_max_load_factor	= 1.0f;

	private:

		template <bool isConst>
		class ChainingTableIterator  {
		private:

			using element_t = table_type::value_type;

			using position = std::conditional_t<
				isConst,
				const_local_iterator,
				local_iterator
			>;

			using table_ptr = std::conditional_t<
				isConst,
				const table_type*,
				table_type*
			>;

			table_ptr _table;			// pointer to owner of the node
			size_type _index;			// position in the array
			position _local;			// local iterator position within the bucket

			explicit ChainingTableIterator(
				table_ptr table,
				size_type index, 
				position pos
			) : _table(table), _index(index), _local(pos) {
				validate();
			}

			void validate() {
				if (_index != _table->capacity()) {
					auto& bucket = _table->_array[_index];
					if (bucket.isEmpty() || _local == bucket.end())
						throw std::exception("Invalid iterator.");
				}
			}

			static void wrapToNextBucket(
				table_ptr table, 
				size_type& index, 
				position& pos
			) {
				while (++index < table->capacity()) {
					auto& bucket = table->_array[index];
					pos = bucket.begin();
					if (!bucket.isEmpty())
						break;
				}
			}

			void increment() {
				bool isEndOfBucket = (++_local == _table->_array[_index].end());
				if (isEndOfBucket) 
					wrapToNextBucket(_table, _index, _local);
			}

			friend class table_type;
			friend class ChainingTableIterator<!isConst>;

		public:

			// set values must be const to preserve ordering
			using value_type = std::conditional_t<
				is_map, element_t, const element_t
			>;

			using difference_type = std::ptrdiff_t;

			using pointer = std::conditional_t<
				isConst, const value_type*, value_type*
			>;

			using reference = std::conditional_t<
				isConst, const value_type&, value_type&
			>;

			using iterator_category = std::forward_iterator_tag;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty ChainingTableIterator.
			/// </para></summary> ----------------------------------------------
			ChainingTableIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given ChainingTableIterator.
			/// </para></summary> ----------------------------------------------
			ChainingTableIterator(const ChainingTableIterator& copy) = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// Destructs the ChainingTableIterator.
			///	</summary> -----------------------------------------------------
			~ChainingTableIterator() = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The ChainingTableIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this ChainingTableIterator with the copied data.
			/// </returns> -----------------------------------------------------
			ChainingTableIterator& operator=(const ChainingTableIterator& other) 
				= default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given ChainingTableIterator for 
			/// implicit conversion from non-const version to a const 
			/// ChainingTableIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const ChainingTableIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided ChainingTableIterator to copy.
			/// </typeparam> ---------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			ChainingTableIterator(ChainingTableIterator<wasConst> copy) : 
				ChainingTableIterator(copy._table, copy._index, copy._local) 
			{

			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the iterator
			/// in its current state.
			///	</returns> -----------------------------------------------------
			reference operator*() const {
				return *_local;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Arrow Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a pointer to the element pointed to by the iterator in 
			/// its current state.
			///	</returns> ----------------------------------------------------
			pointer operator->() const {
				if constexpr (std::is_pointer_v<position>)
					return _local;
				else
					return _local.operator->();
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> -----------------------------------------------------
			ChainingTableIterator& operator++() {
				increment();
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy of
			/// the iterator before updating.
			///	</returns> -----------------------------------------------------
			ChainingTableIterator operator++(int) {
				auto copy = *this;
				increment();
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The ChainingTableIterator appearing on the left-hand side of the 
			/// '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The ChainingTableIterator appearing on the right-hand side of the 
			/// '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the ChainingTableIterator are both pointing to 
			/// the same element, false otherwise.
			///	</returns> -----------------------------------------------------
			friend bool operator==(
				const ChainingTableIterator& lhs,
				const ChainingTableIterator& rhs
			) {
				return lhs._local == rhs._local;
			}
		};
	};


	// -------------------------------------------------------------------------
	/// <summary>
	/// 
	/// </summary>
	/// 
	/// <typeparam name="chain_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="element_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="prehash_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="hash_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="equality_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="allocator_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="hasDuplicates">
	/// 
	/// </typeparam> -----------------------------------------------------------
	template <
		class element_t, 
		class prehash_t,
		class hash_t,
		class equality_t,
		class allocator_t,
		class chain_t,
		bool hasDuplicates
	>
	class ChainingHashTable : 
		public BaseHashTable<
			ChainingHashTable<element_t, prehash_t, hash_t, equality_t, allocator_t, chain_t, hasDuplicates
		>
	> {
	private:

		using this_type = ChainingHashTable<
			element_t, 
			prehash_t,
			hash_t, 
			equality_t, 
			allocator_t, 
			chain_t, 
			hasDuplicates
		>;

		using array_type = DynamicArray<chain_t, rebind<allocator_t, chain_t>>;
		using bucket_type = table_traits<this_type>::bucket_type;

	public:

		using value_type			= table_traits<this_type>::value_type;
		using key_type				= table_traits<this_type>::key_type;
		using mapped_type			= table_traits<this_type>::mapped_type;
		using prehash_type			= table_traits<this_type>::prehash_type;
		using hash_type				= table_traits<this_type>::hash_type;
		using equality_type			= table_traits<this_type>::equality_type;
		using chain_type			= table_traits<this_type>::chain_type;
		using allocator_type		= table_traits<this_type>::allocator_type;
		using reference				= table_traits<this_type>::reference;	
		using const_reference		= table_traits<this_type>::const_reference;	
		using size_type				= table_traits<this_type>::size_type;
		using difference_type		= table_traits<this_type>::difference_type;
		using pointer				= table_traits<this_type>::pointer;
		using const_pointer			= table_traits<this_type>::const_pointer;
		using iterator				= table_traits<this_type>::iterator;
		using const_iterator		= table_traits<this_type>::const_iterator;
		using local_iterator		= table_traits<this_type>::local_iterator;
		using const_local_iterator	= table_traits<this_type>::const_local_iterator;

		static constexpr bool allow_duplicates = hasDuplicates; //TODO this is a duplicate field for concept multimap, make collection traits and specializations for all types, not just hash table

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ChainingHashTable.
		/// </para></summary> --------------------------------------------------
		constexpr ChainingHashTable() 
			noexcept(std::is_nothrow_constructible_v<array_type, Size, bucket_type, allocator_type>) :	
			BaseHashTable<this_type>(), 
			_array(Size(table_traits<this_type>::default_capacity)), 
			_size() 
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Allocator Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ChainingHashTable.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the table.
		/// </param> -----------------------------------------------------------
		constexpr explicit ChainingHashTable(const allocator_type& alloc)
			noexcept(std::is_nothrow_constructible_v<array_type, Size, bucket_type, allocator_type>) :
			BaseHashTable<this_type>(),
			_array(
				Size(table_traits<this_type>::default_capacity), 
				chain_type{}, 
				alloc
			),
			_size()
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		///	<para>
		/// Move constructs a hash table from the given table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be moved from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable(const ChainingHashTable&) = default;

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Move Constructor ---
		/// 
		///	<para>
		/// Move constructs a hash table from the given table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be moved from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable(ChainingHashTable&& other)
			noexcept(std::is_nothrow_move_constructible_v<array_type>) :
			BaseHashTable<this_type>(std::move(other)),
			_array(std::move(other._array)),
			_size(std::move(other._size))
		{
			//TODO resetting the size is the only reason we need explicit move 
			// semantics to begin with. Change size to some meaningful type that 
			// defaults or resets to a sentinel on move and revert to Rule of 0.
			other._size = 0; 
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Copy Assignment Operator ---
		/// 
		///	<para>
		/// Copies the data from the given argument to this table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be copied.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable& operator=(const ChainingHashTable&) = default;

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Move Assignment Operator ---
		/// 
		///	<para>
		/// Moves the data from the given argument to this table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be moved from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable& operator=(ChainingHashTable&& other) {
			this->_max_load_factor = std::move(other._max_load_factor);
			this->_prehash = std::move(other._prehash);
			this->_hash = std::move(other._hash);
			this->_equals = std::move(other._equals);
			_array = std::move(other._array);
			_size = std::move(other._size);
			other._size = 0;
			return *this;
		}

		//TODO constructor with bucket counts, hash func, equality func, etc.

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs a ChainingHashTable with the a copy of the elements in
		/// the specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : ChainingHashTable(init.begin(), init.end(), alloc) {

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ChainingHashTable with the a copy of the elements 
		/// from the given iterator pair.
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
		/// The beginning of the iterator pair to copy from.
		/// </param>
		/// <param name="end">
		/// The end of the iterator pair to copy from.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the table. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> -----------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		ChainingHashTable(
			in_iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : ChainingHashTable(alloc) {
			this->insert(begin, end);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ChainingHashTable with a copy of the elements
		/// from the given range.
		/// </para></summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to construct the table with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the table.
		/// </param> -----------------------------------------------------------
		template <std::ranges::input_range range>
		ChainingHashTable(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : ChainingHashTable(std::ranges::begin(r), std::ranges::end(r), alloc) {

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Removes and deletes all nodes from the table emptying its contents.
		/// </summary> ---------------------------------------------------------
		void clear() noexcept {
			for (auto& bucket : _array)
				bucket.clear();
			_size = 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the number of elements contained by the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of valid, constructed elements in the table.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type size() const noexcept {
			return _size;
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
			return _array.max_size();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() {
			if (this->isEmpty())
				return end();

			size_type index = 0;
			local_iterator pos = _array[index].begin();

			if (pos == _array[index].end()) 
				iterator::wrapToNextBucket(this, index, pos);

			return iterator(this, index, pos);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const {
			if (this->isEmpty())
				return end();

			size_type index = 0;
			const_local_iterator pos = _array[index].begin();

			if (pos == _array[index].end()) 
				const_iterator::wrapToNextBucket(this, index, pos);

			return const_iterator(this, index, pos);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the last element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() {
			size_type end_index = _array.size();
			return iterator(this, end_index, _array[end_index - 1].end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the last element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const {
			size_type end_index = _array.size();
			return const_iterator(this, end_index, _array[end_index - 1].end());
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the first element in the 
		/// specified bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the first element in the
		/// bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] local_iterator begin(size_type index) noexcept {
			return _array[index].begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the end of the specified 
		/// bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the position past the last 
		/// element in the bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] local_iterator end(size_type index) noexcept {
			return _array[index].end();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the first element in the 
		/// specified bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element in the
		/// bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_local_iterator begin(size_type index) const noexcept {
			return _array[index].begin();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the end of the specified 
		/// bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the position past the last 
		/// element in the bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_local_iterator end(size_type index) const noexcept {
			return _array[index].end();
		}

	private:

		friend class BaseHashTable<this_type>;
		friend iterator;
		friend const_iterator;

		array_type _array;
		size_type _size;

		iterator findAt(size_type index, key_type key) {
			local_iterator begin = this->begin(index);
			local_iterator end = this->end(index);

			while (begin != end) {
				if (this->isEqualKey(*begin, key))
					break;
				++begin;
			}

			return (begin == end) ? this->end() : iterator(this, index, begin);
		}

		const_iterator findAt(size_type index, key_type key) const {
			const_local_iterator begin = this->begin(index);
			const_local_iterator end = this->end(index);

			while (begin != end) {
				if (this->isEqualKey(*begin, key))
					break;
				++begin;
			}

			return (begin == end) ? this->end() : const_iterator(this, index, begin);
		}

		template <class T>
		[[nodiscard]] iterator insertAt(const_iterator position, T&& element) {
			return insertAt(position._index, element);
		}

		template <class T>
		[[nodiscard]] iterator insertAt(size_type index, T&& element) {
			bucket_type& bucket = _array[index];
			local_iterator result = tryInsert(bucket, std::forward<T>(element));
			return iterator(this, index, result);
		}

		template <class T>
		[[nodiscard]] local_iterator tryInsert(bucket_type& bucket, T&& element) {
			size_type bucket_size = bucket.size();
			local_iterator result = insertInto(bucket, std::forward<T>(element));
			
			// TODO exception safety - bucket insertion may fail - 
			// only increase total table size if insertion is successful.
			++_size; 
			return result;
		}

		template <class T>
		local_iterator insertInto(bucket_type& bucket, T&& element) {
			if constexpr (sequential<bucket_type>)
				return bucket.emplaceBack(std::forward<T>(element));
			else
				return bucket.emplace(std::forward<T>(element));
		}

		iterator removeAt(const_iterator position) {
			size_type index = this->hash(
				*position, 
				this->_prehash, 
				this->_hash, 
				this->capacity()
			);

			bucket_type& bucket = _array[index];
			local_iterator result = bucket.remove(position._local);
			
			if (result == bucket.end()) 
				iterator::wrapToNextBucket(this, index, result);
			
			--_size;
			return iterator(this, index, result);
		}

		iterator removeAll(const_iterator begin, const_iterator end) {
			iterator result = this->end();
			while (begin != end)
				result = removeAt(begin++);
			return result;
		}

		void rehashTable(
			size_type capacity, 
			prehash_type prehash, 
			hash_type hash
		) {
			array_type copy{ Size(capacity), chain_type{}, this->allocator() };
			
			for (auto& e : *this) {
				size_type index = this->hash(e, prehash, hash, capacity);
				bucket_type& bucket = copy[index];
				insertInto(bucket, std::move_if_noexcept(e));
			}

			_array = std::move(copy);
			this->_prehash = prehash;
			this->_hash = hash;
		}
	};

	// -------------------------------------------------------------------------
	/// <summary>
	/// 
	/// </summary>
	/// 
	/// <typeparam name="chain_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="element_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="prehash_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="hash_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="equality_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="allocator_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="hasDuplicates">
	/// 
	/// </typeparam> -----------------------------------------------------------
	template <
		class element_t, 
		class prehash_t,
		class hash_t,
		class equality_t,
		class allocator_t,
		class chain_t,
		bool hasDuplicates
	> requires (
		std::_Is_specialization_v<chain_t, ForwardList> || 
		std::_Is_specialization_v<chain_t, LinkedList>
	)
	class ChainingHashTable<element_t, prehash_t, hash_t, equality_t, allocator_t, chain_t, hasDuplicates> : 
		public BaseHashTable<
			ChainingHashTable<element_t, prehash_t, hash_t, equality_t, allocator_t, chain_t, hasDuplicates
		>
	> {
	private:

		using this_type = ChainingHashTable<
			element_t, 
			prehash_t,
			hash_t, 
			equality_t, 
			allocator_t, 
			chain_t, 
			hasDuplicates
		>;

		using bucket_type = table_traits<this_type>::bucket_type;
		using bucket_alloc_t = rebind<allocator_t, bucket_type>;
		using array_type = DynamicArray<bucket_type, bucket_alloc_t>;

	public:

		using value_type			= table_traits<this_type>::value_type;
		using key_type				= table_traits<this_type>::key_type;
		using mapped_type			= table_traits<this_type>::mapped_type;
		using prehash_type			= table_traits<this_type>::prehash_type;
		using hash_type				= table_traits<this_type>::hash_type;
		using equality_type			= table_traits<this_type>::equality_type;
		using chain_type			= table_traits<this_type>::chain_type;
		using allocator_type		= table_traits<this_type>::allocator_type;
		using reference				= table_traits<this_type>::reference;	
		using const_reference		= table_traits<this_type>::const_reference;	
		using size_type				= table_traits<this_type>::size_type;
		using difference_type		= table_traits<this_type>::difference_type;
		using pointer				= table_traits<this_type>::pointer;
		using const_pointer			= table_traits<this_type>::const_pointer;
		using iterator				= table_traits<this_type>::iterator;
		using const_iterator		= table_traits<this_type>::const_iterator;
		using local_iterator		= table_traits<this_type>::local_iterator;
		using const_local_iterator	= table_traits<this_type>::const_local_iterator;

		// TODO required to satisfy collection test fixture requirements
		// without creating a forward iterator wrapper for linked list 
		// specialization. Either create wrapper or refactor so this is not required
		using reverse_iterator			= void;
		using const_reverse_iterator	= void;

		static constexpr bool allow_duplicates = hasDuplicates; //TODO this is a duplicate field for concept multimap, make collection traits and specializations for all types, not just hash table

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Default Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ChainingHashTable.
		/// </para></summary> --------------------------------------------------
		constexpr ChainingHashTable() 
			noexcept(std::is_nothrow_constructible_v<array_type, Size, bucket_type, allocator_type>) :	
			BaseHashTable<this_type>(), 
			_list(),
			_array(
				Size(table_traits<this_type>::default_capacity), 
				{ end(), end() }
			)
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Allocator Constructor ---
		/// 
		///	<para>
		/// Constructs an empty ChainingHashTable.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The allocator instance used by the table.
		/// </param> -----------------------------------------------------------
		constexpr explicit ChainingHashTable(const allocator_type& alloc) 
			noexcept(std::is_nothrow_constructible_v<array_type, Size, bucket_type, allocator_type>) :
			BaseHashTable<this_type>(),
			_list(),
			_array(
				Size(table_traits<this_type>::default_capacity), 
				{ end(), end() }, 
				alloc
			)
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Copy Constructor ---
		/// 
		///	<para>
		/// Move constructs a hash table from the given table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be moved from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable(const ChainingHashTable& other) 
			noexcept(std::is_nothrow_copy_constructible_v<array_type>) :
			BaseHashTable<this_type>(other),
			_array(other._array),
			_list(other._list)
		{
			copyEmptyBuckets(other.end());
		};

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Move Constructor ---
		/// 
		///	<para>
		/// Move constructs a hash table from the given table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be moved from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable(ChainingHashTable&& other)
			noexcept(std::is_nothrow_move_constructible_v<array_type>) :
			BaseHashTable<this_type>(std::move(other)),
			_array(std::move(other._array)),
			_list(std::move(other._list))
		{
			copyEmptyBuckets(other.end());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Copy Assignment Operator ---
		/// 
		///	<para>
		/// Copies the data from the given argument to this table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be copied.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable& operator=(const ChainingHashTable& other) {
			this->_max_load_factor = other._max_load_factor;
			this->_prehash = other._prehash;
			this->_hash = other._hash;
			this->_equals = other._equals;

			_list = other._list;
			_array = other._array;

			copyEmptyBuckets(other.end());

			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// --- Move Assignment Operator ---
		/// 
		///	<para>
		/// Moves the data from the given argument to this table.
		/// </para></summary>
		/// 
		/// <param name="alloc">
		/// The table to be moved from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable& operator=(ChainingHashTable&& other) {
			this->_max_load_factor = std::move(other._max_load_factor);
			this->_prehash = std::move(other._prehash);
			this->_hash = std::move(other._hash);
			this->_equals = std::move(other._equals);

			_list = std::move(other._list);
			_array = std::move(other._array);

			copyEmptyBuckets(other.end());

			return *this;
		}

		//TODO constructor with bucket counts, hash func, equality func, etc.

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs a ChainingHashTable with the a copy of the elements in
		/// the specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> -----------------------------------------------------------
		constexpr ChainingHashTable(
			std::initializer_list<value_type> init,
			const allocator_type& alloc = allocator_type{}
		) : ChainingHashTable(init.begin(), init.end(), alloc) {

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Iterator Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ChainingHashTable with the a copy of the elements 
		/// from the given iterator pair.
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
		/// The beginning of the iterator pair to copy from.
		/// </param>
		/// <param name="end">
		/// The end of the iterator pair to copy from.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance used by the table. Default constructs the 
		/// allocator_type if unspecified.
		/// </param> -----------------------------------------------------------
		template <
			std::input_iterator in_iterator,
			std::sentinel_for<in_iterator> sentinel
		>
		ChainingHashTable(
			in_iterator begin,
			sentinel end,
			const allocator_type& alloc = allocator_type{}
		) : ChainingHashTable(alloc) {
			this->insert(begin, end);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Range Constructor ~~~
		/// 
		/// <para>
		/// Constructs a ChainingHashTable with a copy of the elements
		/// from the given range.
		/// </para></summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being constructed from.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to construct the table with.
		/// </param>
		/// <param name="alloc">
		/// The allocator instance for the table.
		/// </param> -----------------------------------------------------------
		template <std::ranges::input_range range>
		ChainingHashTable(
			from_range_t tag,
			range&& r,
			const allocator_type& alloc = allocator_type{}
		) : ChainingHashTable(std::ranges::begin(r), std::ranges::end(r), alloc) {

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Removes and deletes all nodes from the table emptying its contents.
		/// </summary> ---------------------------------------------------------
		void clear() noexcept {
			for (auto& bucket : _array) 
				bucket = { end(), end() };
			_list.clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the number of elements contained by the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of valid, constructed elements in the table.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] size_type size() const noexcept {
			return _list.size();
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
			return _list.max_size();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator begin() {
			return begin(_list);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the first element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator begin() const {
			return begin(_list);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the last element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] iterator end() {
			return end(_list);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the last element in the table.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the last element.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_iterator end() const {
			return end(_list);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the first element in the 
		/// specified bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the first element in the
		/// bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] local_iterator begin(size_type index) noexcept {
			return _array[index].first;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the end of the specified 
		/// bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the position past the last 
		/// element in the bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] local_iterator end(size_type index) noexcept {
			auto pos = _array[index].second;
			return (pos == end()) ? pos : (++pos);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the first element in the 
		/// specified bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element in the
		/// bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_local_iterator begin(size_type index) const noexcept {
			return _array[index].first;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a local iterator pointing to the end of the specified 
		/// bucket.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the position past the last 
		/// element in the bucket.
		/// </returns> --------------------------------------------------------
		[[nodiscard]] const_local_iterator end(size_type index) const noexcept {
			auto pos = _array[index].second;
			return (pos == end()) ? pos : (++pos);
		}

	private:

		friend class BaseHashTable<this_type>;
		friend iterator;
		friend const_iterator;

		array_type _array;
		chain_type _list;

		[[nodiscard]] iterator findAt(size_type index, key_type key) {
			local_iterator begin = this->begin(index);
			local_iterator end = this->end(index);

			while (begin != end) {
				if (this->isEqualKey(*begin, key))
					break;
				++begin;
			}

			return (begin == end) ? this->end() : begin;
		}

		[[nodiscard]] const_iterator findAt(
			size_type index,
			key_type key
		) const {
			const_local_iterator begin = this->begin(index);
			const_local_iterator end = this->end(index);

			while (begin != end) {
				if (this->isEqualKey(*begin, key))
					break;
				++begin;
			}

			return (begin == end) ? this->end() : begin;
		}

		[[nodiscard]] auto& bucketOf(const_iterator pos) {
			size_type index = this->hash(
				*pos, 
				this->_prehash, 
				this->_hash, 
				this->capacity()
			);
			return _array[index];
		}

		template <class T>
		[[nodiscard]] iterator insertAt(size_type index, T&& element) {
			bucket_type& bucket = _array[index];
			return insertInto(bucket, _list, std::forward<T>(element));
		}

		template <class T>
		[[nodiscard]] iterator insertAt(const_iterator position, T&& element) {
			bucket_type& bucket = bucketOf(position);
			return insertInto(bucket, _list, std::forward<T>(element));
		}

		void rehashTable(
			size_type capacity, 
			prehash_type pre, 
			hash_type hash
		) {
			chain_type list_copy{};
			array_type arr_copy (
				Size(capacity), 
				{ end(), end() },
				this->allocator()
			);

			for (auto& e : *this) {
				size_type index = this->hash(e, pre, hash, capacity);
				bucket_type& bucket = arr_copy[index];
				insertInto(bucket, list_copy, std::move_if_noexcept(e));
			}

			_array = std::move(arr_copy);
			_list = std::move(list_copy);
			this->_prehash = pre;
			this->_hash = hash;
		}

		void copyEmptyBuckets(const_iterator sentinel) {
			for (auto& bucket : _array) {
				if (bucket.first == sentinel) {
					bucket.first = end();
					bucket.second = end();
				}
			}
		}

		// ----------------- FORWARD LIST SPECIALIOZATIONS ------------------ //

		[[nodiscard]] iterator begin(chain_type& list) 
			requires (std::_Is_specialization_v<chain_type, ForwardList>)
		{
			return list.stable_begin();
		}

		[[nodiscard]] const_iterator begin(const chain_type& list) 
			const requires (std::_Is_specialization_v<chain_type, ForwardList>) 
		{
			return list.stable_begin();
		}

		[[nodiscard]] iterator end(chain_type& list) 
			requires (std::_Is_specialization_v<chain_type, ForwardList>) 
		{
			return list.stable_end();
		}

		[[nodiscard]] const_iterator end(const chain_type& list) 
			const requires (std::_Is_specialization_v<chain_type, ForwardList>) 
		{
			return list.stable_end();
		}

		template <class T>
		local_iterator insertInto(
			bucket_type& bucket, 
			chain_type& list, 
			T&& element
		) requires (std::_Is_specialization_v<chain_type, ForwardList>) {
			iterator result = end();

			if (bucket.first == end()) {
				result = list.insertAfter(end(list), std::forward<T>(element));
				bucket.first = result;
				bucket.second = result;
			}
			else if (bucket.first == bucket.second) {
				result = list.insertAfter(bucket.second, std::forward<T>(element));
				bucket.second = result;
			}
			else 
				result = list.insertAfter(bucket.first, std::forward<T>(element));

			return result;
		}

		[[nodiscard]] iterator removeAt(const_iterator position) 
			requires (std::_Is_specialization_v<chain_type, ForwardList>) 
		{
			bucket_type& bucket = bucketOf(position);

			// removing last item in list - must splice beginning bucket
			// in front of current pos so the current item can be swapped 
			// forward.
			if (std::next(position) == end())
				spliceFirstBucketAfter(bucket);
			if (position == bucket.second)
				adjustNextBucket(bucket);

			// swap with next element because forward list can only remove nodes
			// after the current
			iterator result = swapForward(bucket, position);
			onRemoveFrom(bucket);
			_list.removeAfter(position);
			return result;
		}

		[[nodiscard]] iterator removeAll(
			const_iterator begin, 
			const_iterator end
		) requires (std::_Is_specialization_v<chain_type, ForwardList>) {
			iterator result = this->end();

			if (begin == end)
				return result;

			while (std::next(begin) != end)
				result = removeAt(begin);
			result = removeAt(begin);

			return result;
		}

		void onRemoveFrom(bucket_type& bucket) 
			requires (std::_Is_specialization_v<chain_type, ForwardList>) 
		{
			if (bucket.first == bucket.second) {
				bucket.first = end();
				bucket.second = end();
			}
			else {
				if (std::next(bucket.first) == bucket.second)
					bucket.second = bucket.first;
			}
		}

		void spliceFirstBucketAfter(bucket_type& bucket) 
			requires (std::_Is_specialization_v<chain_type, ForwardList>)
		{
			iterator begin = _list.stable_begin();
			iterator before = _list.stable_end();
			bucket_type& first_bucket = bucketOf(begin);
			_list.spliceAfter(bucket.second, _list, before, first_bucket.second);
		}

		void adjustNextBucket(bucket_type& bucket) 
			requires (std::_Is_specialization_v<chain_type, ForwardList>)
		{
			bucket_type& next_bucket = bucketOf(std::next(bucket.second));

			if (next_bucket.first == next_bucket.second)
				next_bucket.second = bucket.second;

			next_bucket.first = bucket.second;
		}

		[[nodiscard]] iterator swapForward(
			bucket_type& bucket, 
			const_iterator position
		) requires (std::_Is_specialization_v<chain_type, ForwardList>) {
			using std::swap;

			iterator current = bucket.first;
			while (current != position)
				++current;

			//TODO destruct and copy if not swappable OR manage non-const keys in
			// map containers instead of bad design const_cast-ing f**kery 

			// key type should be const - non swappable by default,
			// but there is no way to move the list node forward
			// without access to the previous node. Therefore the
			// only option is to swap values
			if constexpr (table_traits<this_type>::is_map) {
				auto& pair1 = const_cast<reference>(*position);
				auto& pair2 = const_cast<reference>(*std::next(position));
				auto& key1 = const_cast<std::remove_const_t<key_type>&>(pair1.key());
				auto& val1 = pair1.value();
				auto& key2 = const_cast<std::remove_const_t<key_type>&>(pair2.key());
				auto& val2 = pair2.value();
				swap(key1, key2);
				swap(val1, val2);
			}
			else
				swap(*current, *std::next(current));

			return current;
		}

		// ------------------ LINKED LIST SPECIALIOZATIONS ------------------ //

		[[nodiscard]] iterator begin(chain_type& list) 
			requires (std::_Is_specialization_v<chain_type, LinkedList>)
		{
			return list.begin();
		}

		[[nodiscard]] const_iterator begin(const chain_type& list) 
			const requires (std::_Is_specialization_v<chain_type, LinkedList>)
		{
			return list.begin();
		}

		[[nodiscard]] iterator end(chain_type& list) 
			requires (std::_Is_specialization_v<chain_type, LinkedList>) 
		{
			return list.end();
		}

		[[nodiscard]] const_iterator end(const chain_type& list) 
			const requires (std::_Is_specialization_v<chain_type, LinkedList>) 
		{
			return list.end();
		}

		template <class T>
		local_iterator insertInto(
			bucket_type& bucket, 
			chain_type& list, 
			T&& element
		) requires (std::_Is_specialization_v<chain_type, LinkedList>) {
			iterator result = end();

			if (bucket.first == end()) {

				// Empty buckets must point to this->end(), because list 
				// sentinels are not moved along with list data. When rehashing 
				// or resizing we insert into a copy of the list, but 
				// this->end() is not a valid insert location, therefore insert 
				// into end(list) to ensure the insert location is the correct 
				// 'end' iterator.
				result = list.insert(end(list), std::forward<T>(element));
				bucket.first = result;
				bucket.second = result;
			}
			else if (bucket.first == bucket.second) 
				result = list.insert(bucket.first--, std::forward<T>(element));
			else 
				result = list.insert(bucket.second, std::forward<T>(element));

			return result;
		}

		[[nodiscard]] iterator removeAt(const_iterator position) 
			requires (std::_Is_specialization_v<chain_type, LinkedList>) 
		{
			onRemoveFrom(bucketOf(position), position);
			return _list.remove(position);
		}

		[[nodiscard]] iterator removeAll(
			const_iterator begin, 
			const_iterator end
		) requires (std::_Is_specialization_v<chain_type, LinkedList>) {
			iterator result = this->end();
			while (begin != end)
				result = removeAt(begin++);
			return result;
		}

		void onRemoveFrom(bucket_type& bucket, const_iterator position) 
			requires (std::_Is_specialization_v<chain_type, LinkedList>) 
		{
			if (bucket.first == bucket.second) {
				bucket.first = end();
				bucket.second = end();
			}
			else if (position == bucket.first) 
				++bucket.first;
			else if (position == bucket.second) 
				--bucket.second;
		}
	};

	template <
		class element_t,
		template <class> class prehash_t	= std::hash,
		template <class> class hash_t		= std::modulus,
		template <class> class equality_t	= std::equal_to,
		template <class> class allocator_t	= std::allocator,
		class chain_t						= ForwardList<element_t, allocator_t<element_t>>
	>
	using ChainedHashSet = ChainingHashTable<
		element_t, 
		prehash_t<element_t>, 
		hash_t<size_t>, 
		equality_t<element_t>, 
		allocator_t<element_t>, 
		chain_t, 
		false
	>;

	template <
		class key_t,
		class element_t,
		template <class> class prehash_t	= std::hash,
		template <class> class hash_t		= std::modulus,
		template <class> class equality_t	= std::equal_to,
		template <class> class allocator_t	= std::allocator,
		class chain_t						= ForwardList<key_value_pair<const key_t, element_t>, allocator_t<key_value_pair<key_t, element_t>>>
	>
	using ChainedHashMap = ChainingHashTable<
		key_value_pair<const key_t, element_t>, 
		prehash_t<key_t>, 
		hash_t<size_t>, 
		equality_t<key_t>, 
		allocator_t<key_value_pair<const key_t, element_t>>, 
		chain_t, 
		false
	>;

	template <
		class element_t,
		template <class> class prehash_t	= std::hash,
		template <class> class hash_t		= std::modulus,
		template <class> class equality_t	= std::equal_to,
		template <class> class allocator_t	= std::allocator,
		class chain_t						= ForwardList<element_t, allocator_t<element_t>>
	>
	using ChainedHashMultiSet = ChainingHashTable<
		element_t, 
		prehash_t<element_t>, 
		hash_t<size_t>, 
		equality_t<element_t>, 
		allocator_t<element_t>, 
		chain_t, 
		true
	>;

	template <
		class key_t,
		class element_t,
		template <class> class prehash_t	= std::hash,
		template <class> class hash_t		= std::modulus,
		template <class> class equality_t	= std::equal_to,
		template <class> class allocator_t	= std::allocator,
		class chain_t						= ForwardList<key_value_pair<const key_t, element_t>, allocator_t<key_value_pair<key_t, element_t>>>
	>
	using ChainedHashMultiMap = ChainingHashTable<
		key_value_pair<const key_t, element_t>, 
		prehash_t<key_t>, 
		hash_t<size_t>, 
		equality_t<key_t>, 
		allocator_t<key_value_pair<const key_t, element_t>>, 
		chain_t, 
		true
	>;

	static_assert(
		collection<ChainedHashSet<int>>,
		"ChainedHashSet does not meet the requirements for a collection."
	);

	static_assert(
		collection<ChainedHashMap<int, int>>,
		"ChainedHashMap does not meet the requirements for a collection."
	);

	static_assert(
		collection<ChainedHashMultiSet<int>>,
		"ChainedHashMultiSet does not meet the requirements for a collection."
	);

	static_assert(
		collection<ChainedHashMultiMap<int, int>>,
		"ChainedHashMultiMap does not meet the requirements for a collection."
	);

	static_assert(
		associative<ChainedHashSet<int>>,
		"ChainingHashTable does not meet the requirements for associative access."
	);

	static_assert(
		positional<ChainedHashSet<int>>,
		"ChainingHashTable does not meet the requirements for positional access."
	);

	static_assert(
		forward_iterable<ChainedHashSet<int>>,
		"ChainingHashTable does not meet the requirements for forward iteration."
	);

	static_assert(
		map<ChainedHashMap<int, int>>,
		"ChainingHashTable does not meet the requirements for a map."
	);

	static_assert(
		multimap<ChainedHashMultiMap<int, int>>,
		"ChainedHashMultiMap does not meet the requirements for a multimap."
	);
}