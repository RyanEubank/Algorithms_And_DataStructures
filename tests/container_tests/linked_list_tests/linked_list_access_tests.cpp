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

#include "../../test_suites/sequential_collection_tests/sequential_collection_access_tests.h"
#include "../../test_suites/sequential_collection_tests/index_tests/sequential_index_access_tests.h"
#include "../../test_suites/sequential_collection_tests/iterator_tests/sequential_iterator_access_tests.h"
#include "linked_list_test_fixture.h"

namespace collection_tests {

	using collection_types = instantiate_with_elements<LinkedListTestTypes>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		LinkedListTest,
		SequentialCollectionAccessTests,
		collection_types
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		LinkedListTest,
		SequentialIndexAccessTests,
		collection_types
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		LinkedListTest,
		SequentialIteratorAccessTests,
		collection_types
	);
}