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

#include <iterator>
#include <gtest/gtest.h>

#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T> 
	using SequentialPositionedRemovalTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SequentialPositionedRemovalTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method correctly deletes the first 
	/// element when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests,
		RemoveAtBeginIteratorDeletesFirstElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.remove(obj.begin()); };
		this->expectMethodRemovesFirstElementInSequence(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method removes elements at the requested
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests,
		RemoveAtIteratorDeletesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto position = 1;

		auto method = [&](auto& obj) { 
			auto location = std::next(obj.begin(), position);
			obj.remove(location); 
		};

		this->expectMethodRemovesElementAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests,
		RemoveAtIteratorReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };

		auto expected = c;
		auto result = obj.remove(std::next(obj.begin()));

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the remove method has no effect when called to remove a range
	/// of elements from an empty collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests, 
		RemoveAllBetweenIteratorsOnEmptyCollectionHasNoEffect
	) {
		auto method = [](auto& obj) { obj.remove(obj.begin(), obj.end()); };
		this->expectMethodHasNoEffectOnEmptyCollection(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the remove method has no effect when called to remove an
	/// empty range (begin == end) from a non-empty collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests, 
		RemoveAllBetweenIteratorsHasNoEffectWhenCalledWithEmptyRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto method = [&]() { obj.remove(obj.end(), obj.end()); };

		EXPECT_NO_THROW(method());
		EXPECT_EQ(obj.size(), 3u);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c, d, e, f };
		auto expected = { a, d, e, f };
		auto begin = std::next(obj.begin());
		auto end = std::next(begin, 2);
		obj.remove(begin, end);

		EXPECT_EQ(obj.size(), 4);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedRemovalTests,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c, d, e, f };

		auto begin = std::next(obj.begin());
		auto end = std::next(obj.begin(), 3);

		auto expected = d;
		auto result = obj.remove(begin, end);

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialPositionedRemovalTests,
		RemoveAtBeginIteratorDeletesFirstElementInTheSequence,
		RemoveAtIteratorDeletesElementAtCorrectPosition,
		RemoveAtIteratorReturnsIteratorToNextPosition,
		RemoveAllBetweenIteratorsOnEmptyCollectionHasNoEffect,
		RemoveAllBetweenIteratorsHasNoEffectWhenCalledWithEmptyRange,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	);
}