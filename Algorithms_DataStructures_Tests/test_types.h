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

#include "pch.h"

namespace collection_tests {

	template <template <class> class T>
	using instantiate_with_elements = testing::Types<
		T<uint8_t>,
		T<uint16_t>,
		T<uint32_t>,
		T<uint64_t>,
		T<float>,
		T<double>,
		T<void*>,
		T<std::string>,
		T<typename T<uint8_t>::collection_t>
	>;
}