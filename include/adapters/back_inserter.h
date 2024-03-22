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

	template <class collection>
	class back_insert_iterator {
	public:
		
		using value_type = void;
		using difference_type = std::ptrdiff_t;
		using pointer = void;
		using reference = void;
		using iterator_category = std::output_iterator_tag;

		explicit back_insert_iterator(collection& c) : _collection(c) {}
		back_insert_iterator(const back_insert_iterator& it) = default;
		back_insert_iterator(back_insert_iterator&& it) = default;
		~back_insert_iterator() = default;

		back_insert_iterator& operator=(const back_insert_iterator& other) {
			_collection = other._collection;
			return *this;
		}

		back_insert_iterator& operator=(
			const typename collection::value_type& value
		) {
			_collection.insertBack(value);
			return *this;
		}

		back_insert_iterator& operator=(
			typename collection::value_type&& value
		) {
			_collection.insertBack(std::move(value));
			return *this;
		}

		back_insert_iterator& operator*() {
			return *this;
		}

		back_insert_iterator& operator++() {
			return *this;
		}

		back_insert_iterator& operator++(int) {
			return *this;
		}

	private:
		collection& _collection;

	};

	template <sequential collection>
	back_insert_iterator<collection> back_inserter(collection& c) {
		return back_insert_iterator{ c };
	}
}