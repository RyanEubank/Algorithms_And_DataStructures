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

#include <cstdint>
#include <iterator>
#include <ranges>

namespace collections {

	struct index_of_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the index position of the specified element in the given 
		/// sequence, starting at 0.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched.
		/// </typeparam>
		/// <typeparam name="iterator">
		/// The type of the input iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the range.
		/// </param>
		/// <param name="end">
		/// The sentinel or end iterator of the range.
		/// </param>
		/// <param name="value">
		/// The value to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns the index of the element if found, returns -1 otherwise.
		/// </returns> --------------------------------------------------------
		template <
			class T,
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel
		>
		constexpr auto operator()(
			iterator begin,
			sentinel end,
			const T& value
		) const {
			int64_t i = 0;
			while (begin != end) {
				if (*begin++ == value)
					return i;
				i++;
			}
			return -1;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the index position of the specified element in the given 
		/// sequence, starting at 0.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched for.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range being searched.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be searched.
		/// </param>
		/// <param name="value">
		/// The value to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns the index of the element if found, returns -1 otherwise.
		/// </returns>
		template <class T, std::ranges::input_range range>
		constexpr auto operator()(const range& rg, const T& value) const {
			return (*this)
				(std::ranges::begin(rg), std::ranges::end(rg), value);
		}
	};

	inline constexpr index_of_ index_of;
}