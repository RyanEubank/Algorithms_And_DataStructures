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
#include <istream>
#include <ostream>
#include <type_traits>

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// The streamable concept requires that template types have a generic
	/// overload for the basic input and output stream operators.
	/// </summary> ------------------------------------------------------------
	template <class T, class char_t>
	concept streamable = requires(
		T& obj1,
		const T& obj2,
		std::basic_ostream<char_t> &os,
		std::basic_istream<char_t> &is
	) {
		{ os << obj2 } -> std::convertible_to<std::basic_ostream<char_t>&>;
		{ is >> obj1 } -> std::convertible_to<std::basic_istream<char_t>&>;
	};
}