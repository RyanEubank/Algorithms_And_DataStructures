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
	using AssignmentTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(AssignmentTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's copy constructor makes a deep copy of 
	/// empty objects allowing the original to be destroyed without 
	/// effecting the copy.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssignmentTests, CopyConstructorCopiesEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection_type target{};
		collection_type src(target);

		EXPECT_EQ(src, target);

		src.clear();

		EXPECT_EQ(target, src);
		EXPECT_NO_THROW({ target.clear(); });
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's copy constructor makes a deep copy of 
	/// non-empty objects allowing the original to be destroyed without 
	/// effecting the copy.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssignmentTests, 
		CopyConstructorDeepCopiesNonEmptyObjects
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type target { a, b, c };
		collection_type src(target);

		EXPECT_EQ(src, target);

		src.clear();

		EXPECT_NE(target, src);
		EXPECT_NO_THROW({ target.clear(); });
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's move constructor correctly moves empty 
	/// objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssignmentTests, 
		MoveConstructorTransfersEmptyObjects
	) {
		FORWARD_TEST_TYPES();

		collection_type target{};
		const collection_type expected{};

		ASSERT_EQ(target, expected);
		ASSERT_TRUE(target.isEmpty());

		collection_type src(std::move(target));

		EXPECT_EQ(target, expected);
		EXPECT_EQ(src, expected);
		EXPECT_TRUE(src.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's move constructor correctly moves non-empty 
	/// objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssignmentTests, 
		MoveConstructorTransfersNonEmptyObjects
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type target{ a, b, c };
		const collection_type expected{ a, b, c };

		ASSERT_EQ(target, expected);
		ASSERT_FALSE(target.isEmpty());

		collection_type src(std::move(target));

		EXPECT_NE(target, expected);
		EXPECT_EQ(src, expected);
		EXPECT_TRUE(target.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection correctly manages memory when assigned with
	/// copy assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssignmentTests, 
		CopyAssignmentCorrectlyAssignsContents
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type r1{ a, b, c };
		const collection_type r2{ d, e, f };
		const collection_type r3{};

		collection_type l1{ a, b, c };
		collection_type l2{ a, b, c };
		collection_type l3{ a, b, c };
		collection_type l4{};

		ASSERT_EQ(l1, r1);
		ASSERT_NE(l2, r2);
		ASSERT_NE(l3, r3);
		ASSERT_NE(l4, r1);

		l1 = r1;
		l2 = r2;
		l3 = r3;
		l4 = r1;

		EXPECT_EQ(l1, r1);
		EXPECT_EQ(l2, r2);
		EXPECT_EQ(l3, r3);
		EXPECT_EQ(l4, r1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection correctly manages memory on empty objects
	/// when moved via assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssignmentTests, MoveAssignmentTransfersEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection_type src{};
		collection_type target{};
		const collection_type expected{};

		ASSERT_EQ(src, expected);
		ASSERT_EQ(target, expected);

		src = std::move(target);

		EXPECT_EQ(src, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection correctly manages memory on non-empty objects
	/// when moved via assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssignmentTests, 
		MoveAssignmentTransfersNonEmptyObjects
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type src{ a, b, c };
		collection_type target{ d, e, f };
		const collection_type expected{ d, e, f };

		ASSERT_NE(src, expected);

		src = std::move(target);

		EXPECT_EQ(src, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection correctly manages memory when moved
	/// between empty and non empty objects via move assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssignmentTests, 
		MoveAssignmentTransfersBetweenEmptyAndNonEmptyObjects
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type not_expected{ d, e, f };
		collection_type src1{ a, b, c };
		collection_type target1{};
		const collection_type expected1{};

		ASSERT_NE(src1, expected1);

		src1 = std::move(target1);

		EXPECT_EQ(src1, expected1);
		EXPECT_NE(src1, not_expected);

		collection_type src2{};
		collection_type target2{ a, b, c };
		const collection_type expected2{ a, b, c };

		ASSERT_NE(src2, expected2);

		src2 = std::move(target2);

		EXPECT_EQ(src2, expected2);
		EXPECT_NE(src2, not_expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that swapping collections correctly swaps elements on empty 
	/// objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssignmentTests, SwapSwitchesEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection_type src{};
		collection_type target{};
		const collection_type expected{};

		ASSERT_EQ(src, expected);
		ASSERT_EQ(target, expected);

		swap(src, target);

		EXPECT_EQ(src, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that swapping collections correctly swaps elements on non-empty 
	/// objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssignmentTests, SwapSwitchesNonEmptyObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type src{ a, b, c };
		collection_type target{ d, e, f };
		const collection_type expected{ d, e, f };

		ASSERT_NE(src, expected);
		ASSERT_EQ(target, expected);

		swap(src, target);

		EXPECT_EQ(src, expected);
		EXPECT_NE(target, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that swapping collections correctly swaps elements between empty
	/// and non-empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssignmentTests, SwapSwitchesBetweenEmptyAndNonObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type src1{ a, b, c };
		collection_type target1{};
		const collection_type expected1{};

		ASSERT_NE(src1, expected1);
		ASSERT_EQ(target1, expected1);

		swap(src1, target1);

		EXPECT_EQ(src1, expected1);
		EXPECT_NE(target1, expected1);

		collection_type src2{};
		collection_type target2{ a, b, c };
		const collection_type expected2{ a, b, c };

		ASSERT_NE(src2, expected2);
		ASSERT_EQ(target2, expected2);

		swap(src2, target2);

		EXPECT_EQ(src2, expected2);
		EXPECT_NE(target2, expected2);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssignmentTests,
		CopyConstructorCopiesEmptyObjects,
		CopyConstructorDeepCopiesNonEmptyObjects,
		MoveConstructorTransfersEmptyObjects,
		MoveConstructorTransfersNonEmptyObjects,
		CopyAssignmentCorrectlyAssignsContents,
		MoveAssignmentTransfersEmptyObjects,
		MoveAssignmentTransfersNonEmptyObjects,
		MoveAssignmentTransfersBetweenEmptyAndNonEmptyObjects,
		SwapSwitchesEmptyObjects,
		SwapSwitchesNonEmptyObjects,
		SwapSwitchesBetweenEmptyAndNonObjects
	);
}