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

#include <cstdlib>
#include <concepts>
#include <iterator>
#include <ostream>
#include <ranges>

#include "../concepts/collection.h"

namespace collections {

	struct stream_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Writes the given iterator range to the given stream object; The 
		/// first value written is always the size of the range.
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The character type of the stream being written to.
		/// </typeparam>
		/// <typeparam name="iterator">
		/// The type of the input iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the object being written out.
		/// </param>
		/// <param name="end">
		/// The end iterator of the object being written out.
		/// </param> 		
		/// <param name="os">
		/// The stream to write to.
		/// </param> ----------------------------------------------------------
		template <
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel, 
			class char_t
		>
		constexpr void operator()(
			iterator begin, 
			sentinel end, 
			size_t size,
			std::basic_ostream<char_t>& os
		) const {
			os << size << " ";

			while (begin != end) {
				os << *begin++;
				if (begin != end)
					os << " ";
			}

			os << std::endl;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Writes the given input range to the given stream object; The first
		/// value written is always the size of the range.
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The character type of the stream being written to.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range bein g written out.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to write to the stream.
		/// </param>
		/// <param name="os">
		/// The stream to write to.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range range, class char_t>
		constexpr void operator()(
			const range& r, std::basic_ostream<char_t>& os
		) const {
			return (*this)(
				std::ranges::begin(r), 
				std::ranges::end(r), 
				std::ranges::size(r),
				os
			);
		}
	};

	inline constexpr stream_ stream;
}