/* ============================================================================
 * Copyright (C) 2023  Ryan Eubank
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
#include <iostream>

namespace collections {


	template <class T>
	void shiftArray(T* begin, T* end, int64_t shiftAmount) {
		if (shiftAmount > 0) {
			for (T* p = end - 1; p >= begin; --p)
				*(p + shiftAmount) = *p;
		}
		else {
			for (T* p = begin; p != end; ++p)
				*(p + shiftAmount) = *p;
		}
	}
}