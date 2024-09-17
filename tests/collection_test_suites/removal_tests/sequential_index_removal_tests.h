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

#include "util/types.h"
#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using SequentialIndexRemovalTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SequentialIndexRemovalTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the first
	/// element when called with an index of [0].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAtIndexZeroDeletesFirstElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.remove(Index(0)); };
		this->expectMethodRemovesFirstElementInSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the last element
	/// when called with an index of [size() - 1].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAtLastIndexDeletesLastElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.remove(Index(obj.size() - 1)); };
		this->expectMethodRemovesLastElementInSequence(method);
	}


	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method deletes elements at the requested 
	/// position.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAtIndexDeletesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto position = 1;

		auto method = [&](auto& obj) { 
			auto location = Index(position);
			obj.remove(location); 
		};

		this->expectMethodRemovesElementAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeAt method checks index bounds throwing an exception
	/// if an out of bounds removal is attempted.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SequentialIndexRemovalTests, RemoveAtIndexChecksBounds) {
		auto method = [](auto& obj, Index& index) { obj.remove(index); };
		auto safeIndex = [](auto& obj) { return obj.size() - 1; };
		auto unsafeIndex = [](auto& obj) { return obj.size(); };

		this->expectMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method returns an iterator to the element
	/// following the one that was deleted.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAtIndexReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

		auto expected = c;
		auto result = obj.remove(Index{ 1 });

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the remove method has no effect when called to remove an
	/// empty index range [size, size) from a non-empty collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests, 
		RemoveAllBetweenIndexHasNoEffectWhenCalledWithEmptyRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto method = [&]() { 
			obj.remove({ .begin = 1, .end = 1 });
		};

		EXPECT_NO_THROW(method());
		EXPECT_EQ(obj.size(), 3u);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAllBetweenIndexCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c, d, e, f };
		auto expected = { a, d, e, f };
		obj.remove({ .begin = 1, .end = 3 });

		EXPECT_EQ(obj.size(), 4);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method throws an exception when the
	/// range is out of bounds or the begin index is > the end index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAllBetweenIndexChecksBounds
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

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

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexRemovalTests,
		RemoveAllBetweenIndexReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c, d, e, f };

		auto begin = std::next(obj.begin());
		auto end = std::next(obj.begin(), 3);

		auto expected = d;
		auto result = obj.remove(IndexRange{ .begin = 1, .end = 3 });

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIndexRemovalTests,
		RemoveAtIndexZeroDeletesFirstElementInTheSequence,
		RemoveAtLastIndexDeletesLastElementInTheSequence,
		RemoveAtIndexDeletesElementAtCorrectPosition,
		RemoveAtIndexReturnsIteratorToNextPosition,
		RemoveAtIndexChecksBounds,
		RemoveAllBetweenIndexHasNoEffectWhenCalledWithEmptyRange,
		RemoveAllBetweenIndexCorrectlyRemovesElements,
		RemoveAllBetweenIndexChecksBounds,
		RemoveAllBetweenIndexReturnsIteratorToNextPosition
	);
}