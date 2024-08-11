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
	class SequentialIteratorInsertTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIteratorInsertTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method correctly places elements in the 
	/// first position when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests,
		InsertAtBeginIteratorPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.insert(obj.begin(), value);
		};
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method correctly places elements in the 
	/// last position when called with the end iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests,
		InsertAtEndIteratorPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.insert(obj.end(), value);
		};
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts elements at the requested
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests,
		InsertAtIteratorPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, auto& value) {
			auto position = std::next(obj.begin());
			obj.insert(position, value);
		};
		this->testMethodPlacesElementInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method returns an iterator to the newly
	/// inserted element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests, 
		InsertAtIteratorReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) {
			return obj.insert(std::next(obj.begin()), a);
		};
		auto expected = [&](auto& obj) { return a; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// beginning of the sequence when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests, 
		InsertRangeAtBeginIteratorPlacesElementsFirstInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto pos = obj.begin();
			obj.insert(pos, begin, end);
		};
		this->testMethodPlacesRangeAtBeginning(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// end of the sequence when called with the end iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests,
		InsertRangeAtEndIteraorPlacesElementsLastInTheSequence
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto pos = obj.end();
			obj.insert(pos, begin, end);
		};
		this->testMethodPlacesRangeAtEnd(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// position of the given iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests,
		InsertRangeAtIteratorPlacesElementsAtCorrectPosition
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto iterator = std::next(obj.begin());
			obj.insert(iterator, begin, end);
		};
		this->testMethodPlacesRangeInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method returns an iterator to the fist
	/// element of the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorInsertTests, 
		InsertRangeAtIteratorReturnsIteratorToFirstElementOfRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) -> iterator {
			auto input = { d, e, f };
			auto iterator = std::next(obj.begin());
			return obj.insert(iterator, input.begin(), input.end());
		};

		auto expected = [&](auto& obj) { return d; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIteratorInsertTests,
		InsertAtBeginIteratorPlacesElementFirstInTheSequence,
		InsertAtEndIteratorPlacesElementLastInTheSequence,
		InsertAtIteratorPlacesElementAtCorrectPosition,
		InsertAtIteratorReturnsIteratorToPosition,
		InsertRangeAtBeginIteratorPlacesElementsFirstInTheSequence,
		InsertRangeAtEndIteraorPlacesElementsLastInTheSequence,
		InsertRangeAtIteratorPlacesElementsAtCorrectPosition,
		InsertRangeAtIteratorReturnsIteratorToFirstElementOfRange
	);
}