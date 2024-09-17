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
	using SetInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SetInsertionTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method has no effect when inserting a duplicate
	/// into a set.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SetInsertionTests, InsertFailsOnDuplicateElement) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		obj.insert(a);
		auto expected = { a, b, c };

		EXPECT_EQ(obj.size(), 3);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method returns an iterator to an equivalent 
	/// existing element when inserting a duplicate into a set.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SetInsertionTests, InsertReturnsIteratorToDuplicateOnFailure) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		auto result = obj.insert(a);

		EXPECT_EQ(obj.size(), 3);
		EXPECT_EQ(*result, a);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method has no effect when inserting a range 
	/// consisting of only duplicates into a set.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SetInsertionTests, InsertFailsOnDuplicateRange) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto expected = { a, b, c };
		obj.insert(expected.begin(), expected.end());

		EXPECT_EQ(obj.size(), 3);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method returns an iterator to the last failed
	/// insertion on duplicate insert.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SetInsertionTests, InsertRangeReturnsIteratorToLastDuplicateElement) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		auto input = { a, b, c };
		auto result = obj.insert(input.begin(), input.end());

		EXPECT_EQ(obj.size(), 3);
		EXPECT_EQ(*result, c);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will perform a union of two sets when
	/// inserting a range with some overlap with the original collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SetInsertionTests, InsertRangeConstructsUnionOfBothSets) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};

		auto input = { a, d, b };
		auto result = obj.insert(input.begin(), input.end());
		auto expected = { a, b, c, d };

		EXPECT_EQ(obj.size(), 4);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SetInsertionTests,
		InsertFailsOnDuplicateElement,
		InsertReturnsIteratorToDuplicateOnFailure,
		InsertFailsOnDuplicateRange,
		InsertRangeReturnsIteratorToLastDuplicateElement,
		InsertRangeConstructsUnionOfBothSets
	);
}