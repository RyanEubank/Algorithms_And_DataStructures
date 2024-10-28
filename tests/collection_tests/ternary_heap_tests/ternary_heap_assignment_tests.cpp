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

#include <string>
#include <gtest/gtest.h>

#include "adapters/Heap.h"

#include "../../collection_test_suites/assignment_tests.h"

namespace collection_tests {

	using test_params = testing::Types<
		TernaryHeap<uint8_t>,
		TernaryHeap<uint16_t>,
		TernaryHeap<uint32_t>,
		TernaryHeap<uint64_t>,
		TernaryHeap<float>,
		TernaryHeap<void*>,
		TernaryHeap<std::string>,
		TernaryHeap<TernaryHeap<int>>
	>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		TernaryHeapTest,
		AssignmentTests,
		test_params
	);

}