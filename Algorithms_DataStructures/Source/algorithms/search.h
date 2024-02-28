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

//	// --------------------------------------------------------------------
//	/// <summary>
//	/// Returns whether the elements of the given collection are all 
//	/// contained by the calling collection.
//	/// </summary>
//	/// 
//	/// <param name="collection">
//	/// - The collection to be searched.
//	/// </param>
//	/// 
//	/// <returns>
//	/// Returns true if the calling collection contains the all the 
//	/// elements of the other collection, false otherwise.
//	/// </returns> --------------------------------------------------------
//	template <std::ranges::input_range T>
//	requires std::ranges::input_range<derived_t>
//	bool containsAll(const T& collection) const {
//		for (const auto& e : collection) {
//			if (!contains(e))
//				return false;
//		}
//		return true;
//	}

//	// --------------------------------------------------------------------
//	/// <summary>
//	/// Returns whether an element that matches the predicate is found. 
//	/// </summary>
//	/// 
//	/// <param name="test">
//	/// - The predicate to match elements against.
//	/// </param>
//	/// 
//	/// <returns>
//	/// Returns true if the collection contains at least one element that
//	/// matches the given predicate.
//	/// </returns> --------------------------------------------------------
//	template <std::predicate<value_type> predicate>
//		requires std::ranges::input_range<derived_t>
//	bool containsMatch(predicate test) const {
//		for (const auto& e : *(THIS_DERIVED)) {
//			if (test(e))
//				return true;
//		}
//		return false;
//	}

//	// --------------------------------------------------------------------
//	/// <summary>
//	/// Returns whether all elements in the collection match the given 
//	/// predicate. 
//	/// </summary>
//	/// 
//	/// <param name="test">
//	/// - The predicate to match elements against.
//	/// </param>
//	/// 
//	/// <returns>
//	/// Returns true if the every element contained in the collection 
//	/// matches the given predicate, returns false if at least one fails.
//	/// </returns> --------------------------------------------------------
//	template <std::predicate<value_type> predicate>
//		requires std::ranges::input_range<derived_t>
//	bool allMatch(predicate test) const {
//		for (const auto& e : *(THIS_DERIVED)) {
//			if (!test(e))
//				return false;
//		}
//		return true;
//	}