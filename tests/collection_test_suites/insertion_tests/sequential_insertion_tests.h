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

#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using SequentialInsertionTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SequentialInsertionTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertFront method inserts the given element at the
	/// first position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		InsertFrontPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.insertFront(value); };
		this->expectMethodInsertsAtBeginningOfSequence(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertBack method inserts the given element at the last 
	/// position.
	/// </summary> -----------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		InsertBackPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.insertBack(value); };
		this->expectMethodInsertsAtEndOfSequence(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceFront method inserts the given element at the
	/// first position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		EmplaceFrontPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.emplaceFront(value); };
		this->expectMethodInsertsAtBeginningOfSequence(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceBack method inserts the given element at the 
	/// last position.
	/// </summary> -----------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		EmplaceBackPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.emplaceBack(value); };
		this->expectMethodInsertsAtEndOfSequence(method);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialInsertionTests,
		InsertFrontPlacesElementFirstInTheSequence,
		InsertBackPlacesElementLastInTheSequence,
		EmplaceFrontPlacesElementFirstInTheSequence,
		EmplaceBackPlacesElementLastInTheSequence
	);
}