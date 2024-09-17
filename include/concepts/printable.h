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

#include <cstdint>
#include <cstdlib>
#include <ostream>

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// Defines the requirements for types that provide a method to print to an 
	/// output stream in human readable format.
	/// </summary> ------------------------------------------------------------
	template <class T, class char_t>
	concept printable = requires (
		const T& obj,
		std::basic_ostream<uint8_t>& os,
		size_t limit
	) {
		{ obj.print(os, limit) };
	};
}