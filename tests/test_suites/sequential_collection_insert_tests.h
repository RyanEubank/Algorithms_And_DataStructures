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
	class SequentialCollectionInsertTests : 
		public SequentialCollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialCollectionInsertTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertFront method inserts the given element at the
	/// first position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests, 
		InsertFrontPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.insertFront(value); };
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertBack method inserts the given element at the last 
	/// position.
	/// </summary> -----------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests, 
		InsertBackPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.insertBack(value); };
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests, 
		InsertAtIndexZeroPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
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
		SequentialCollectionInsertTests, 
		InsertAtLastIndexPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
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
		SequentialCollectionInsertTests,
		InsertAtIndexPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, auto& value) {
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
		SequentialCollectionInsertTests, 
		InsertAtIndexChecksBounds
	) {
		FORWARD_TEST_TYPES;

		auto value = element{};
		auto method = [&](auto& obj, Index& index) { obj.insert(index, value); };
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
		SequentialCollectionInsertTests, 
		InsertAtIndexReturnsIteratorToPosition
	) {
		auto value = this->_test_data.different_elements()[0];
		auto method = [=](auto& obj) {
			return obj.insert(Index(1), value);
		};

		this->testMethodReturnsIteratorToExpectedElement(method, value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method correctly places elements in the 
	/// first position when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests,
		InsertAtBeginIteratorPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
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
		SequentialCollectionInsertTests,
		InsertAtEndIteratorPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
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
		SequentialCollectionInsertTests,
		InsertAtIteratorPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, auto& value) {
			auto iterator = std::next(obj.begin());
			obj.insert(iterator, value);
		};
		this->testMethodPlacesElementInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method returns an iterator to the newly
	/// inserted element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests, 
		InsertAtIteratorReturnsIteratorToPosition
	) {
		using iterator = typename TypeParam::collection_t::iterator;

		auto value = this->_test_data.different_elements()[0];
		auto method = [=](auto& obj) -> iterator {
			auto pos = obj.begin();
			return obj.insert(++pos, value);
		};

		this->testMethodReturnsIteratorToExpectedElement(method, value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts the given range at the
	/// beginning of the sequence when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests, 
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
		SequentialCollectionInsertTests,
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
		SequentialCollectionInsertTests,
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
		SequentialCollectionInsertTests, 
		InsertRangeAtIteratorReturnsIteratorToFirstElementOfRange
	) {
		using iterator = typename TypeParam::collection_t::iterator;

		auto new_elements = this->_test_data.different_elements();
		auto method = [&](auto& obj) -> iterator {
			auto iterator = std::next(obj.begin());
			return obj.insert(iterator, new_elements.begin(), new_elements.end());
		};

		this->testMethodReturnsIteratorToExpectedElement(method, new_elements[0]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts the given range at the
	/// beginning of the sequence when called with and index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionInsertTests,
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
		SequentialCollectionInsertTests,
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
		SequentialCollectionInsertTests,
		InsertRangeAtIndexPlacesElementsAtCorrectPosition
	) {
		auto method = [](auto& obj, auto begin, auto end) {
			auto index = Index(1);
			obj.insert(index, begin, end);
		};
		this->testMethodPlacesRangeInTheMiddle(method);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialCollectionInsertTests,
		InsertFrontPlacesElementFirstInTheSequence,
		InsertBackPlacesElementLastInTheSequence,
		InsertAtIndexZeroPlacesElementFirstInTheSequence,
		InsertAtLastIndexPlacesElementLastInTheSequence,
		InsertAtIndexPlacesElementAtCorrectPosition,
		InsertAtIndexChecksBounds,
		InsertAtIndexReturnsIteratorToPosition,
		InsertAtBeginIteratorPlacesElementFirstInTheSequence,
		InsertAtEndIteratorPlacesElementLastInTheSequence,
		InsertAtIteratorPlacesElementAtCorrectPosition,
		InsertAtIteratorReturnsIteratorToPosition,
		InsertRangeAtBeginIteratorPlacesElementsFirstInTheSequence,
		InsertRangeAtEndIteraorPlacesElementsLastInTheSequence,
		InsertRangeAtIteratorPlacesElementsAtCorrectPosition,
		InsertRangeAtIteratorReturnsIteratorToFirstElementOfRange,
		InsertRangeAtIndexZeroPlacesElementsFirstInTheSequence,
		InsertRangeAtLastIndexPlacesElementsLastInTheSequence,
		InsertRangeAtIndexPlacesElementsAtCorrectPosition
	);
}