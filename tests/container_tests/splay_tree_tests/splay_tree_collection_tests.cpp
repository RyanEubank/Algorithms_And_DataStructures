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

#include "../../test_suites/collection_tests.h"
#include "splay_tree_test_fixture.h"

namespace collection_tests {

	using collection_types = instantiate_with_elements<SplayTreeTestTypes>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		SplayTreeTest,
		CollectionTests,
		collection_types
	);
}