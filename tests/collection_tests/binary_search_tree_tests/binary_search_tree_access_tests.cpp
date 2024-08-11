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

#include "binary_search_tree_test_fixture.h"
#include "../../test_suites/associative_collection_tests/associative_collection_access_tests.h"
#include "../../test_suites/associative_collection_tests/iterator_tests/associative_iterator_access_tests.h"

namespace collection_tests {

	INSTANTIATE_TYPED_TEST_SUITE_P(
		BinarySearchTreeTest,
		AssociativeCollectionAccessTests,
		bst_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		BinarySearchTreeTest,
		AssociativeIteratorAccessTests,
		bst_test_params
	);
}