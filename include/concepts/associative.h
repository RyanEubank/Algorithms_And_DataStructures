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
	/// The concept associative defines an interface for template container 
	/// types that provide associative access. Associative types must implement
	/// the following methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			insert(element) -> no constraints on return type
	///		</term></item></para>
	///		<para><item><term>
	///			remove(element) -> no constraints on return type
	///		</term></item></para>
	/// </list>
	///  
	/// </summary> ------------------------------------------------------------
	template <class collection_t, class element_t>
	concept associative = requires(collection_t& c, const element_t& e) {
		{ c.insert(e) };
		{ c.remove(e) };
	};
}