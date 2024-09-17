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
	using SequentialIndexInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SequentialIndexInsertionTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements into
	/// empty sequential colletions.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		InsertAtIndexPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.insert(Index(0), a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [0].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		InsertAtIndexZeroPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ 0 };
			obj.insert(index, value);
		};
		this->expectMethodInsertsAtBeginningOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [size()].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		InsertAtLastIndexPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ obj.size() };
			obj.insert(index, value);
		};
		this->expectMethodInsertsAtEndOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts elements at the requested 
	/// position.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		InsertAtIndexPlacesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, const auto& value) {
			obj.insert(Index(position), value);
		};

		this->expectMethodInsertsAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method checks the bounds of the given 
	/// index.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		InsertAtIndexChecksBounds
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj, Index& index) { obj.insert(index, a); };
		auto safeIndex = [](auto& obj) { return obj.size(); };
		auto unsafeIndex = [](auto& obj) { return obj.size() + 1; };

		this->expectMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method returns an iterator to the newly
	/// inserted element.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		InsertAtIndexReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, const auto& value) {
			auto location = Index(position);
			obj.insert(location, value);
		};

		this->expectMethodInsertsAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range into empty
	/// sequential collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		InsertRangeAtIndexPlacesElementsInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		auto expected = { a, b, c };
		obj.insert(Index{0}, expected.begin(), expected.end());


		EXPECT_EQ(obj.size(), 3);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}


	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// beginning of the sequence when called with and index of [0].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		InsertRangeAtIndexZeroPlacesElementsFirstInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto index = Index{ 0 };
			obj.insert(index, begin, end);
		};
		this->expectMethodInsertsRangeAtBeginningOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// end of the sequence when called an index of [size()].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		InsertRangeAtLastIndexPlacesElementsLastInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto index = Index{ obj.size() };
			obj.insert(index, begin, end);
		};
		this->expectMethodInsertsRangeAtEndOfSequence(method);
	}


	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// position of the given index.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		InsertRangeAtIndexPlacesElementsAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, auto begin, auto end) {
			auto location = Index(position);
			obj.insert(location, begin, end);
		};

		this->expectMethodInsertsRangeAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// position of the given index.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		InsertRangeAtIndexReturnsIteratorToFirstElementOfRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto input = { d, e, f };

		auto result = obj.insert(Index{1}, input.begin(), input.end());
		auto expected = d;

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method throws an exception when the
	/// range is out of bounds or the begin index is > the end index.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		InsertRangeAtIndexChecksBounds
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj1{ a, b, c };
		collection_type obj2{ a, b, c };
		auto input = { d, e, f };

		auto safeCall = [&]() { 
			obj1.insert(Index(3), input.begin(), input.end()); 
		};
		auto unsafefCall = [&]() { 
			obj2.insert(Index(4), input.begin(), input.end()); 
		};

		EXPECT_NO_THROW(safeCall());
		EXPECT_THROW(unsafefCall(), std::out_of_range);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method correctly places elements into
	/// empty sequential colletions.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		EmplaceAtIndexPlacesElementInEmptyCollection
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.emplace(Index(0), a);
		auto expected = { a };

		EXPECT_EQ(obj.size(), 1);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method correctly places elements when
	/// called with an index of [0].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		EmplaceAtIndexZeroPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ 0u };
			obj.emplace(index, value);
		};
		this->expectMethodInsertsAtBeginningOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method correctly places elements when
	/// called with an index of [size()].
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		EmplaceAtLastIndexPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ obj.size() };
			obj.emplace(index, value);
		};
		this->expectMethodInsertsAtEndOfSequence(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method inserts elements at the 
	/// requested position.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests,
		EmplaceAtIndexPlacesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, const auto& value) {
			obj.emplace(Index(position), value);
		};

		this->expectMethodInsertsAtGivenPositionInSequence(
			obj, method, position
		);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method checks the bounds of the given 
	/// index.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SequentialIndexInsertionTests, EmplaceAtIndexChecksBounds) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj, Index& index) { obj.emplace(index, a); };
		auto safeIndex = [](auto& obj) { return obj.size(); };
		auto unsafeIndex = [](auto& obj) { return obj.size() + 1; };

		this->expectMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method returns an iterator to the newly
	/// inserted element.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertionTests, 
		EmplaceAtIndexReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };
		auto position = 2;

		auto method = [&](auto& obj, const auto& value) {
			auto location = Index(position);
			obj.emplace(location, value);
		};

		this->expectMethodInsertsAtGivenPositionInSequence(
			obj, method, position
		);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIndexInsertionTests,
		InsertAtIndexPlacesElementInEmptyCollection,
		InsertAtIndexZeroPlacesElementFirstInTheSequence,
		InsertAtLastIndexPlacesElementLastInTheSequence,
		InsertAtIndexPlacesElementAtCorrectPosition,
		InsertAtIndexReturnsIteratorToPosition,
		InsertAtIndexChecksBounds,
		InsertRangeAtIndexPlacesElementsInEmptyCollection,
		InsertRangeAtIndexZeroPlacesElementsFirstInTheSequence,
		InsertRangeAtLastIndexPlacesElementsLastInTheSequence,
		InsertRangeAtIndexPlacesElementsAtCorrectPosition,
		InsertRangeAtIndexReturnsIteratorToFirstElementOfRange,
		InsertRangeAtIndexChecksBounds,
		EmplaceAtIndexPlacesElementInEmptyCollection,
		EmplaceAtIndexZeroPlacesElementFirstInTheSequence,
		EmplaceAtLastIndexPlacesElementLastInTheSequence,
		EmplaceAtIndexPlacesElementAtCorrectPosition,
		EmplaceAtIndexChecksBounds,
		EmplaceAtIndexReturnsIteratorToPosition
	);
}