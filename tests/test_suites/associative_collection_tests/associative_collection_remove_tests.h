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
	class AssociativeCollectionRemoveTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeCollectionRemoveTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert on empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionRemoveTests, 
		RemoveAtIteratorDeletesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c};
		ASSERT_FALSE(obj.isEmpty());
		auto expected = { *obj.begin(), *std::next(obj.begin(), 2) };

		obj.remove(std::next(obj.begin()));

		this->testObjectContainsAllExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionRemoveTests,
		RemoveAtIteratorReturnsIteratorToNextPosition
	) {
		auto method = [](auto& obj) {
			return obj.remove(std::next(obj.begin()));
		};
		auto expected = [](auto& obj) { return *std::next(obj.begin(), 2); };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator range remove method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionRemoveTests,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{a, b, c, d, e, f};
		ASSERT_FALSE(obj.isEmpty());

		auto begin = std::next(obj.begin());
		auto end = std::next(obj.begin(), 4);

		auto expected = { 
			*obj.begin(), 
			*std::next(obj.begin(), 4),
			*std::next(obj.begin(), 5)
		};

		obj.remove(begin, end);

		this->testObjectContainsAllExpectedElements(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionRemoveTests,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	) {
		using iterator = TypeParam::collection_t::iterator;

		auto method = [](auto& obj) -> iterator {
			auto begin = std::next(obj.begin());
			auto end = std::next(obj.begin(), 3);
			return obj.remove(begin, end);
		};
		auto expected = [](auto& obj) { return *std::next(obj.begin(), 3); };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeCollectionRemoveTests,
		RemoveAtIteratorDeletesElementAtCorrectPosition,
		RemoveAtIteratorReturnsIteratorToNextPosition,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	);
}