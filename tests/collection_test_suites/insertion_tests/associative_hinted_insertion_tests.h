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
	using AssociativeHintedInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(AssociativeHintedInsertionTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert values in 
	/// empty associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		InsertWithLocationHintPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.insert(obj.begin(), a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert in non-empty 
	/// associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		InsertWithLocationHintPlacesElementInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		obj.insert(obj.begin(), d);
		auto expected = { a, b, c, d };

		EXPECT_EQ(obj.size(), 4);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method for associative collections will
	/// return an iterator to the newly inserted element when successful.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		InsertWithLocationHintReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, f };

		auto result = obj.insert(obj.begin(), c);
		auto expected = c;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert a given range 
	/// of elements into an empty associative collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		InsertWithLocationHintPlacesRangeInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		auto expected = { a, b, c };
		obj.insert(obj.begin(), expected.begin(), expected.end());

		EXPECT_EQ(obj.size(), 3);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert a given range 
	/// of elements into a non-empty associative collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		InsertWithLocationHintPlacesRangeInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

		auto input = { d, e, f };
		obj.insert(obj.begin(), input.begin(), input.end());
		auto expected = { a, b, c, d, e, f };

		EXPECT_EQ(obj.size(), 6);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert range method for associative collections 
	/// will return an iterator to the last element in the range when 
	/// successful.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		InsertWithLocationHintReturnsIteratorToLastInsertedElementOfRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto input = { d, e, f };

		auto result = obj.insert(obj.begin(), input.begin(), input.end()); 
		auto expected = f;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will correctly insert values in 
	/// empty associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		EmplaceWithLocationHintPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.emplace(obj.begin(), a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will correctly insert in non-empty 
	/// associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		EmplaceWithLocationHintPlacesElementInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		obj.emplace(obj.begin(), d);
		auto expected = { a, b, c, d };

		EXPECT_EQ(obj.size(), 4);
		this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method for associative collections will 
	/// return an iterator to the newly inserted element when successful.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeHintedInsertionTests, 
		EmplaceWithLocationHintReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

		auto result = obj.emplace(obj.begin(), d);
		auto expected = d;

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeHintedInsertionTests,
		InsertWithLocationHintPlacesElementInEmptyCollection,
		InsertWithLocationHintPlacesElementInNonEmptyCollection,
		InsertWithLocationHintReturnsIteratorToInsertedElement,
		InsertWithLocationHintPlacesRangeInEmptyCollection,
		InsertWithLocationHintPlacesRangeInNonEmptyCollection,
		InsertWithLocationHintReturnsIteratorToLastInsertedElementOfRange,
		EmplaceWithLocationHintPlacesElementInEmptyCollection,
		EmplaceWithLocationHintPlacesElementInNonEmptyCollection,
		EmplaceWithLocationHintReturnsIteratorToInsertedElement
	);
}