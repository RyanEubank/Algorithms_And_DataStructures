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

#include "sequential_collection_tests.h"

namespace collection_tests {

	template <class params>
	class SequentialCollectionEmplaceTests : 
		public SequentialCollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialCollectionEmplaceTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceFront method inserts the given element at the
	/// first position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests, 
		EmplaceFrontPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.emplaceFront(value); };
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceBack method inserts the given element at the 
	/// last position.
	/// </summary> -----------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests, 
		EmplaceBackPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.emplaceBack(value); };
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method correctly places elements when
	/// called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests,
		EmplaceAtIndexZeroPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
			auto index = Index{ 0 };
			obj.emplace(index, value);
			};
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method correctly places elements when
	/// called with an index of [size()].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests,
		EmplaceAtLastIndexPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
			auto index = Index{ obj.size() };
			obj.emplace(index, value);
			};
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method inserts elements at the 
	/// requested position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests,
		EmplaceAtIndexPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, auto& value) {
			auto index = Index(1);
			obj.emplace(index, value);
		};
		this->testMethodPlacesElementInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method checks the bounds of the given 
	/// index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests, 
		EmplaceAtIndexChecksBounds
	) {
		using element = typename TypeParam::element_t;
		auto value = element{};

		auto method = [&](auto& obj, Index& index) { obj.emplace(index, value); };
		auto safeIndex = [](auto& obj) { return obj.size(); };
		auto unsafeIndex = [](auto& obj) { return obj.size() + 1; };

		this->testMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method correctly places elements at 
	/// the first positon when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests,
		EmplaceAtBeginIteratorPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
			obj.emplace(obj.begin(), value);
		};
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method correctly places elements at 
	/// the last position when called with the end iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests,
		EmplaceAtEndIteratorPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) {
			obj.emplace(obj.end(), value);
		};
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method inserts elements at the 
	/// requested position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests,
		EmplaceAtIteratorPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, auto& value) {
			auto iterator = std::next(obj.begin());
			obj.emplace(iterator, value);
		};
		this->testMethodPlacesElementInTheMiddle(method);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialCollectionEmplaceTests,
		EmplaceFrontPlacesElementFirstInTheSequence,
		EmplaceBackPlacesElementLastInTheSequence,
		EmplaceAtIndexZeroPlacesElementFirstInTheSequence,
		EmplaceAtLastIndexPlacesElementLastInTheSequence,
		EmplaceAtIndexPlacesElementAtCorrectPosition,
		EmplaceAtIndexChecksBounds,
		EmplaceAtBeginIteratorPlacesElementFirstInTheSequence,
		EmplaceAtEndIteratorPlacesElementLastInTheSequence,
		EmplaceAtIteratorPlacesElementAtCorrectPosition
	);
}