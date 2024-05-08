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

#include "sequential_collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class SequentialCollectionRemoveTests :
		public SequentialCollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialCollectionRemoveTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeFront method removes the first element in the 
	/// collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests, 
		RemoveFrontDeletesFirstElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.removeFront(); };
		this->testMethodRemovesFirstElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeBack method removes the last element in the 
	/// collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests, 
		RemoveBackDeletesLastElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.removeBack(); };
		this->testMethodRemovesLastElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the first
	/// element when called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAtIndexZeroDeletesFirstElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.remove(Index(0)); };
		this->testMethodRemovesFirstElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the last element
	/// when called with an index of [size() - 1].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAtLastIndexDeletesLastElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.remove(Index(obj.size() - 1)); };
		this->testMethodRemovesLastElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method deletes elements at the requested 
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAtIndexDeletesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		auto index = Index(1);
		collection obj(collections::from_range, input);
		auto removed_element = obj[index.get()];

		obj.remove(index);

		EXPECT_EQ(collections::find(obj, removed_element), obj.end());
		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(obj[1], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeAt method checks index bounds throwing an 
	/// exception if an out of bounds removal is attempted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAtIndexChecksBounds
	) {
		auto method = [](auto& obj, Index& index) { obj.remove(index); };
		auto safeIndex = [](auto& obj) { return obj.size() - 1; };
		auto unsafeIndex = [](auto& obj) { return obj.size(); };

		this->testMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAtIndexReturnsIteratorToNextPosition
	) {
		auto method = [](auto& obj) {
			return obj.remove(Index(1));
		};
		auto expected = this->_test_data.control()[2];

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method correctly deletes the first 
	/// element when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
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
		SequentialCollectionRemoveTests,
		RemoveAtIteratorDeletesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		collection obj(collections::from_range, input);
		auto iterator = std::next(obj.begin());
		auto removedElement = *iterator;

		obj.remove(iterator);

		EXPECT_EQ(collections::find(obj, removedElement), obj.end());
		EXPECT_EQ(obj.size(), input.size() - 1);

		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(obj[1], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAtIteratorReturnsIteratorToNextPosition
	) {
		using iterator = typename TypeParam::collection_t::iterator;

		auto value = this->_test_data.control()[2];
		auto method = [](auto& obj) -> iterator {
			auto iterator = std::next(obj.begin());
			return obj.remove(iterator);
		};
		
		this->testMethodReturnsIteratorToExpectedElement(method, value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAllBetweenIndexCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		collection obj(collections::from_range, input);

		EXPECT_EQ(obj.size(), input.size());

		obj.remove({ .begin = 0, .end = 2 });

		EXPECT_EQ(obj.size(), input.size() - 2);
		EXPECT_EQ(obj[0], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method throws an exception when the
	/// range is out of bounds or the begin index is > the end index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests, 
		RemoveAllBetweenIndexChecksBounds
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		collection obj(collections::from_range, input);

		ASSERT_GT(obj.size(), 1);

		auto safeRange = IndexRange{ .begin = 0, .end = input.size() - 1 };
		auto unsafeBegin = IndexRange{ .begin = input.size(), .end = input.size() };
		auto unsafeEnd = IndexRange{ .begin = 0, .end = input.size() + 1 };
		auto unsafeRange = IndexRange{ .begin = input.size(), .end = input.size() + 1 };
		auto unsafeOutOfOrder = IndexRange{ .begin = input.size() - 1, .end = 0 };

		EXPECT_NO_THROW(obj.remove(safeRange));
		EXPECT_THROW(obj.remove(unsafeBegin), std::out_of_range);
		EXPECT_THROW(obj.remove(unsafeEnd), std::out_of_range);
		EXPECT_THROW(obj.remove(unsafeRange), std::out_of_range);
		EXPECT_THROW(obj.remove(unsafeOutOfOrder), std::exception);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAllBetweenIndexReturnsIteratorToNextPosition
	) {
		using iterator = typename TypeParam::collection_t::iterator;

		auto value = this->_test_data.control()[2];
		auto method = [](auto& obj) -> iterator {
			auto range = IndexRange{ .begin = 0, .end = 2 };
			return obj.remove(range);
		};
		
		this->testMethodReturnsIteratorToExpectedElement(method, value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		collection obj(collections::from_range, input);

		EXPECT_EQ(obj.size(), input.size());

		auto begin = obj.begin();
		auto end = std::next(begin, 2);
		obj.remove(begin, end);

		EXPECT_EQ(obj.size(), input.size() - 2);
		EXPECT_EQ(obj[0], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	) {
		using iterator = typename TypeParam::collection_t::iterator;

		auto method = [](auto& obj) -> iterator {
			auto begin = obj.begin();
			auto end = std::prev(obj.end());
			return obj.remove(begin, end);
		};
		auto expected = this->_test_data.control()[2];

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialCollectionRemoveTests,
		RemoveFrontDeletesFirstElementInTheSequence,
		RemoveBackDeletesLastElementInTheSequence,
		RemoveAtIndexZeroDeletesFirstElementInTheSequence,
		RemoveAtLastIndexDeletesLastElementInTheSequence,
		RemoveAtIndexDeletesElementAtCorrectPosition,
		RemoveAtIndexChecksBounds,
		RemoveAtIndexReturnsIteratorToNextPosition,
		RemoveAtBeginIteratorDeletesFirstElementInTheSequence,
		RemoveAtIteratorDeletesElementAtCorrectPosition,
		RemoveAtIteratorReturnsIteratorToNextPosition,
		RemoveAllBetweenIndexCorrectlyRemovesElements,
		RemoveAllBetweenIndexChecksBounds,
		RemoveAllBetweenIndexReturnsIteratorToNextPosition,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	);
}