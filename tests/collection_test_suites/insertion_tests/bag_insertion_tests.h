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
	using BagInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(BagInsertionTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that inserting a dupliucate element inserts like normal without
	/// failing.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(BagInsertionTests, InsertDuplicatePlacesElementInCollection) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		count_list counts = { {a, 2}, {b, 1}, {c, 1} };

		obj.insert(a);

		EXPECT_EQ(obj.size(), 4);
		this->expectCount(obj, counts);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method returns an iterator to the inserted 
	/// element when inserting a duplicate.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(BagInsertionTests, InsertReturnsIteratorToInsertedDuplicate) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		auto result = obj.insert(a);

		EXPECT_EQ(obj.size(), 4);
		EXPECT_EQ(*result, a);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method has no effect when inserting a range 
	/// consisting of only duplicates into a set.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BagInsertionTests, 
		InsertDuplicateRangeCorrectlyPlacesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto input = { a, b, c };
		count_list counts = { {a, 2}, {b, 2}, {c, 2} };

		obj.insert(input.begin(), input.end());
		
		EXPECT_EQ(obj.size(), 6);
		this->expectCount(obj, counts);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert range method returns an iterator to the last
	/// inserted duplicate.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BagInsertionTests, 
		InsertRangeReturnsIteratorToLastDuplicateElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		auto input = { a, b, c };
		auto result = obj.insert(input.begin(), input.end());

		EXPECT_EQ(*result, c);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert range method will correctly insert a range that
	/// contains both duplicate and unique elements into the collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BagInsertionTests, 
		InsertRangePlacesDuplicatesAndUniqueElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b };

		auto input = { a, b, d, e };
		count_list counts = { {a, 2}, {b, 2}, {c, 0}, { d, 1 }, {e, 1} };

		obj.insert(input.begin(), input.end());

		EXPECT_EQ(obj.size(), 6);
		this->expectCount(obj, counts);
	}


	REGISTER_TYPED_TEST_SUITE_P(
		BagInsertionTests,
		InsertDuplicatePlacesElementInCollection,
		InsertReturnsIteratorToInsertedDuplicate,
		InsertDuplicateRangeCorrectlyPlacesElements,
		InsertRangeReturnsIteratorToLastDuplicateElement,
		InsertRangePlacesDuplicatesAndUniqueElements
	);
}