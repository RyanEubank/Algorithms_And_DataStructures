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

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// The indexable concept defines an interface and behavior for template 
	/// types that can be accessed by the index operator and maintain
	/// sequential access. Indexable types must have the following properties:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- The index operator for non-const objects returns a reference.
	///		</term></item></para>
	///		<para><item><term>
	///			- The index operator for const objects returns a const 
	///           reference.
	///		</term></item></para>
	///		<para><item><term>
	///			- An 'at' method for non-const objects that checks bounds and 
	///			  returns a reference.
	///		</term></item></para>
	///		<para><item><term>
	///			- An 'at' method for const objects that checks bounds and 
	///			  returns a const reference.
	///		</term></item></para>
	/// </list>
	///  
	/// </summary> ------------------------------------------------------------
	template <class collection_t, class element_t, class index_t>
	concept indexable = requires(
		collection_t& c1, 
		const collection_t& c2, 
		index_t i, 
		const element_t& e
	) {
		{ c1[i] } -> std::same_as<element_t&>;
		{ c2[i] } -> std::same_as<const element_t&>;
		{ c1.at(i) } -> std::same_as<element_t&>;
		{ c2.at(i) } -> std::same_as<const element_t&>;
	};
}