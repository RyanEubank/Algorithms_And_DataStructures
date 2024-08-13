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
#include <iterator>
#include <ranges>

namespace collections {

	struct shift_ {
	private:

		constexpr void _shift(auto begin, auto end, int64_t amount) const {
			while (begin != end) {
				*std::next(begin, amount) = *begin;
				begin++;
			}
		}

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs a shift of the elements in a bidirectional iterator pair 
		/// moving them forward or backwards in the sequence by the specified 
		/// amount.
		/// </summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the bidirectional iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the range being shifted.
		/// </param>
		/// <param name="end">
		/// The end iterator of the range being shifted.
		/// </param>
		/// <param name="shiftAmount">
		/// The amount to shift the range by.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the first shifted element.
		/// </returns> --------------------------------------------------------
		template <
			std::bidirectional_iterator iterator,
			std::sentinel_for<iterator> sentinel
		>
		constexpr auto operator()(
			iterator begin, 
			sentinel end, 
			int64_t shiftAmount
		) const {
			if (shiftAmount > 0) {
				auto b = std::reverse_iterator(end);
				auto e = std::reverse_iterator(begin);
				_shift(b, e, -shiftAmount);
			}
			else 
				_shift(begin, end, shiftAmount);

			return begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs a shift of the elements in a bidirectional range moving
		/// them forward or backwards by the specified amount.
		/// </summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being shifted.
		/// </typeparam>
		/// 
		/// <param name="r">
		/// The range to be shifted.
		/// </param>
		/// <param name="shiftAmount">
		/// The amount or number of steps to shift the range by.
		/// </param>
		/// 
		/// <returns>
		/// Returns an iterator to the first shifted element.
		/// </returns> --------------------------------------------------------
		template<std::ranges::bidirectional_range range>
		constexpr auto operator()(range&& rg, int64_t shiftAmount) const {
			return (*this)
				(std::ranges::begin(rg), std::ranges::end(rg), shiftAmount);
		}
	};

	inline constexpr shift_ shift;
}