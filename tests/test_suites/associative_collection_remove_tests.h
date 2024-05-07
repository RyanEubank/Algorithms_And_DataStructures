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

#include "collection_tests.h"

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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);

		auto iterator = std::next(obj.begin());
		auto removedElement = *iterator;

		obj.remove(iterator);
		EXPECT_EQ(collections::find(obj, removedElement), obj.end());
		EXPECT_EQ(obj.size(), input.size() - 1);
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
		using iterator = typename TypeParam::collection_t::iterator;

		auto value = this->testInput.control()[2];
		auto method = [](auto& obj) -> iterator {
			auto iterator = std::next(obj.begin());
			return obj.remove(iterator);
		};

		this->testMethodReturnsIteratorToExpectedElement(method, value);
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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);
		EXPECT_EQ(obj.size(), input.size());

		auto begin = obj.begin();
		auto end = std::next(begin, 2);

		obj.remove(begin, end);

		EXPECT_EQ(obj.size(), input.size() - 2);
		EXPECT_EQ(collections::find(obj, input[0]), obj.end());
		EXPECT_EQ(collections::find(obj, input[1]), obj.end());
		EXPECT_NE(collections::find(obj, input[2]), obj.end());
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
		using iterator = typename TypeParam::collection_t::iterator;
		auto method = [](auto& obj) -> iterator {
			auto begin = obj.begin();
			auto end = std::prev(obj.end());
			return obj.remove(begin, end);
		};
		auto expected = this->testInput.control()[2];
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