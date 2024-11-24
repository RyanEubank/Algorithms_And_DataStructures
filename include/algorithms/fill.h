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
		/// Fills the range given by the iterator pair with the specified value.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being repeated.
		/// </typeparam>
		/// <typeparam name="src_iterator">
		/// The type of the inputr iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="src_sentinel">
		/// The type of the src sentinel or end iterator.
		/// </typeparam>
		/// <typeparam name="dest_iterator">
		/// The type of the output iterator being iterated over.
		/// </typeparam>
		/// <typeparam name="src_sentinel">
		/// The type of the destination sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="src_begin">
		/// The beginning iterator to start repeating from.
		/// </param>
		/// <param name="src_end">
		/// The sentinel or end iterator to stop repeating from.
		/// </param>
		/// <param name="dest_begin">
		/// The beginning iterator to start copying to.
		/// </param>
		/// <param name="dest_end">
		/// The sentinel or end iterator to stop copying to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output begin iterator after it has be iterated to the 
		/// end.
		/// </returns> --------------------------------------------------------
		template <
			std::input_iterator src_iterator,
			std::sentinel_for<src_iterator> src_sentinel,
			class dest_iterator,
			std::sentinel_for<dest_iterator> dest_sentinel
		> requires std::output_iterator<dest_iterator, std::iter_value_t<dest_iterator>>
		constexpr dest_iterator operator()(
			dest_iterator dest_begin,
			dest_sentinel dest_end,
			src_iterator src_begin,
			src_sentinel src_end
		) const {
			auto current = src_begin;
			while (dest_begin != dest_end) {
				*dest_begin++ = *current;
				if (++current == src_end)
					current = src_begin;
			}
			return dest_begin;
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

		// --------------------------------------------------------------------
		/// <summary>
		/// Fills the given range with the specified list of values to repeat.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being repeated.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range being filled.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be filled.
		/// </param>
		/// <param name="values">
		/// The list of values to repeat.
		/// </param>
		/// 
		/// <returns>
		/// Returns the starting iterator of the range after it has been 
		/// iterated to the end.
		/// </returns> --------------------------------------------------------
		template <std::ranges::input_range input, class output>
			requires std::ranges::output_range<output, std::ranges::range_value_t<output>>
		constexpr std::ranges::borrowed_iterator_t<output> operator()(
			output&& dest,
			input&& src
		) const {
			if constexpr (std::ranges::sized_range<input> && 
				std::ranges::sized_range<output>
			) {
				if (std::ranges::size(src) > std::ranges::size(dest))
					throw std::invalid_argument("Sourse range larger than destination.");
			}

			return (*this) (
				std::ranges::begin(dest), 
				std::ranges::end(dest),
				std::ranges::begin(src),
				std::ranges::end(src)
			);
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
		/// <param name="count">
		/// The number of times value is copied over.
		/// </param>
		/// <param name="value">
		/// The value to fill the range with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator after it has been iterated count 
		/// times.
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
		/// Fills the specified destination with repeated copies of values up
		/// to count number of items.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being repeated.
		/// </typeparam>
		/// <typeparam name="iterator">
		/// The type of the output iterator being iterated over.
		/// </typeparam>
		/// 
		/// <param name="destination">
		/// The beginning iterator to start filling at.
		/// </param>
		/// <param name="count">
		/// The number of items to fill or repeat.
		/// </param>
		/// <param name="value">
		/// The range of values to fill the destination with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the destination iterator after it has be iterated to the 
		/// end.
		/// </returns> --------------------------------------------------------
		template <
			class iterator,
			std::ranges::input_range input
		> requires std::output_iterator<iterator, std::iter_value_t<iterator>>
		constexpr iterator operator()(
			iterator destination,
			size_t count,
			input&& values
		) const {
			auto current = values.begin();
			for (size_t i = 0; i < count; ++i) {
				*destination++ = *current;
				if (++current == values.end())
					current = values.begin();
			}
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
			return (*this) (std::ranges::begin(rg), count, value);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Fills the given destination range with the specified number of
		/// repeated copies from the input range.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being inserted.
		/// </typeparam>
		/// <typeparam name="output">
		/// The type of the range being filled.
		/// </typeparam>
		/// 
		/// <param name="dest">
		/// The range to be filled.
		/// </param>
		/// <param name="count">
		/// The number of elements to fill.
		/// </param>
		/// <param name="values">
		/// The range of values to fill the destination with.
		/// </param>
		/// 
		/// <returns>
		/// Returns the starting iterator of the range after it has been 
		/// iterated to the end.
		/// </returns> --------------------------------------------------------
		template <
			std::ranges::input_range input, 
			class output
		> requires 
			std::ranges::output_range<output, std::ranges::range_value_t<output>>
		constexpr std::ranges::borrowed_iterator_t<output> operator()(
			output&& dest,
			size_t count,
			input&& values
		) const {
			return (*this) (std::ranges::begin(dest), count, values);
		}
	};

	inline constexpr fill_ fill;
	inline constexpr fill_n_ fill_n;
}