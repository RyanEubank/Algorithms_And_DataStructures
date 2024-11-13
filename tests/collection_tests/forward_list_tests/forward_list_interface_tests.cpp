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

#include <string>
#include <gtest/gtest.h>

#include "containers/ForwardList.h"
#include "../../collection_test_suites/collection_test_fixture.h"
#include "../../collection_test_suites/list_interface_tests.h"

namespace collection_tests {

	using test_params = testing::Types<ForwardList<std::string>>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ForwardListTest,
		ListInterfaceTests,
		test_params
	);

	template <class T>
	using ForwardListInterfaceTests = CollectionTest<T>;

	TYPED_TEST_SUITE(ForwardListInterfaceTests, test_params);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the spliceAfter() method splices nodes from one forward list
	/// to another correctly placing the nodes when splicing after the end to
	/// place them back at the beginning of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		SpliceAfterFromDifferentListCorrectlySplicesAtBeginningOfTargetList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list_1{ a, b, c, d, e };
		collection_type list_2{ f, g, h, i, j };

		auto pos = list_1.stable_end();
		auto begin = list_2.stable_end();
		auto end = std::next(list_2.stable_begin(), 2);

		auto expected_1 = { f, g, h, a, b, c, d, e };
		auto expected_2 = { i, j };

		list_1.spliceAfter(pos, list_2, begin, end);

		this->expectSequence(list_1.begin(), list_1.end(), expected_1);
		this->expectSequence(list_2.begin(), list_2.end(), expected_2);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the spliceAfter() method splices nodes from one forward list
	/// to another correctly placing the nodes when splicing them into the 
	/// middle of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		SpliceAfterFromDifferentListCorrectlyPlacesNodesInTheMiddleOfTargetList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list_1{ a, b, c, d, e };
		collection_type list_2{ f, g, h, i, j };

		auto pos = std::next(list_1.stable_begin());
		auto begin = list_2.stable_begin();
		auto end = std::next(list_2.stable_begin(), 3);

		auto expected_1 = { a, b, g, h, i, c, d, e };
		auto expected_2 = { f, j };

		list_1.spliceAfter(pos, list_2, begin, end);

		this->expectSequence(list_1.begin(), list_1.end(), expected_1);
		this->expectSequence(list_2.begin(), list_2.end(), expected_2);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the spliceAfter() method splices nodes from one forward list
	/// to another correctly placing the nodes when splicing after the last node
	/// in the list placing the elements at the end.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		SpliceAfterFromDifferentListCorrectlySplicesAtEndOfTargetList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list_1{ a, b, c, d, e };
		collection_type list_2{ f, g, h, i, j };

		auto pos = std::next(list_1.stable_begin(), 4);
		auto begin = std::next(list_2.stable_begin(), 1);
		auto end =  std::next(list_2.stable_begin(), 4);

		auto expected_1 = { a, b, c, d, e, h, i, j };
		auto expected_2 = { f, g };

		list_1.spliceAfter(pos, list_2, begin, end);

		this->expectSequence(list_1.begin(), list_1.end(), expected_1);
		this->expectSequence(list_2.begin(), list_2.end(), expected_2);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the spliceAfter() method splices from a list to itself and
	/// correctly places the nodes when splicing after the end putting them back
	/// at the beginning.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		SpliceAfterFromSameListCorrectlySplicesAtBeginningOfList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c, d, e };

		auto pos = list.stable_end();
		auto begin = list.stable_begin();
		auto end = std::next(list.stable_begin(), 3);

		auto expected = { b, c, d, a, e };

		list.spliceAfter(pos, list, begin, end);

		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the spliceAfter() method splices from a list to itself and
	/// correctly places the nodes when splicing after a node in the middle of 
	/// the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		SpliceAfterFromSameListCorrectlyPlacesNodesInTheMiddleOfList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c, d, e };

		auto pos = list.stable_begin();
		auto begin = std::next(list.stable_begin(), 1);
		auto end = std::next(list.stable_begin(), 3);

		auto expected = { a, c, d, b, e };

		list.spliceAfter(pos, list, begin, end);

		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the spliceAfter() method  splices from a list to itself and
	/// correctly places the nodes when splicing after the last node in the 
	/// list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		SpliceAfterFromSameListCorrectlySplicesAtEndOfList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c, d, e };

		auto pos = std::next(list.stable_begin(), 4);
		auto begin = list.stable_begin();
		auto end = std::next(list.stable_begin(), 3);

		auto expected = { a, e, b, c, d };

		list.spliceAfter(pos, list, begin, end);

		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertAfter() method correctly places elements after the 
	/// stable begin iterator in the second position in the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		InsertAfterBeginIteratorPlacesElementSecondInList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		list.insertAfter(list.stable_begin(), d);

		auto expected = { a, d, b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertAfter() method correctly places elements after the 
	/// stable end iterator at the beginning of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		InsertAfterEndIteratorPlacesElementFirstInList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		list.insertAfter(list.stable_end(), d);

		auto expected = { d, a, b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertAfter() method returns an iterator to the inserted
	/// element.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		InsertAfterReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		auto result = list.insertAfter(list.stable_begin(), d);

		EXPECT_EQ(*result, d);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceAfter() method correctly places elements after the 
	/// stable begin iterator in the second position in the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		EmplaceAfterBeginIteratorPlacesElementSecondInList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		list.emplaceAfter(list.stable_begin(), d);

		auto expected = { a, d, b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceAfter() method correctly places elements after the 
	/// stable end iterator at the beginning of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		EmplaceAfterEndIteratorPlacesElementFirstInList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		list.emplaceAfter(list.stable_end(), d);

		auto expected = { d, a, b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceAfter() method returns an iterator to the inserted
	/// element.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		EmplaceAfterReturnsIteratorToInsertedElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		auto result = list.emplaceAfter(list.stable_begin(), d);

		EXPECT_EQ(*result, d);
	}


	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertAfter() method correctly places a range of elements 
	/// after the stable begin iterator in the second position in the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		InsertRangeAfterBeginIteratorPlacesElementSecondInList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		auto elements = { d, e, f };
		list.insertAfter(list.stable_begin(), elements.begin(), elements.end());

		auto expected = { a, d, e, f, b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertAfter() method correctly places a range of elements 
	/// after the stable end iterator at the beginning of the list.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		InsertRangeAfterEndIteratorPlacesElementFirstInList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		auto elements = { d, e, f };
		list.insertAfter(list.stable_end(), elements.begin(), elements.end());

		auto expected = { d, e, f, a, b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertAfter() method returns an iterator to the first
	/// inserted element when inserting a range.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests, 
		InsertRangeAfterReturnsIteratorToFirstElementOfRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		auto elements = { d, e, f };
		auto result = list.insertAfter(
			list.stable_begin(), elements.begin(), elements.end());

		EXPECT_EQ(*result, d);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAfter() method correctly deletes the
	/// second element in the list when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests,
		RemoveAfterBeginIteratorDeletesSecondElementInTheList
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		list.removeAfter(list.stable_begin());

		auto expected = { a, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAfter() method correctly deletes the
	/// first element in the list when called with the end iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests,
		RemoveAtIteratorDeletesElementAtCorrectPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		list.removeAfter(list.stable_end());

		auto expected = { b, c };
		this->expectSequence(list.begin(), list.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		ForwardListInterfaceTests,
		RemoveAtIteratorReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type list{ a, b, c };
		auto result = list.removeAfter(list.stable_begin());
		EXPECT_EQ(*result, c);
	}
}