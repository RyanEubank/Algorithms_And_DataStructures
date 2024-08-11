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
		requires indexable_collection<typename params::collection_t, size_t>
	class SequentialIndexRemoveTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIndexRemoveTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the first
	/// element when called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemoveTests,
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
		SequentialIndexRemoveTests,
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
		SequentialIndexRemoveTests,
		RemoveAtIndexDeletesElementAtCorrectPosition
	) {
		auto method = [](auto& obj) { obj.remove(Index(1)); };
		this->testMethodRemovesMiddleElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeAt method checks index bounds throwing an 
	/// exception if an out of bounds removal is attempted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIndexRemoveTests, RemoveAtIndexChecksBounds) {
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
		SequentialIndexRemoveTests,
		RemoveAtIndexReturnsIteratorToNextPosition
	) {
		auto method = [](auto& obj) { return obj.remove(Index(1)); };
		auto expected = [](auto& obj) { return obj[2]; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemoveTests,
		RemoveAllBetweenIndexCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c, d, e, f };
		auto expected = { a, d, e, f };
		obj.remove({ .begin = 1, .end = 3 });

		this->testCollectionEqualsExpectedSequence(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method throws an exception when the
	/// range is out of bounds or the begin index is > the end index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemoveTests,
		RemoveAllBetweenIndexChecksBounds
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_GT(obj.size(), 1);

		auto safeRange = IndexRange{ .begin = 0, .end = 2 };
		auto unsafeBegin = IndexRange{ .begin = 3, .end = 3 };
		auto unsafeEnd = IndexRange{ .begin = 0, .end = 4 };
		auto unsafeRange = IndexRange{ .begin = 3, .end = 4 };
		auto unsafeOutOfOrder = IndexRange{ .begin = 2, .end = 0 };

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
		SequentialIndexRemoveTests,
		RemoveAllBetweenIndexReturnsIteratorToNextPosition
	) {
		auto method = [](auto& obj) {
			return obj.remove(IndexRange{ .begin = 0, .end = 2 });
		};
		auto expected = [](auto& obj) { return obj[2]; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIndexRemoveTests,
		RemoveAtIndexZeroDeletesFirstElementInTheSequence,
		RemoveAtLastIndexDeletesLastElementInTheSequence,
		RemoveAtIndexDeletesElementAtCorrectPosition,
		RemoveAtIndexReturnsIteratorToNextPosition,
		RemoveAtIndexChecksBounds,
		RemoveAllBetweenIndexCorrectlyRemovesElements,
		RemoveAllBetweenIndexChecksBounds,
		RemoveAllBetweenIndexReturnsIteratorToNextPosition
	);
}