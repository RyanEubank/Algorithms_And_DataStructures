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

	struct copy_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the given iterator range to the destination starting at
		/// the given output iterator.
		/// </summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being copied from.
		/// </typeparam>
		/// 
		/// <typeparam name="sentinel">
		/// The sentinel or end iterator of the range being copied from.
		/// </typeparam>
		/// 
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the iterator range to copy from.
		/// </param>
		/// 
		/// <param name="end">
		/// The end of the iterator range to copy from.
		/// </param>
		/// 
		/// <param name="destination">
		/// The destination to copy to.
		/// </param> 
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// copied, or the original destination if the range is empty.
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
				*destination++ = *begin++;
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
		/// 
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range being copied from.
		/// </param>
		/// 
		/// <param name="destination">
		/// The output iterator to start copying to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// copied, or the original destination if the range is empty.
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

	struct copy_n_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the specified number of values from the iterator range to
		/// the given destination.
		/// </summary>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being copied from.
		/// </typeparam>
		/// 
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the iterator range to copy from.
		/// </param>
		/// 
		/// <param name="count">
		/// The number of elements to copy.
		/// </param>
		/// 
		/// <param name="destination">
		/// The destination to copy to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// copied, or the original destination if count equals zero.
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
				*destination++ = *begin++;
			return destination;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Copies the specified number of values from the range to the 
		/// destination.
		/// </summary>
		/// 
		/// <typeparam name="range">
		/// The type of the input range being copied from.
		/// </typeparam>
		/// 
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range being copied from.
		/// </param>
		/// 
		/// <param name="count">
		/// The number of elements to copy.
		/// </param>
		/// 
		/// <param name="destination">
		/// The output iterator to start copying to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator pointing past the last element
		/// copied, or the original destination if count equals zero.
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
			return (*this)(std::ranges::begin(rg), count, destination);
		}
	};

	inline constexpr copy_ copy;
	inline constexpr copy_n_ copy_n;
}