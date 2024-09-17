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

#include "NamedType.h"

namespace collections {
	
	/// <summary>
	/// Strongly typed integral to indicate a sequential index.
	/// </summary>
	using Index = NamedType<size_t, struct IndexType>;

	/// <summary>
	/// Strongly typed integral to indicate an amount of memory being reserved.
	/// </summary>
	using Reserve = NamedType<size_t, struct ReserveType>;

	/// <summary>
	/// Strongly typed integral to indicate a size or specific count.
	/// </summary>
	using Size = NamedType<size_t, struct SizeType>;

}