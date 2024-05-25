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

#include "../sequential_collection_test_fixture.h"

namespace collection_tests {

	template <class params> 
	class SequentialIteratorEmplaceTests : 
		public SequentialCollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIteratorEmplaceTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method correctly places elements in the 
	/// first position when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorEmplaceTests,
		EmplaceAtBeginIteratorPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.emplace(obj.begin(), value);
		};
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method correctly places elements in the 
	/// last position when called with the end iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorEmplaceTests,
		EmplaceAtEndIteratorPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, const auto& value) {
			obj.emplace(obj.end(), value);
		};
		this->testMethodPlacesElementLast(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method inserts elements at the requested
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorEmplaceTests,
		EmplaceAtIteratorPlacesElementAtCorrectPosition
	) {
		auto method = [](auto& obj, auto& value) {
			auto position = std::next(obj.begin());
			obj.emplace(position, value);
		};
		this->testMethodPlacesElementInTheMiddle(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator emplace method returns an iterator to the newly
	/// inserted element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorEmplaceTests, 
		EmplaceAtIteratorReturnsIteratorToPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto method = [&](auto& obj) {
			return obj.emplace(std::next(obj.begin()), a);
		};
		auto expected = [&](auto& obj) { return a; };

		this->testMethodReturnsIteratorToExpectedElement(method, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIteratorEmplaceTests,
		EmplaceAtBeginIteratorPlacesElementFirstInTheSequence,
		EmplaceAtEndIteratorPlacesElementLastInTheSequence,
		EmplaceAtIteratorPlacesElementAtCorrectPosition,
		EmplaceAtIteratorReturnsIteratorToPosition
	);
}