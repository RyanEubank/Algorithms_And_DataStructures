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
	class SequentialIndexInsertTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIndexInsertTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests, 
		InsertAtIndexZeroPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ 0 };
			obj.insert(index, value);
		};
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [size()].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests, 
		InsertAtLastIndexPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ obj.size() };
			obj.insert(index, value);
		};
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts elements at the requested 
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests,
		InsertAtIndexPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index(1);
			obj.insert(index, value);
		};
		this->testMethodPlacesElementInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method checks the bounds of the given 
	/// index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests, 
		InsertAtIndexChecksBounds
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj, Index& index) { obj.insert(index, a); };
		auto safeIndex = [](auto& obj) { return obj.size(); };
		auto unsafeIndex = [](auto& obj) { return obj.size() + 1; };

		this->testMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method returns an iterator to the newly
	/// inserted element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests, 
		InsertAtIndexReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) { return obj.insert(Index(1), a); };
		auto expected = [&](auto& obj) { return a; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// beginning of the sequence when called with and index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests,
		InsertRangeAtIndexZeroPlacesElementsFirstInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto index = Index{ 0 };
			obj.insert(index, begin, end);
		};
		this->testMethodPlacesRangeAtBeginning(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// end of the sequence when called an index of [size()].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests,
		InsertRangeAtLastIndexPlacesElementsLastInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto index = Index{ obj.size() };
			obj.insert(index, begin, end);
		};
		this->testMethodPlacesRangeAtEnd(method);
	}


	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// position of the given index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests,
		InsertRangeAtIndexPlacesElementsAtCorrectPosition
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto index = Index(1);
			obj.insert(index, begin, end);
		};
		this->testMethodPlacesRangeInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// position of the given index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexInsertTests,
		InsertRangeAtIndexReturnsIteratorToFirstElementOfRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) {
			auto input = { a, b, c };
			return obj.insert(Index(1), input.begin(), input.end());
		};
		auto expected = [&](auto& obj) { return a; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method throws an exception when the
	/// range is out of bounds or the begin index is > the end index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIndexInsertTests, InsertRangeAtIndexChecksBounds) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj1{ a, b, c };
		collection obj2{ a, b, c };
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

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIndexInsertTests,
		InsertAtIndexZeroPlacesElementFirstInTheSequence,
		InsertAtLastIndexPlacesElementLastInTheSequence,
		InsertAtIndexPlacesElementAtCorrectPosition,
		InsertAtIndexReturnsIteratorToPosition,
		InsertAtIndexChecksBounds,
		InsertRangeAtIndexZeroPlacesElementsFirstInTheSequence,
		InsertRangeAtLastIndexPlacesElementsLastInTheSequence,
		InsertRangeAtIndexPlacesElementsAtCorrectPosition,
		InsertRangeAtIndexReturnsIteratorToFirstElementOfRange,
		InsertRangeAtIndexChecksBounds
	);
}