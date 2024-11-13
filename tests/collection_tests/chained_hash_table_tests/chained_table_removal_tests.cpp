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
#include "containers/ChainingHashTable.h"
#include "containers/DynamicArray.h"
#include "containers/ForwardList.h"
#include "containers/LinkedList.h"

#include "../../collection_test_suites/removal_tests/associative_removal_tests.h"

namespace collection_tests {

	using test_params = testing::Types <
		ChainedHashSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, DynamicArray<uint8_t>>,
		ChainedHashSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, SimpleBST<uint8_t>>,
		ChainedHashSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, ForwardList<uint8_t>>,
		ChainedHashSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, LinkedList<uint8_t>>,

		ChainedHashMap<uint8_t, std::string, std::hash, std::modulus, std::equal_to, std::allocator, MapBST<uint8_t, std::string>>,
		ChainedHashMap<uint8_t, std::string, std::hash, std::modulus, std::equal_to, std::allocator, ForwardList<key_value_pair<const uint8_t, std::string>>>,
		ChainedHashMap<uint8_t, std::string, std::hash, std::modulus, std::equal_to, std::allocator, LinkedList<key_value_pair<const uint8_t, std::string>>>,

		ChainedHashMultiSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, MultiBST<uint8_t>>,
		ChainedHashMultiSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, ForwardList<uint8_t>>,
		ChainedHashMultiSet<uint8_t, std::hash, std::modulus, std::equal_to, std::allocator, LinkedList<uint8_t>>,

		ChainedHashMultiMap<uint8_t, std::string, std::hash, std::modulus, std::equal_to, std::allocator, MultiMapBST<uint8_t, std::string>>,
		ChainedHashMultiMap<uint8_t, std::string, std::hash, std::modulus, std::equal_to, std::allocator, ForwardList<key_value_pair<const uint8_t, std::string>>>,
		ChainedHashMultiMap<uint8_t, std::string, std::hash, std::modulus, std::equal_to, std::allocator, LinkedList<key_value_pair<const uint8_t, std::string>>>
	>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ChainedHashTableTest,
		AssociativeRemovalTests,
		test_params
	);
}