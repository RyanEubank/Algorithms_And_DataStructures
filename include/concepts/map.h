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
#include <type_traits>

#include "../util/key_value_pair.h"

namespace collections {

	// --------------------------------------------------------------------
	/// <summary><para>
	/// Defines the miniimal set of typedefs for maps.
	/// </para>
	/// 
	/// Required aliases:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///			key_type
	///		</term></item></para>
	///		<para><item><term>
	///			value_type
	///		</term></item></para>
	///		<para><item><term>
	///			mapped_type
	///		</term></item></para>
	/// </list>
	///  
	/// </summary> --------------------------------------------------------
	template <class T>
	concept map = 
		std::_Is_specialization_v<typename T::value_type, key_value_pair> &&
		requires() {
			typename T::key_type;
			typename T::value_type;
			typename T::mapped_type;
		};

	// --------------------------------------------------------------------
	/// <summary><para>
	/// Defines the miniimal set of methods and operators for multimaps.
	/// </para>
	/// 
	/// Required variables and methods:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///			bool allow_dupicates is true
	///		</term></item></para>
	///		<para><item><term>
	///			size_t count(const key_type&)
	///		</term></item></para>
	/// </list>
	///  
	/// </summary> --------------------------------------------------------
	template <class T>
	concept multimap = map<T> && requires(
		const T& c2, 
		const typename T::key_type& key
	) {
		c2.allow_duplicates == true;
		{ c2.count(key) } -> std::convertible_to<size_t>;
	};
}