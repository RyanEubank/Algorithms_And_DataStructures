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

#include "../associative_collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class AssociativeIteratorInsertTests : 
		public AssociativeCollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeIteratorInsertTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert values on 
	/// empty collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorInsertTests, 
		InsertWithLocationHintPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};
		ASSERT_TRUE(obj.isEmpty());
		obj.insert(obj.begin(), a);
		auto expected = { a };

		this->testObjectContainsExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert on non-empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorInsertTests, 
		InsertWithLocationHintPlacesElementInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_FALSE(obj.isEmpty());
		obj.insert(obj.begin(), d);
		auto expected = { a, b, c, d };

		this->testObjectContainsExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will return an iterator to the
	/// newly inserted element when successful.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorInsertTests, 
		InsertWithLocationHintReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) { return obj.insert(obj.begin(), a); };
		auto expected = [&](auto& obj) { return a; };
		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert a given range 
	/// of elements into an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorInsertTests, 
		InsertWithLocationHintPlacesRangeInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};

		ASSERT_TRUE(obj.isEmpty());

		auto input = { d, e, f };
		obj.insert(obj.begin(), input.begin(), input.end());

		this->testObjectContainsExpectedElements(obj, input);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert a given range 
	/// of elements into a non-empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorInsertTests, 
		InsertWithLocationHintPlacesRangeInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_FALSE(obj.isEmpty());

		auto input = { d, e, f };
		obj.insert(obj.begin(), input.begin(), input.end());
		auto expected = { a, b, c, d, e, f };

		this->testObjectContainsExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert range method will return an iterator to 
	/// the last element in the range when successful.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorInsertTests, 
		InsertWithLocationHintReturnsIteratorToInsertedRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto input = { a, b, c };
		auto method = [&](auto& obj) { 
			return obj.insert(obj.begin(), input.begin(), input.end()); 
		};
		auto expected = [&](auto& obj) { return c; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeIteratorInsertTests,
		InsertWithLocationHintPlacesElementInEmptyCollection,
		InsertWithLocationHintPlacesElementInNonEmptyCollection,
		InsertWithLocationHintReturnsIteratorToInsertedElement,
		InsertWithLocationHintPlacesRangeInEmptyCollection,
		InsertWithLocationHintPlacesRangeInNonEmptyCollection,
		InsertWithLocationHintReturnsIteratorToInsertedRange
	);
}