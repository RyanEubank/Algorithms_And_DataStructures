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

#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "containers/AVLTree.h"

#include "../../collection_test_suites/access_tests/associative_bound_tests.h"
#include "../../collection_test_suites/access_tests/associative_search_tests.h"
#include "../../collection_test_suites/access_tests/bag_tests.h"
#include "../../collection_test_suites/access_tests/map_tests.h"

namespace collection_tests {

	using tree_test_params = testing::Types <
		SimpleAVL<std::string>,
		MapAVL<uint8_t, std::string>,
		MultiAVL<std::string>,
		MultiMapAVL<uint8_t, std::string>
	>;

	using bag_test_params = testing::Types<
		MultiAVL<std::string>,
		MultiMapAVL<uint8_t, std::string>
	>;

	using map_test_params = testing::Types<
		MapAVL<uint8_t, std::string>,
		MultiMapAVL<uint8_t, std::string>
	>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		AVLTreeTest,
		AssociativeSearchTests,
		tree_test_params
	);


	INSTANTIATE_TYPED_TEST_SUITE_P(
		SplayTreeTest,
		AssociativeBoundTests,
		tree_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		SplayTreeTest,
		BagTests,
		bag_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		SplayTreeTest,
		MapTests,
		map_test_params
	);
}