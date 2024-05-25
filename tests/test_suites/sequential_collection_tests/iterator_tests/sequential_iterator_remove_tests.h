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

#include "../sequential_collection_test_fixture.h"

namespace collection_tests {

	template <class params> 
	class SequentialIteratorRemoveTests : 
		public SequentialCollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIteratorRemoveTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method correctly deletes the first 
	/// element when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorRemoveTests,
		RemoveAtBeginIteratorDeletesFirstElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.remove(obj.begin()); };
		this->testMethodRemovesFirstElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method removes elements at the requested
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorRemoveTests,
		RemoveAtIteratorDeletesElementAtCorrectPosition
	) {
		auto method = [](auto& obj) { obj.remove(std::next(obj.begin())); };
		this->testMethodRemovesMiddleElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorRemoveTests,
		RemoveAtIteratorReturnsIteratorToNextPosition
	) {
		auto method = [](auto& obj) {
			return obj.remove(std::next(obj.begin()));
		};
		auto expected = [&](auto& obj) { return *std::next(obj.begin(), 2); };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorRemoveTests,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c, d, e, f };
		auto expected = { a, d, e, f };
		auto begin = std::next(obj.begin());
		auto end = std::next(begin, 2);
		obj.remove(begin, end);

		this->testObjectEqualsExpectedSequence(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorRemoveTests,
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
		SequentialIteratorRemoveTests,
		RemoveAtBeginIteratorDeletesFirstElementInTheSequence,
		RemoveAtIteratorDeletesElementAtCorrectPosition,
		RemoveAtIteratorReturnsIteratorToNextPosition,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	);
}