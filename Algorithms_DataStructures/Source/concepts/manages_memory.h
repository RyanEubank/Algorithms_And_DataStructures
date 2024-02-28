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
#include <concepts>

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// The concept is_managed defines behaviour for template types that must
	/// at minimum implement the Rule of 5 for memory management plus
	/// a swap method:
	/// 
	///	<list type = "bullet">
	///		<para><item><term>
	///			- The type is copy constructible,
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is move constructible.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is copy assignable.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is move assignable.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is destructible.
	///		</term></item></para>
	///		<para><item><term>
	///			- The type is swappable.
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class collection_t>
	concept manages_memory =
		std::copy_constructible<collection_t> &&
		std::move_constructible<collection_t> &&
		std::is_copy_assignable_v<collection_t> &&
		std::is_move_assignable_v<collection_t> &&
		std::is_destructible_v<collection_t> &&
		std::is_swappable_v<collection_t>;
}