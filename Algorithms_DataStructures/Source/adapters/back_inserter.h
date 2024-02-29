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
#include <vector>

#include "../concepts/list.h"
#include "../containers/DynamicArray.h"

namespace collections {

	template <class list_t>
	class back_insert_iterator {
	public:
		
		using value_type = void;
		using difference_type = std::ptrdiff_t;
		using pointer = void;
		using reference = void;
		using iterator_category = std::output_iterator_tag;

		explicit back_insert_iterator(list_t& l) : _list(l) {}
		back_insert_iterator(const back_insert_iterator& it) = default;
		back_insert_iterator(back_insert_iterator&& it) = default;
		~back_insert_iterator() = default;

		back_insert_iterator& operator=(const back_insert_iterator& other) {
			_list = other._list;
			return *this;
		}

		back_insert_iterator& operator=(
			const typename list_t::value_type& value
		) {
			_list.insertBack(value);
			return *this;
		}

		//back_insert_iterator& operator=(
		//	typename list_t::value_type&& value
		//) {
		//	_list.insertBack(std::move(value));
		//	return *this;
		//}

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
		list_t& _list;

	};

	//static_assert(
	//	std::output_iterator<back_insert_iterator<DynamicArray<int>>, int>,
	//	"back_insert_iterator does not meet output iterator requirements."
	//);

	template <list list_t>
	back_insert_iterator<list_t> back_inserter(list_t& l) {
		return back_insert_iterator{ l };
	}
}