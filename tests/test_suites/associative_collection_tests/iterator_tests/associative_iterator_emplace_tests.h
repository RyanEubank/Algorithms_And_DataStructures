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

#include "../../collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class AssociativeIteratorEmplaceTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeIteratorEmplaceTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will correctly insert values on 
	/// empty collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorEmplaceTests, 
		EmplaceWithLocationHintPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};
		ASSERT_TRUE(obj.isEmpty());
		obj.emplace(obj.begin(), a);
		auto expected = { a };

		this->testObjectContainsAllExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will correctly insert on non-empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorEmplaceTests, 
		EmplaceWithLocationHintPlacesElementInNonEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_FALSE(obj.isEmpty());
		obj.emplace(obj.begin(), d);
		auto expected = { a, b, c, d };

		this->testObjectContainsAllExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will return an iterator to the
	/// newly inserted element when successful.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorEmplaceTests, 
		EmplaceWithLocationHintReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) { return obj.emplace(obj.begin(), a); };
		auto expected = [&](auto& obj) { return a; };
		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeIteratorEmplaceTests,
		EmplaceWithLocationHintPlacesElementInEmptyCollection,
		EmplaceWithLocationHintPlacesElementInNonEmptyCollection,
		EmplaceWithLocationHintReturnsIteratorToInsertedElement
	);
}