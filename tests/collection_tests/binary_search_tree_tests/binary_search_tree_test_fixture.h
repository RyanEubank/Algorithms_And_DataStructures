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

#include "../../test_suites/collection_test_fixture.h"

#include "containers/BinarySearchTree.h"

namespace collection_tests {

	using namespace collections;

	template <class T>
	struct BinarySearchTreeTestTypes {
		using collection_t	= BinarySearchTree<T, std::less<T>, std::allocator<T>>;
		using mock_t		= BinarySearchTree<T, std::less<T>, MockAllocator<T>>;
	};

	using bst_test_params = instantiate_with_elements<BinarySearchTreeTestTypes>;
}
