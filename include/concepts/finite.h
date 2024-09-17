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

#include <cstdlib>
#include <concepts>
#include <type_traits>

namespace collections {

	// -------------------------------------------------------------------------
	/// <summary><para>
	/// Defines requirements for collections, containers, and adapters with 
	/// finite size.
	/// </para>
	/// 
	/// Required methods:
	/// 
	/// <list type="bullet">
	///		<para><item><term>
	///			- void clear()
	///		</term></item></para>
	///		<para><item><term>
	///			- size_t size() const
	///		</term></item></para>
	///		<para><item><term>
	///			- bool isEmpty() const
	///		</term></item></para>
	/// 	<para><item><term>
	///			- bool max_size() const
	///		</term></item></para>
	///	</list>
	/// 
	/// </summary> -------------------------------------------------------------
	template <class T>
	concept finite = requires(T& obj1, const T& obj2) {
		{ obj1.clear() };
		{ obj2.size() }			-> std::convertible_to<size_t>;
		{ obj2.isEmpty() }		-> std::convertible_to<bool>;
		{ obj2.max_size() }		-> std::convertible_to<size_t>;
	};

}
