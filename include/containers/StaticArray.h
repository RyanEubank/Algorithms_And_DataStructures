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

#include <iterator>

#include "../concepts/collection.h"
#include "../util/NamedType.h"

namespace collections {

	template <class element_t, size_t N> 
	class StaticArray {
	public:

		using value_type		= element_t;
		using reference			= value_type&;
		using const_reference	= const value_type&;
		using iterator			= value_type*;
		using const_iterator	= const value_type*;
		using difference_type	= std::iterator_traits<iterator>::difference_type;
		using size_type			= size_t;
		using pointer			= value_type*;
		using const_pointer		= const value_type*;

	private:

		void validateIndex(size_type index) const {
			if (index >= N)
				throw std::out_of_range("Index out of range.\n");
		}

	public:

		value_type _array[N];

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Index Operator ~~~
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// array.
		/// </returns> --------------------------------------------------------
		constexpr reference operator[](size_type index) {
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Index Operator ~~~
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a const reference to the element at the specified index 
		/// in the array.
		/// </returns> --------------------------------------------------------
		constexpr const_reference operator[](size_type index) const {
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the internal array managed by the StaticArray.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the internal array as a raw pointer.
		/// </returns> --------------------------------------------------------
		constexpr pointer asRawArray() noexcept {
			return this->_array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the internal array managed by the StaticArray.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the internal array as a constant pointer.
		/// </returns> --------------------------------------------------------
		constexpr const_pointer asRawArray() const noexcept {
			return this->_array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the size of the array.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the size NTTP, (an unsigned integer) that the static array
		/// was constructed with.
		/// </returns> --------------------------------------------------------
		constexpr size_type size() const noexcept {
			return N;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Safely indexes into the StaticArry checking the bounds before
		/// returning the result.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index in the
		/// array.
		/// </returns> --------------------------------------------------------
		constexpr reference at(size_type index) {
			validateIndex(index);
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Safely indexes into the StaticArry checking the bounds before
		/// returning the result.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the element to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a constant reference to the element at the specified index 
		/// in the array.
		/// </returns> --------------------------------------------------------
		constexpr const_reference at(size_type index) const {
			validateIndex(index);
			return _array[index];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the StaticArray.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a random access iterator to the first element in the 
		/// StaticArray.
		/// </returns> --------------------------------------------------------
		constexpr iterator begin() noexcept {
			return _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the StaticArray.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a random access iterator to the location after the last 
		/// element in the StaticArray.
		/// </returns> --------------------------------------------------------
		constexpr iterator end() noexcept {
			return _array + N;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the 
		/// StaticArray.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the first element in 
		/// the StaticArray.
		/// </returns> --------------------------------------------------------
		constexpr const_iterator begin() const noexcept {
			return _array;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the StaticArray.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant random access iterator to the location after the 
		/// last element in the StaticArray.
		/// </returns> --------------------------------------------------------
		constexpr const_iterator end() const noexcept {
			return _array + N;
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given StaticArrays.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first array to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second array to be swapped.
		/// </param> ----------------------------------------------------------
		constexpr friend void swap(
			StaticArray& a,
			StaticArray& b
		) noexcept {
			using std::swap;
			swap(a._array, b._array);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Spaceship Operator ~~~
		/// <para>
		/// Performs ordered comparison using default memberwise comaparison
		/// via '<=>'.
		/// </para></summary>
		/// 
		/// <param name="lhs">
		/// The array appearing on the left hand side of the comparison.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The array appearing on the right hand side of the comparison.
		/// </param> ----------------------------------------------------------
		constexpr friend auto operator<=>(
			const StaticArray& lhs,
			const StaticArray& rhs
		) noexcept = default;
	};
}