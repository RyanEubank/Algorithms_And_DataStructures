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
	/// Tests that the insertFront method returns an iterator to the newly
	/// inserted element at the front of the collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		InsertFrontReturnsIteratorToFirstElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		auto result = obj.insertFront(d);
		auto expected = d;

		EXPECT_EQ(*result, expected);
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
	/// Tests that the insertBack method returns an iterator to the newly
	/// inserted element at the front of the collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		InsertBackReturnsIteratorToLastElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		auto result = obj.insertBack(d);
		auto expected = d;

		EXPECT_EQ(*result, expected);
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
	/// Tests that the emplaceFront method returns an iterator to the newly
	/// inserted element at the front of the collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		EmplaceFrontReturnsIteratorToFirstElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		auto result = obj.emplaceFront(d);
		auto expected = d;

		EXPECT_EQ(*result, expected);
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

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceBack method returns an iterator to the newly
	/// inserted element at the front of the collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialInsertionTests, 
		EmplaceBackReturnsIteratorToLastElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		auto result = obj.emplaceBack(d);
		auto expected = d;

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialInsertionTests,
		InsertFrontPlacesElementFirstInTheSequence,
		InsertFrontReturnsIteratorToFirstElement,
		InsertBackPlacesElementLastInTheSequence,
		InsertBackReturnsIteratorToLastElement,
		EmplaceFrontPlacesElementFirstInTheSequence,
		EmplaceFrontReturnsIteratorToFirstElement,
		EmplaceBackPlacesElementLastInTheSequence,
		EmplaceBackReturnsIteratorToLastElement
	);
}