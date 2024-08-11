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
	class SequentialIndexEmplaceTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIndexEmplaceTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method correctly places elements when
	/// called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexEmplaceTests,
		EmplaceAtIndexZeroPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			auto index = Index{ 0u };
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
		SequentialIndexEmplaceTests,
		EmplaceAtLastIndexPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
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
		SequentialIndexEmplaceTests,
		EmplaceAtIndexPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, const auto& value) {
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
	TYPED_TEST_P(SequentialIndexEmplaceTests, EmplaceAtIndexChecksBounds) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj, Index& index) { obj.emplace(index, a); };
		auto safeIndex = [](auto& obj) { return obj.size(); };
		auto unsafeIndex = [](auto& obj) { return obj.size() + 1; };

		this->testMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed emplace method returns an iterator to the newly
	/// inserted element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexEmplaceTests, 
		EmplaceAtIndexReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) { return obj.emplace(Index(1), a); };
		auto expected = [&](auto& obj) { return a; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIndexEmplaceTests,
		EmplaceAtIndexZeroPlacesElementFirstInTheSequence,
		EmplaceAtLastIndexPlacesElementLastInTheSequence,
		EmplaceAtIndexPlacesElementAtCorrectPosition,
		EmplaceAtIndexReturnsIteratorToPosition,
		EmplaceAtIndexChecksBounds
	);
}