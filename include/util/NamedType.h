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

#include <type_traits>
#include <utility>

namespace collections {

	// ------------------------------------------------------------------------
	/// <summary>
	/// NamedType is a class to implement strong typing to other types, 
	/// commonly used to differentiate intrinsicly convertible type in an
	/// interface to prevent argument swapping, implicit conversions, etc.
	/// </summary>
	/// 
	/// <typeparam name="T">
	/// The underlying type being referred to by the NamedType.
	/// </typeparam>
	/// 
	/// <typeparam name="tag">
	/// An empty tag type to provide to make the type unique.
	/// </typeparam> ----------------------------------------------------------
	template <typename T, typename tag>
	class NamedType {
	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a NamedType from the given value.
		/// </summary>
		/// 
		/// <param name="value">
		/// The underlying value contained by the NamedType.
		/// </param> ----------------------------------------------------------
		explicit NamedType(T const& value) : value_(value) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Move constructs a NamedType from the given value.
		/// </summary>
		/// 
		/// <param name="value">
		/// The underlying value contained by the NamedType.
		/// </param> ----------------------------------------------------------
		explicit NamedType(T&& value) requires !std::is_reference_v<T> :
			value_(std::move(value)) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the contained value.
		/// </summary> 
		/// 
		/// <returns>
		/// A reference to the underlying value.
		/// </returns> --------------------------------------------------------
		T& get() { return value_; }

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the contained value.
		/// </summary> 
		/// 
		/// <returns>
		/// A reference to the underlying value.
		/// </returns> --------------------------------------------------------
		T const& get() const { return value_; }

	private:
		T value_;
	};
}