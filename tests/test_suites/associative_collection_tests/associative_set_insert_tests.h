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

#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class AssociativeSetInsertTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeSetInsertTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that 
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSetInsertTests, 
		InsertFailsOnDuplicateElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_EQ(obj.size(), 3);

		obj.insert(a);

		EXPECT_EQ(obj.size(), 3);
		auto expected = { a, b, c };
		this->testObjectContainsAllExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that 
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSetInsertTests, 
		InsertReturnsIteratorToDuplicateOnFailure
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_EQ(obj.size(), 3);

		auto result = obj.insert(a);

		EXPECT_EQ(obj.size(), 3);
		EXPECT_EQ(*result, a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that 
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSetInsertTests, 
		InsertFailsOnDuplicateRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };
		ASSERT_EQ(obj.size(), 3);

		auto input = { a, b, c };
		obj.insert(input.begin(), input.end());

		EXPECT_EQ(obj.size(), 3);
		auto expected = { a, b, c };
		this->testObjectContainsAllExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that 
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSetInsertTests, 
		InsertRangeReturnsIteratorToLastDuplicateElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_EQ(obj.size(), 3);

		auto input = { a, b, c };
		auto result = obj.insert(input.begin(), input.end());

		EXPECT_EQ(obj.size(), 3);
		EXPECT_EQ(*result, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that 
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSetInsertTests, 
		InsertRangeConstructsUnionOfBothSets
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_EQ(obj.size(), 3);

		auto input = { a, d, b };
		auto result = obj.insert(input.begin(), input.end());

		auto expected = { a, b, c, d };
		this->testObjectContainsAllExpectedElements(obj, expected);
	}



	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeSetInsertTests,
		InsertFailsOnDuplicateElement,
		InsertReturnsIteratorToDuplicateOnFailure,
		InsertFailsOnDuplicateRange,
		InsertRangeReturnsIteratorToLastDuplicateElement,
		InsertRangeConstructsUnionOfBothSets
	);
}