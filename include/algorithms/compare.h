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
#include <ranges>

namespace collections {

	struct lexicographic_compare_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs a lexicographic comparison of the given iterator range.
		/// </summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin_1">
		/// The beginning iterator of the first range being compared.
		/// </param>
		/// <param name="end_1">
		/// The end iterator of the first range being compared.
		/// </param>
		/// <param name="begin_2">
		/// The beginning iterator of the second range being compared.
		/// </param>
		/// <param name="end_2">
		/// The end iterator of the second range being compared.
		/// </param>
		/// 
		/// <returns>
		/// Returns a std::ordering or subtype signalling the ordered 
		/// comparison of the range.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator iterator, 
			std::sentinel_for<iterator> sentinel
		>
		constexpr auto operator()(
			iterator begin_1, 
			sentinel end_1, 
			iterator begin_2, 
			sentinel end_2
		) const {
			auto comparison = decltype(*begin_1 <=> *begin_2) {};

			while (begin_1 != end_1 && begin_2 != end_2) {
				comparison = (*begin_1++ <=> *begin_2++);
				if (comparison != 0)
					return comparison;
			}

			return comparison;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs a lexicographic comparison of the given range.
		/// </summary>
		/// 
		/// <typeparam name="range">
		/// The type of the range being compared.
		/// </typeparam>
		/// 
		/// <param name="r1">
		/// The first range being compared.
		/// </param>
		/// <param name="r2">
		/// The second range being compared.
		/// </param>
		/// 
		/// <returns>
		/// Returns a std::ordering or subtype signalling the ordered 
		/// comparison of the range.
		/// </returns> --------------------------------------------------------
		template <std::ranges::input_range range>
		constexpr auto operator()(const range& r1, const range& r2) const {

			if constexpr (std::ranges::sized_range<range>) {
				using compare_type = decltype(
					*std::ranges::begin(r1) <=> *std::ranges::begin(r2)
				);

				auto compare = std::ranges::size(r1) <=> std::ranges::size(r2);
				if (compare != 0)
					return static_cast<compare_type>(compare);
			}

			return (*this)(
				std::ranges::begin(r1), 
				std::ranges::end(r1), 
				std::ranges::begin(r2),
				std::ranges::end(r2)
			);
		}
	};

	inline constexpr lexicographic_compare_ lexicographic_compare;
}