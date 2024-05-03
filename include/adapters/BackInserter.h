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

#include <assert.h>
#include <iterator>
#include <ranges>

#include "../concepts/sequential.h"
#include "../containers/DynamicArray.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Class designed to implement a back insertion iterator for collections.
	/// </summary>
	/// 
	/// <typeparam name="collection">
	/// The collection type to be iterator over.
	/// </typeparam> ----------------------------------------------------------
	template <class collection>
	class back_insert_iterator {
	public:
		
		using value_type = void;
		using difference_type = std::ptrdiff_t;
		using pointer = void;
		using reference = void;
		using iterator_category = std::output_iterator_tag;

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a back insertion iterator for the given collection.
		/// </summary>
		/// 
		/// <param name="c">
		/// The collection to construct a back_insert_iterator for.
		/// </param> ----------------------------------------------------------
		explicit back_insert_iterator(collection& c) : _collection(c) {}

		back_insert_iterator(const back_insert_iterator& it) = default;
		back_insert_iterator(back_insert_iterator&& it) = default;
		~back_insert_iterator() = default;

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the collection reference from the specified iterator to this 
		/// one.
		/// </summary>
		/// 
		/// <param name="other">
		/// The back_insert_iterator to copy.
		/// </param>
		/// 
		/// <returns>
		/// Returns the iterator after copying.
		/// </returns> --------------------------------------------------------
		back_insert_iterator& operator=(const back_insert_iterator& other) {
			_collection = other._collection;
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified value at the back or end of the collection
		/// the iterator references.
		/// </summary>
		/// 
		/// <param name="value">
		/// The value to be inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns the iterator after insertion.
		/// </returns> --------------------------------------------------------
		back_insert_iterator& operator=(
			const typename collection::value_type& value
		) {
			_collection.insertBack(value);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Move inserts the specified value at the back or end of the 
		/// collection the iterator references.
		/// </summary>
		/// 
		/// <param name="value">
		/// The value to be move inserted.
		/// </param>
		/// 
		/// <returns>
		/// Returns the iterator after insertion.
		/// </returns> --------------------------------------------------------
		back_insert_iterator& operator=(
			typename collection::value_type&& value
		) {
			_collection.insertBack(std::move(value));
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Dereferences the back_insert_iterator, does nothing, but neccessary
		/// for iterator semantics.
		/// </summary>
		/// 
		/// <returns>
		/// Returns this iterator (NOP).
		/// </returns> --------------------------------------------------------
		back_insert_iterator& operator*() {
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Increments the back_insert_iterator, does nothing, but neccessary
		/// for iterator semantics.
		/// </summary>
		/// 
		/// <returns>
		/// Returns this iterator (NOP).
		/// </returns> --------------------------------------------------------
		back_insert_iterator& operator++() {
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Increments the back_insert_iterator, does nothing, but neccessary
		/// for iterator semantics.
		/// </summary>
		/// 
		/// <returns>
		/// Returns this iterator (NOP).
		/// </returns> --------------------------------------------------------
		back_insert_iterator& operator++(int) {
			return *this;
		}

	private:
		collection& _collection;

	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// Convienience method to construct a back_insert_iterator for the given
	/// collection.
	/// </summary>
	/// 
	/// <typeparam name="collection">
	/// The type of collection to construct the iterator with.
	/// </typeparam>
	/// 
	/// <param name="c">
	/// The collection to construct the iterator for.
	/// </param>
	/// 
	/// <returns>
	/// Returns a constructed back_insert_iterator for the given collection.
	/// </returns> ------------------------------------------------------------
	template <sequential collection>
	back_insert_iterator<collection> back_inserter(collection& c) {
		return back_insert_iterator{ c };
	}
}