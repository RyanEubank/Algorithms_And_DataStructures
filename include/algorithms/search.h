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

#include "../concepts/associative.h"
#include "../concepts/collection.h"

namespace collections {

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
		constexpr auto operator()(range&& rg, const T& value) const {
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
		template <class T, associative collection>
		constexpr auto operator()(collection&& c, const T& value) const {
			return c.find(value);
		}
	};

	struct find_if_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the first iterator in the specified range that matches
		/// the given predicate.
		/// </summary>
		/// 
		/// <typeparam name="predicate">
		/// The type of the predicate function to match against elements.
		/// </typeparam>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being iterated over.
		/// </typeparam>
		/// 
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the range.
		/// </param>
		/// 
		/// <param name="end">
		/// The sentinel or end iterator of the range.
		/// </param>
		/// 
		/// <param name="p">
		/// The predicate to match against.
		/// </param>
		/// 
		/// <returns>
		/// Returns at iterator to the first matching element, otherwise returns 
		/// the end iterator.
		/// </returns> --------------------------------------------------------
		template <
			class predicate,
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel
		> requires std::predicate<predicate, std::iter_value_t<iterator>>
		constexpr auto operator()(
			iterator begin,
			sentinel end,
			predicate p
		) const {
			while (begin != end) {
				if (p(*begin++))
					break;
			}
			return begin;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the first iterator in the specified range that matches
		/// the given predicate.
		/// </summary>
		/// 
		/// <typeparam name="predicate">
		/// The type of the predicate function to match against elements.
		/// </typeparam>
		/// 
		/// <typeparam name="range">
		/// The type of the range to be searched.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be searched.
		/// </param>
		/// 
		/// <param name="p">
		/// The predicate to match against.
		/// </param>
		/// 
		/// <returns>
		/// Returns at iterator to the first matching element, otherwise returns 
		/// the end iterator of the range.
		/// otherwise.
		/// </returns> --------------------------------------------------------
		template <
			class predicate, 
			std::ranges::input_range range
		> requires std::predicate<predicate, std::ranges::range_value_t<range>>
		constexpr auto operator()(range&& rg, predicate p) const {
			return (*this)(std::ranges::begin(rg), std::ranges::end(rg), p);
		}
	};

	inline constexpr find_ find;
	inline constexpr find_if_ find_if;

	struct contains_ {

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns whether the specified iterator range contains an element
		/// equal to the given value.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched.
		/// </typeparam>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being iterated over.
		/// </typeparam>
		/// 
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the range.
		/// </param>
		/// 
		/// <param name="end">
		/// The sentinel or end iterator of the range.
		/// </param>
		/// 
		/// <param name="value">
		/// The value to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if an equivalent element exists, false otherwise.
		/// </returns> ---------------------------------------------------------
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
			return find(begin, end, value) != end;
		}
		
		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns whether the specified iterator range contains an element
		/// equal to the given value.
		/// </summary>
		/// 
		/// <typeparam name="T">
		/// The type of the value being searched for.
		/// </typeparam>
		/// 
		/// <typeparam name="range">
		/// The type of the range to be searched.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be searched.
		/// </param>
		/// 
		/// <param name="value">
		/// The value to search for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if an equivalent element exists, false otherwise.
		/// </returns> ---------------------------------------------------------
		template <class T, std::ranges::input_range range>
		constexpr auto operator()(range&& rg, const T& value) const {
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
		template <class T, associative collection>
		constexpr auto operator()(collection&& c, const T& value) const {
			return c.find(value) != c.end();
		}
	};

	struct contains_match_ {

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether an element that matches the specified predicate
		/// exists in the given iterator range.
		/// </summary>
		/// 
		/// <typeparam name="predicate">
		/// The type of the predicate function to match against elements.
		/// </typeparam>
		/// 
		/// <typeparam name="iterator">
		/// The type of the input iterator being iterated over.
		/// </typeparam>
		/// 
		/// <typeparam name="sentinel">
		/// The type of the sentinel or end iterator.
		/// </typeparam>
		/// 
		/// <param name="begin">
		/// The beginning iterator of the range.
		/// </param>
		/// 
		/// <param name="end">
		/// The sentinel or end iterator of the range.
		/// </param>
		/// 
		/// <param name="p">
		/// The predicate to match elements against.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the given value is found within the range, false 
		/// otherwise.
		/// </returns> --------------------------------------------------------
		template <
			class predicate,
			std::input_iterator iterator,
			std::sentinel_for<iterator> sentinel
		> requires std::predicate<predicate, std::iter_value_t<iterator>>
		constexpr bool operator()(
			iterator begin,
			sentinel end,
			predicate p
		) const {
			return find_if(begin, end, p) != end;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns whether an element that matches the specified predicate
		/// exists in the given range.
		/// </summary>
		/// 
		/// <typeparam name="predicate">
		/// The type of the predicate function to match against elements.
		/// </typeparam>
		/// 
		/// <typeparam name="range">
		/// The type of the range to be searched.
		/// </typeparam>
		/// 
		/// <param name="rg">
		/// The range to be searched.
		/// </param>
		/// 
		/// <param name="p">
		/// The predicate to match against.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if a matching element exists, false otherwise.
		/// otherwise.
		/// </returns> ---------------------------------------------------------
		template <
			class predicate,
			std::ranges::input_range range
		> requires std::predicate<predicate, std::ranges::range_value_t<range>>
		constexpr bool operator()(range&& rg, predicate p) const {
			return (*this)(std::ranges::begin(rg), std::ranges::end(rg), p);
		}
	};

	inline constexpr contains_ contains;
	inline constexpr contains_match_ contains_match;
}