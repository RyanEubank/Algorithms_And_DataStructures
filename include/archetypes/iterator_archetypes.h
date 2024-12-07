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

#include <iterator>

namespace collections {

	template <class T>
	class input_iterator_archetype {
	private:
		using archetype = input_iterator_archetype<T>;

	public:

		using value_type = T;
		using pointer = T*;
		using difference_type = std::ptrdiff_t;

		struct reference {
			operator const T& () const { return {}; }
		};

		input_iterator_archetype() = default;
		~input_iterator_archetype() = default;
		input_iterator_archetype(const archetype&) = default;
		input_iterator_archetype(archetype&&) = default;
		archetype& operator=(const archetype&) = default;
		archetype& operator=(archetype&&) = default;
		void operator&() const = delete;
		reference operator*() const { return reference();  }
		archetype& operator++() { return *this; }
		archetype operator++(int) { return *this; }

		friend void operator,(const archetype&, const archetype&) = delete;
		friend bool operator==(const archetype& lhs, const archetype& rhs) { return true; }
	};

	static_assert(std::input_iterator<input_iterator_archetype<int>>);
}