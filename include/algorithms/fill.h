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

	struct fill_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Fills the range given by the iterator pair with the specified value.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being inserted.
		/// </typeparam>
		/// <typeparam name="iterator">
		/// The type of the output iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator to start filling at.
		/// </param>
		/// <param name="end">
		/// The sentinel or end iterator to stop filling at.
		/// </param>
		/// <param name="value">
		/// The value to fill the range with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the start iterator after it has be iterated to the end.
		/// </returns> --------------------------------------------------------
		template <
			class T,
			std::output_iterator<const T&> iterator,
			std::sentinel_for<iterator> sentinel
		>
		constexpr iterator operator()(
			iterator begin,
			sentinel end,
			const T& value
		) const {
			while (begin != end)
				*begin++ = value;
			return begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Fills the given range with the specified value.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being inserted.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range being filled.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be filled.
		/// </param>
		/// <param name="value">
		/// The value to fill the range with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the starting iterator of the range after it has been 
		/// iterated to the end.
		/// </returns> --------------------------------------------------------
		template <class T, std::ranges::output_range<const T&> range>
		constexpr std::ranges::borrowed_iterator_t<range> operator()(
			range&& rg,
			const T& value
		) const {
			return (*this)
				(std::ranges::begin(rg), std::ranges::end(rg), value);
		}
	};

	struct fill_n_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Fills count number of values starting at the given iterator.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being inserted.
		/// </typeparam>
		/// <typeparam name="iterator">
		/// The type of the output iterator being iterated over.
		/// </typeparam>
		/// 
		/// <param name="destination">
		/// The beginning iterator to start filling at.
		/// </param>
		/// <param name="end">
		/// The sentinel or end iterator to stop filling at.
		/// </param>
		/// <param name="value">
		/// The value to fill the range with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the start iterator after it has be iterated to the end.
		/// </returns> --------------------------------------------------------
		template <
			class T,
			std::output_iterator<const T&> iterator
		>
		constexpr iterator operator()(
			iterator destination,
			size_t count,
			const T& value
		) const {
			for (size_t i = 0; i < count; ++i)
				*destination++ = value;
			return destination;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Fills the given range with the specified value.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being inserted.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range being filled.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be filled.
		/// </param>
		/// <param name="count">
		/// The number of elements to fill.
		/// </param>
		/// <param name="value">
		/// The value to fill the range with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the starting iterator of the range after it has been 
		/// iterated to the end.
		/// </returns> --------------------------------------------------------
		template <class T, std::ranges::output_range<const T&> range>
		constexpr std::ranges::borrowed_iterator_t<range> operator()(
			range&& rg,
			size_t count,
			const T& value
		) const {
			return (*this)
				(std::ranges::begin(rg), count, value);
		}
	};

	inline constexpr fill_ fill;
	inline constexpr fill_n_ fill_n;
}