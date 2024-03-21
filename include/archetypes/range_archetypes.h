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
#include <ranges>

#include "iterator_archetypes.h"

namespace collections {

	template <class T>
	class input_range_archetype {
	private:
		using archetype = input_range_archetype<T>;
		using iterator = input_iterator_archetype<T>;

	public:

		input_range_archetype() = delete;
		~input_range_archetype() = delete;
		input_range_archetype(const archetype&) = delete;
		input_range_archetype(archetype&&) = delete;

		archetype& operator=(const archetype&) = delete;
		archetype& operator=(archetype&&) = delete;
		void operator&() const = delete;

		iterator begin() {
			return iterator{};
		}

		iterator end() {
			return iterator{};
		}

		friend void operator,(const archetype&, const archetype&) = delete;
	};

	static_assert(std::ranges::input_range<input_range_archetype<int>>);
}