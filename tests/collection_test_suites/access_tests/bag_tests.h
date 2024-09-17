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
	using BagTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(BagTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the count method correctly returns the number of occurences
	/// of elements.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(BagTests, CountReturnsNumberOfOccurences) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, b, b, c, c, e, f, f, f, f, f };

		if constexpr (map<collection_type>) {
			EXPECT_EQ(obj.count(a.key()), 1);
			EXPECT_EQ(obj.count(b.key()), 3);
			EXPECT_EQ(obj.count(c.key()), 2);
			EXPECT_EQ(obj.count(d.key()), 0);
			EXPECT_EQ(obj.count(e.key()), 1);
			EXPECT_EQ(obj.count(f.key()), 5);
		}
		else {
			EXPECT_EQ(obj.count(a), 1);
			EXPECT_EQ(obj.count(b), 3);
			EXPECT_EQ(obj.count(c), 2);
			EXPECT_EQ(obj.count(d), 0);
			EXPECT_EQ(obj.count(e), 1);
			EXPECT_EQ(obj.count(f), 5);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(
		BagTests,
		CountReturnsNumberOfOccurences
	);
}