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

#include "containers/AVLTree.h"

#include "../../collection_test_suites/assignment_tests.h"

namespace collection_tests {

	using test_params = testing::Types<
		SimpleAVL<uint8_t>,
		SimpleAVL<uint16_t>,
		SimpleAVL<uint32_t>,
		SimpleAVL<uint64_t>,
		SimpleAVL<float>,
		SimpleAVL<void*>,
		SimpleAVL<std::string>,
		SimpleAVL<SimpleAVL<int>>,
		MapAVL<uint8_t, std::string>,
		MultiAVL<uint8_t>,
		MultiMapAVL<uint8_t, std::string>
	>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		AVLTreeTest,
		AssignmentTests,
		test_params
	);
}