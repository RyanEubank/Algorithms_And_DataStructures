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

#include "../../test_suites/sequential_collection_tests/sequential_collection_insert_tests.h"
#include "../../test_suites/sequential_collection_tests/index_tests/sequential_index_insert_tests.h"
#include "../../test_suites/sequential_collection_tests/iterator_tests/sequential_iterator_insert_tests.h"
#include "forward_list_test_fixture.h"

namespace collection_tests {

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ForwardListTest,
		SequentialCollectionInsertTests,
		forward_list_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ForwardListTest,
		SequentialIndexInsertTests,
		forward_list_test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ForwardListTest,
		SequentialIteratorInsertTests,
		forward_list_test_params
	);
}