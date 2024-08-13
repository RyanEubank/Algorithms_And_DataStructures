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
#include <utility>

#include "../concepts/collection.h"

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Class designed to implement an insertion iterator for collections.
	/// </summary>
	/// 
	/// <typeparam name="collection">
	/// The collection type to be iterator over.
	/// </typeparam> ----------------------------------------------------------
	template <ordered_collection collection_>
	class insert_iterator {
	public:

		using iterator = typename collection_::const_iterator;
		using value_type = void;
		using difference_type = std::ptrdiff_t;
		using pointer = void;
		using reference = void;
		using iterator_category = std::output_iterator_tag;

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs an insert iterator for the given collection.
		/// </summary>
		/// 
		/// <param name="c">
		/// The collection to construct a insert_iterator for.
		/// </param> ----------------------------------------------------------
		explicit insert_iterator(collection_& c, iterator i) : 
			_collection(c), _pos(i)
		{

		}

		insert_iterator(const insert_iterator& it) = default;
		insert_iterator(insert_iterator&& it) = default;
		~insert_iterator() = default;

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the collection reference from the specified iterator to this 
		/// one.
		/// </summary>
		/// 
		/// <param name="other">
		/// The insert_iterator to copy.
		/// </param>
		/// 
		/// <returns>
		/// Returns the iterator after copying.
		/// </returns> --------------------------------------------------------
		insert_iterator& operator=(const insert_iterator& other) {
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
		insert_iterator& operator=(
			const typename collection_::value_type& value
		) {
			_pos = _collection.insert(_pos, value);
			_pos++;
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
		insert_iterator& operator=(
			typename collection_::value_type&& value
		) {
			_pos = _collection.insert(_pos, std::move(value));
			_pos++;
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Dereferences the insert_iterator, does nothing, but neccessary for
		/// iterator semantics.
		/// </summary>
		/// 
		/// <returns>
		/// Returns this iterator (NOP).
		/// </returns> --------------------------------------------------------
		insert_iterator& operator*() {
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Increments the binsert_iterator, does nothing, but neccessary for
		/// iterator semantics.
		/// </summary>
		/// 
		/// <returns>
		/// Returns this iterator (NOP).
		/// </returns> --------------------------------------------------------
		insert_iterator& operator++() {
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Increments the insert_iterator, does nothing, but neccessary for
		/// iterator semantics.
		/// </summary>
		/// 
		/// <returns>
		/// Returns this iterator (NOP).
		/// </returns> --------------------------------------------------------
		insert_iterator& operator++(int) {
			return *this;
		}

	private:
		collection_& _collection;
		iterator _pos;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// Convienience method to construct a insert_iterator for the given
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
	/// Returns a constructed insert_iterator for the given collection.
	/// </returns> ------------------------------------------------------------
	template <ordered_collection collection_>
	insert_iterator<collection_> inserter(
		collection_& c, 
		typename collection_::const_iterator it
	) {
		return insert_iterator{ c, it };
	}
}