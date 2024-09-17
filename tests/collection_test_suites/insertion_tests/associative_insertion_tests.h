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
	using AssociativeInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(AssociativeInsertionTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert on empty associative
	/// collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		InsertPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.insert(a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert on non-empty 
	/// associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		InsertPlacesElementInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		obj.insert(d);
		auto expected = { a, b, c, d };

		EXPECT_EQ(obj.size(), 4);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method for associative collections will return an 
	/// iterator to the newly inserted element when successful.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		InsertReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, f };

		auto result = obj.insert(c);
		auto expected = c;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert a given range of
	/// elements into an empty associative collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		InsertPlacesRangeInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		auto expected = { a, b, c };
		obj.insert(expected.begin(), expected.end());

		EXPECT_EQ(obj.size(), 3);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert a given range of
	/// elements into a non-empty associative collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		InsertPlacesRangeInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

		auto input = { d, e, f };
		obj.insert(input.begin(), input.end());
		auto expected = { a, b, c, d, e, f };

		EXPECT_EQ(obj.size(), 6);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert range method for associative collections will 
	/// return an iterator to the last element in the range when successful.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		InsertReturnsIteratorToInsertedRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto input = { d, e, f };

		auto result = obj.insert(input.begin(), input.end()); 
		auto expected = f;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplace method will correctly insert on empty associative
	/// collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		EmplacePlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.emplace(a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplace method will correctly insert on non-empty 
	/// associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		EmplacePlacesElementInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		obj.emplace(d);
		auto expected = { a, b, c, d };

		EXPECT_EQ(obj.size(), 4);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplace method will correctly insert on non-empty 
	/// associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeInsertionTests, 
		EmplaceReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, e, f, c };

		auto result = obj.emplace(b);
		auto expected = b;

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeInsertionTests,
		InsertPlacesElementInEmptyCollection,
		InsertPlacesElementInNonEmptyCollection,
		InsertReturnsIteratorToInsertedElement,
		InsertPlacesRangeInEmptyCollection,
		InsertPlacesRangeInNonEmptyCollection,
		InsertReturnsIteratorToInsertedRange,
		EmplacePlacesElementInEmptyCollection,
		EmplacePlacesElementInNonEmptyCollection,
		EmplaceReturnsIteratorToInsertedElement
	);
}