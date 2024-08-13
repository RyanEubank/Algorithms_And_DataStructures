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

	struct move_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Moves the given iterator range to the destination starting at
		/// the given output iterator.
		/// </summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being moved from.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The sentinel or end iterator of the range being moved from.
		/// </typeparam>
		/// <typeparam name="output">
		/// The type of the output iterator being moved to.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the iterator range to move from.
		/// </param>
		/// <param name="end">
		/// The end of the iterator range to copy from.
		/// </param>
		/// <param name="destination">
		/// The output iterator to start copying to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// moved, or the original destination if the range is empty.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel,
			std::weakly_incrementable output
		>
		constexpr output operator()(
			iterator begin,
			sentinel end,
			output destination
		) const {
			while (begin != end)
				*destination++ = std::ranges::iter_move(begin++);
			return destination;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the given range to the destination starting at the output 
		/// iterator.
		/// </summary>
		/// 
		/// <typeparam name="range">
		/// The type of the input range being copied from.
		/// </typeparam>
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range being copied from.
		/// </param>
		/// <param name="destination">
		/// The output iterator to start copying to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// moved, or the original destination if the range is empty.
		/// </returns> --------------------------------------------------------
		template <
			std::ranges::input_range range,
			std::weakly_incrementable output
		>
		constexpr output operator()(
			const range& rg, 
			output destination
		) const {
			return (*this)
				(std::ranges::begin(rg), std::ranges::end(rg), destination);
		}
	};

	struct move_n_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Moves count number of elements from the given iterator to the 
		/// destination.
		/// </summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being moved from.
		/// </typeparam>
		/// <typeparam name="output">
		/// The type of the output iterator being moved to.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the iterator range to move from.
		/// </param>
		/// <param name="count">
		/// The number of elements to move.
		/// </param>
		/// <param name="destination">
		/// The output iterator to start moving to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// moved, or the original destination if the range is empty.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator iterator,
			std::weakly_incrementable output
		>
		constexpr output operator()(
			iterator begin,
			size_t count,
			output destination
		) const {
			for (size_t i = 0; i < count; ++i)
				*destination++ = std::ranges::iter_move(begin++);
			return destination;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the given range to the destination starting at the output 
		/// iterator.
		/// </summary>
		/// 
		/// <typeparam name="range">
		/// The type of the input range being copied from.
		/// </typeparam>
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range being copied from.
		/// </param>
		/// <param name="count">
		/// The number of elements to move.
		/// </param>
		/// <param name="destination">
		/// The output iterator to start copying to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// moved, or the original destination if the range is empty.
		/// </returns> --------------------------------------------------------
		template <
			std::ranges::input_range range,
			std::weakly_incrementable output
		>
		constexpr output operator()(
			const range& rg,
			size_t count,
			output destination
		) const {
			return (*this)
				(std::ranges::begin(rg), count, destination);
		}
	};

	inline constexpr move_ move;
	inline constexpr move_n_ move_n;
}