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

#include <iostream>

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// The streamable concept requires that template types have a generic
	/// overload for the basic input and output stream operators.
	/// </summary> ------------------------------------------------------------
	template <class collection_t, class char_t>
	concept streamable = requires(
		collection_t& c1,
		const collection_t& c2,
		std::basic_ostream<char_t> &os,
		std::basic_istream<char_t> &is
	) {
		{ os << c2 } -> std::convertible_to<std::basic_ostream<char_t>&>;
		{ is >> c1 } -> std::convertible_to<std::basic_istream<char_t>&>;
	};
}