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

#include "containers/BinarySearchTree.h"

#include "../../collection_test_suites/access_tests/associative_bound_tests.h"
#include "../../collection_test_suites/access_tests/associative_search_tests.h"
#include "../../collection_test_suites/access_tests/bag_tests.h"
#include "../../collection_test_suites/access_tests/map_tests.h"

namespace collection_tests {

	using tree_test_params = testing::Types <
		SimpleBST<std::string>,
		MapBST<uint8_t, std::string>,
		MultiBST<std::string>,
		MultiMapBST<uint8_t, std::string>
	>;

	using bag_test_params = testing::Types<
		MultiBST<std::string>,
		MultiMapBST<uint8_t, std::string>
	>;

	using map_test_params = testing::Types<
		MapBST<uint8_t, std::string>,
		MultiMapBST<uint8_t, std::string>
	>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		BinarySearchTreeTest,
		AssociativeSearchTests,
		tree_test_params
	);


	INSTANTIATE_TYPED_TEST_SUITE_P(
		BinarySearchTreeTest,
		AssociativeBoundTests,
		tree_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		BinarySearchTreeTest,
		BagTests,
		bag_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		BinarySearchTreeTest,
		MapTests,
		map_test_params
	);
}