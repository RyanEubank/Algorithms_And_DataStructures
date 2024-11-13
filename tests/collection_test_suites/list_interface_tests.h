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
#include "collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using ListInterfaceTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(ListInterfaceTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that splicing nodes from one list to another removes and places
	/// the affected nodes correctly when spliced at the beginning of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ListInterfaceTests, 
		SpliceFromDifferentListCorrectlySplicesAtBeginningOfTargetList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list_1{a, b, c, d, e};
		collection_type list_2{f, g, h, i, j};

		auto pos = list_1.begin();
		auto begin = list_2.begin();
		auto end = std::next(list_2.begin(), 3);

		auto expected_1 = { f, g, h, a, b, c, d, e };
		auto expected_2 = { i, j };

		list_1.splice(pos, list_2, begin, end);

		this->expectSequence(list_1.begin(), list_1.end(), expected_1);
		this->expectSequence(list_2.begin(), list_2.end(), expected_2);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that splicing nodes from one list to another removes and places
	/// the affected nodes correctly when spliced into the middle of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ListInterfaceTests, 
		SpliceFromDifferentListCorrectlySplicesIntoMiddleOfTargetList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list_1{a, b, c, d, e};
		collection_type list_2{f, g, h, i, j};

		auto pos = std::next(list_1.begin());
		auto begin = std::next(list_2.begin());
		auto end = std::next(list_2.begin(), 4);

		auto expected_1 = { a, g, h, i, b, c, d, e };
		auto expected_2 = { f, j };

		list_1.splice(pos, list_2, begin, end);

		this->expectSequence(list_1.begin(), list_1.end(), expected_1);
		this->expectSequence(list_2.begin(), list_2.end(), expected_2);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that splicing nodes from one list to another removes and places
	/// the affected nodes correctly when spliced at the end of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ListInterfaceTests, 
		SpliceFromDifferentListCorrectlySplicesAtEndOfTargetList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list_1{a, b, c, d, e};
		collection_type list_2{f, g, h, i, j};

		auto pos = list_1.end();
		auto begin = std::next(list_2.begin(), 2);
		auto end = list_2.end();

		auto expected_1 = { a, b, c, d, e, h, i, j };
		auto expected_2 = { f, g };

		list_1.splice(pos, list_2, begin, end);

		this->expectSequence(list_1.begin(), list_1.end(), expected_1);
		this->expectSequence(list_2.begin(), list_2.end(), expected_2);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that splicing nodes from a list to itself places the affected
	/// nodes correctly when splicing at the beginning of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ListInterfaceTests, 
		SpliceFromSameListCorrectlySplicesAtBeginningOfList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{a, b, c, d, e};

		auto pos = list.begin();
		auto begin = std::next(list.begin());
		auto end = std::next(list.begin(), 4);

		auto expected = { b, c, d, a, e };

		list.splice(pos, list, begin, end);

		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that splicing nodes from a list to itself places the affected
	/// nodes correctly when splicing somewhere in the middle of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ListInterfaceTests, 
		SpliceFromSameListCorrectlySplicesIntoMiddleOfList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{a, b, c, d, e};

		auto pos = std::next(list.begin());
		auto begin = std::next(list.begin(), 2);
		auto end = std::next(list.begin(), 4);

		auto expected = { a, c, d, b, e };

		list.splice(pos, list, begin, end);

		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that splicing nodes from a list to itself places the affected
	/// nodes correctly when splicing at the end of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ListInterfaceTests, 
		SpliceFromSameListCorrectlySplicesAtEndOfList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{a, b, c, d, e};

		auto pos = list.end();
		auto begin = std::next(list.begin());
		auto end = std::next(list.begin(), 4);

		auto expected = { a, e, b, c, d };

		list.splice(pos, list, begin, end);

		this->expectSequence(list.begin(), list.end(), expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ListInterfaceTests,
		SpliceFromDifferentListCorrectlySplicesAtBeginningOfTargetList,
		SpliceFromDifferentListCorrectlySplicesIntoMiddleOfTargetList,
		SpliceFromDifferentListCorrectlySplicesAtEndOfTargetList,
		SpliceFromSameListCorrectlySplicesAtBeginningOfList,
		SpliceFromSameListCorrectlySplicesIntoMiddleOfList,
		SpliceFromSameListCorrectlySplicesAtEndOfList
	);
}