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
	using SequentialPositionedInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SequentialPositionedInsertionTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional insert method will correctly insert values in 
	/// empty sequential collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		InsertAtLocationPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.insert(obj.begin(), a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional insert method correctly places elements in the 
	/// first position when called with the begin iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		InsertAtBeginIteratorPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.insert(obj.begin(), value);
		};
		this->expectMethodInsertsAtBeginningOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional insert method correctly places elements in the 
	/// last position when called with the end iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		InsertAtEndIteratorPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.insert(obj.end(), value);
		};
		this->expectMethodInsertsAtEndOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional insert method inserts elements at the 
	/// requested position.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		InsertAtIteratorPlacesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, const auto& value) {
			auto location = std::next(obj.begin(), position);
			obj.insert(location, value);
		};

		this->expectMethodInsertsAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional insert method returns an iterator to the newly
	/// inserted element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		InsertAtIteratorReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		auto result = obj.insert(std::next(obj.begin()), a);
		auto expected = a;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional insert method will correctly insert a given 
	/// range of elements into an empty sequential collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		InsertAtIteratorPlacesRangeInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		auto expected = { a, b, c };
		obj.insert(obj.begin(), expected.begin(), expected.end());

		EXPECT_EQ(obj.size(), 3);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// beginning of the sequence when called with the begin iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		InsertRangeAtBeginIteratorPlacesElementsFirstInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto pos = obj.begin();
			obj.insert(pos, begin, end);
		};
		this->expectMethodInsertsRangeAtBeginningOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// end of the sequence when called with the end iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		InsertRangeAtEndIteratorPlacesElementsLastInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto pos = obj.end();
			obj.insert(pos, begin, end);
		};
		this->expectMethodInsertsRangeAtEndOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// position of the given iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		InsertRangeAtIteratorPlacesElementsAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, auto begin, auto end) {
			auto location = std::next(obj.begin(), position);
			obj.insert(location, begin, end);
		};

		this->expectMethodInsertsRangeAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method returns an iterator to the fist
	/// element of the given range.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		InsertRangeAtIteratorReturnsIteratorToFirstElementOfRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto input = { d, e, f };

		auto result = obj.insert(obj.begin(), input.begin(), input.end()); 
		auto expected = d;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional emplace method will correctly insert values on 
	/// empty sequential collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		EmplaceAtLocationPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.emplace(obj.begin(), a);
		auto expected = { a };

		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional emplace method correctly places elements in 
	/// the first position in the sequence when called with the begin iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		EmplaceAtBeginIteratorPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.emplace(obj.begin(), value);
		};
		this->expectMethodInsertsAtBeginningOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional emplace method correctly places elements in 
	/// the last position in the sequence when called with the end iterator.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		EmplaceAtEndIteratorPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.emplace(obj.end(), value);
		};
		this->expectMethodInsertsAtEndOfSequence(method);
	}


	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional emplace method inserts elements at the 
	/// requested position.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests,
		EmplaceAtIteratorPlacesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, const auto& value) {
			auto location = std::next(obj.begin(), position);
			obj.emplace(location, value);
		};

		this->expectMethodInsertsAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the positional emplace method returns an iterator to the 
	/// newly inserted element.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialPositionedInsertionTests, 
		EmplaceAtIteratorReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		auto result = obj.insert(std::next(obj.begin(), 1), a);
		auto expected = a;

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialPositionedInsertionTests,
		InsertAtLocationPlacesElementInEmptyCollection,
		InsertAtBeginIteratorPlacesElementFirstInTheSequence,
		InsertAtEndIteratorPlacesElementLastInTheSequence,
		InsertAtIteratorPlacesElementAtCorrectPosition,
		InsertAtIteratorReturnsIteratorToPosition,
		InsertAtIteratorPlacesRangeInEmptyCollection,
		InsertRangeAtBeginIteratorPlacesElementsFirstInTheSequence,
		InsertRangeAtEndIteratorPlacesElementsLastInTheSequence,
		InsertRangeAtIteratorPlacesElementsAtCorrectPosition,
		InsertRangeAtIteratorReturnsIteratorToFirstElementOfRange,
		EmplaceAtLocationPlacesElementInEmptyCollection,
		EmplaceAtBeginIteratorPlacesElementFirstInTheSequence,
		EmplaceAtEndIteratorPlacesElementLastInTheSequence,
		EmplaceAtIteratorPlacesElementAtCorrectPosition,
		EmplaceAtIteratorReturnsIteratorToPosition
	);
}