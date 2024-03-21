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
#include <functional>

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// The searchable_interface concept defines an interface for template 
	/// collections that can be searched for elements, and searched by 
	/// predicate. Searchable template types must expose the following methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			contains(element) -> returns a boolean or equivalent.
	///		</term></item></para>
	///		<para><item><term>
	///			containsMatch(predicate) -> returns a boolean or equivalent.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class collection_t>
	concept searchable_interface = requires(
		const collection_t& c,
		typename collection_t::const_reference e,
		std::function<bool(typename collection_t::value_type)> predicate
	) {
			{ c.contains(e) } -> std::convertible_to<bool>;
			{ c.containsMatch(predicate) } -> std::convertible_to<bool>;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// The searchable concept defines a requirement that collections with
	/// equality comparable elements may be searched via the collections
	/// search algorithms either by implementing searchable_interface or by
	/// being a compatible input range.
	/// </summary> ------------------------------------------------------------
	template <class collection_t>
	concept searchable = 
		(std::equality_comparable<typename collection_t::value_type> && 
			(searchable_interface<collection_t> || std::ranges::input_range<collection_t>)) 
		|| !std::equality_comparable<typename collection_t::value_type>;
}
