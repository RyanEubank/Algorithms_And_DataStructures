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

#include <gtest/gtest.h>

#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using MapTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(MapTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection's iterators can be modified to change the
	/// value of an element but not its key.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(MapTests, IteratorsCanUpdateValueButNotKey) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

		auto first = obj.begin();
		auto second = std::next(obj.begin());

		ASSERT_NE(first->key(), second->key());
		ASSERT_NE(first->value(), second->value());

		//obj.updateValue(first, second->value())

		//EXPECT_NE((*first).key(), (*second).key());
		//EXPECT_EQ((*first).value(), (*second).value());

		auto& key = (*first).key();
		EXPECT_TRUE(std::is_const_v<std::remove_reference_t<decltype(key)>>);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		MapTests,
		IteratorsCanUpdateValueButNotKey
	);
}