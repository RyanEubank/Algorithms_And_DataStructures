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

#include <concepts>
#include <iterator>
#include <ranges>

#include "../concepts/collection.h"

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
		/// <typeparam name="sentinel">
		/// The sentinel or end iterator of the range being copied from.
		/// </typeparam>
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the iterator range to copy from.
		/// </param>
		/// <param name="end">
		/// The end of the iterator range to copy from.
		/// </param>
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
		/// <returns>
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

	inline constexpr copy_ copy;

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
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning of the iterator range to copy from.
		/// </param>
		/// <param name="count">
		/// The number of elements to copy.
		/// </param>
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
		/// <typeparam name="output">
		/// The type of the output iterator being copied to.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range being copied from.
		/// </param>
		/// <param name="count">
		/// The number of elements to copy.
		/// </param>
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

	inline constexpr copy_n_ copy_n;

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

	inline constexpr fill_ fill;

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

	inline constexpr fill_n_ fill_n;

	struct find_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the given value if found in the specified
		/// iterator pair.
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
		/// Returns a valid iterator to the searched element or the end 
		/// iterator if the value is not found.
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
			while (begin != end) {
				if (*begin == value)
					break;
				begin++;
			}
			return begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the given value if found in the specified
		/// range.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched for.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range to be searched.
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
		/// Returns a valid iterator to the searched element or the end 
		/// iterator if the value is not found.
		/// </returns> --------------------------------------------------------
		template <class T, std::ranges::input_range range>
		constexpr auto operator()(const range& rg, const T& value) const {
			return (*this)(std::ranges::begin(rg), std::ranges::end(rg), value);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the given value if found within the 
		/// associative collection.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched for.
		/// </typeparam>
		/// <typeparam name="collection">
		/// The type of the collection to be searched.
		/// </typeparam>
		/// 
		/// <param name="c">
		/// The collection to be searched.
		/// </param>
		/// <param name="value">
		/// The value to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns a valid iterator to the searched element or the end 
		/// iterator if the value is not found.
		/// </returns> --------------------------------------------------------
		template <class T, associative_collection collection>
		constexpr auto operator()(const collection& c, const T& value) const {
			return c.find(value);
		}
	};

	inline constexpr find_ find;

	struct find_if_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the given value is found within the given iterator
		/// range.
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
		/// Returns true if the given value is found within the range, false 
		/// otherwise.
		/// </returns> --------------------------------------------------------
		template <
			class T,
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel,
			std::predicate <T, std::iter_value_t<iterator>> Predicate
		>
		constexpr auto operator()(
			iterator begin,
			sentinel end,
			Predicate p
		) const {
			while (begin != end) {
				if (p(*begin++))
					break;
			}
			return begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the given value is found within the given range.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched for.
		/// </typeparam>
		/// <typeparam name="range">
		/// The type of the range to be searched.
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
		/// Returns true if the value is contained by the range, false 
		/// otherwise.
		/// </returns> --------------------------------------------------------
		template <
			class T, 
			std::ranges::input_range range, 
			std::predicate <T, std::ranges::range_value_t<range>> Predicate
		>
		constexpr auto operator()(const range& rg, Predicate p) const {
			return (*this)(std::ranges::begin(rg), std::ranges::end(rg), p);
		}
	};

	inline constexpr find_if_ find_if;

	struct index_of_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the index of the requested element.
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
		/// Returns the index of the requested element.
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

	inline constexpr move_ move;

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

	inline constexpr move_n_ move_n;

	struct shift_ {
	private:
		
		constexpr void doShift(auto begin, auto end, int64_t amount) const {
			while (begin != end) {
				*std::next(begin, amount) = *begin;
				begin++;
			}
		}

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs a shift of the range specified by the given iterator pair
		/// (begin, end].
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
				doShift(b, e, -shiftAmount);
			}
			else 
				doShift(begin, end, shiftAmount);

			return begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Performs a shift of the given range.
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