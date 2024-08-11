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
#include "algorithms/collection_algorithms.h"

namespace collection_tests {

	TYPED_TEST_SUITE_P(CollectionTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a default constructed collection is initially empty.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, DefaultConstructorCreatesEmptyObject) {
		FORWARD_TEST_TYPES();

		const collection obj{};

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.size(), 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection constructed with an initialization list
	/// correctly sets the contents.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, InitializationConstructorSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const auto input = { a, b, c };
		const collection obj{ a, b, c };

		this->testObjectContainsAllExpectedElements(obj, input);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from an iterator
	/// pair.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, IteratorConstructorSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const auto input = { a, b, c };
		const collection obj(input.begin(), input.end());

		this->testObjectContainsAllExpectedElements(obj, input);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from a range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, RangeConstructorSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const auto input = { a, b, c };
		const collection obj(collections::from_range, input);

		this->testObjectContainsAllExpectedElements(obj, input);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the size method correctly reports the size of the 
	/// collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, SizeReturnsCorrectValue) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection empty_obj{};
		const collection non_empty_obj { a, b, c };

		EXPECT_EQ(empty_obj.size(), 0);
		EXPECT_EQ(non_empty_obj.size(), 3);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that an empty collection can be cleared without raising an 
	/// exception.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, EmptyObjectCanBeClearedWithoutError) {
		FORWARD_TEST_TYPES();

		collection obj{};

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_NO_THROW(obj.clear());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a non-empty collection can be cleared without raising an 
	/// exception.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, ClearEmptiesObject) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj { a, b, c };

		ASSERT_FALSE(obj.isEmpty());
		ASSERT_NE(obj.size(), 0);

		obj.clear();

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.size(), 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's copy constructor makes a deep copy of 
	/// empty objects allowing the original to be destroyed without 
	/// effecting the copy.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, CopyConstructorCopiesEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection target{};
		collection src(target);

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
	TYPED_TEST_P(CollectionTests, CopyConstructorDeepCopiesNonEmptyObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection target { a, b, c };
		collection src(target);

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
	TYPED_TEST_P(CollectionTests, MoveConstructorTransfersEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection target{};
		const collection expected{};

		ASSERT_EQ(target, expected);
		ASSERT_TRUE(target.isEmpty());

		collection src(std::move(target));

		EXPECT_EQ(target, expected);
		EXPECT_EQ(src, expected);
		EXPECT_TRUE(src.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's move constructor correctly moves non-empty 
	/// objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, MoveConstructorTransfersNonEmptyObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection target{ a, b, c };
		const collection expected{ a, b, c };

		ASSERT_EQ(target, expected);
		ASSERT_FALSE(target.isEmpty());

		collection src(std::move(target));

		EXPECT_NE(target, expected);
		EXPECT_EQ(src, expected);
		EXPECT_TRUE(target.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection correctly manages memory when assigned with
	/// copy assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, CopyAssignmentCorrectlyAssignsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection r1{ a, b, c };
		const collection r2{ d, e, f };
		const collection r3{};
		
		collection l1{ a, b, c };
		collection l2{ a, b, c };
		collection l3{ a, b, c };
		collection l4{};

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
	TYPED_TEST_P(CollectionTests, MoveAssignmentTransfersEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection src{};
		collection target{};
		const collection expected{};

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
	TYPED_TEST_P(CollectionTests, MoveAssignmentTransfersNonEmptyObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection src{ a, b, c };
		collection target{ d, e, f };
		const collection expected{ d, e, f };

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
		CollectionTests, 
		MoveAssignmentTransfersBetweenEmptyAndNonEmptyObjects
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection not_expected{ d, e, f };
		collection src1{ a, b, c };
		collection target1{};
		const collection expected1{};

		ASSERT_NE(src1, expected1);

		src1 = std::move(target1);

		EXPECT_EQ(src1, expected1);
		EXPECT_NE(src1, not_expected);

		collection src2{};
		collection target2{ a, b, c };
		const collection expected2{ a, b, c };

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
	TYPED_TEST_P(CollectionTests, SwapSwitchesEmptyObjects) {
		FORWARD_TEST_TYPES();

		collection src{};
		collection target{};
		const collection expected{};

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
	TYPED_TEST_P(CollectionTests, SwapSwitchesNonEmptyObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection src{ a, b, c };
		collection target{ d, e, f };
		const collection expected{ d, e, f };

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
	TYPED_TEST_P(CollectionTests, SwapSwitchesBetweenEmptyAndNonObjects) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection src1{ a, b, c };
		collection target1{};
		const collection expected1{};

		ASSERT_NE(src1, expected1);
		ASSERT_EQ(target1, expected1);

		swap(src1, target1);

		EXPECT_EQ(src1, expected1);
		EXPECT_NE(target1, expected1);

		collection src2{};
		collection target2{ a, b, c };
		const collection expected2{ a, b, c };

		ASSERT_NE(src2, expected2);
		ASSERT_EQ(target2, expected2);

		swap(src2, target2);

		EXPECT_EQ(src2, expected2);
		EXPECT_NE(target2, expected2);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that collection equality will correctly test for collection size.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, CollectionEqualityDependsOnSizeAndElements) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj1{ a, b, c };
		const collection obj2{ a, b, c };
		const collection obj3{ d, e, f };
		const collection obj4{ a, b };

		EXPECT_EQ(obj1, obj2);
		EXPECT_NE(obj1, obj3);
		EXPECT_NE(obj1, obj4);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the stream operators correctly write to, and read from back
	/// into the collection maintaining the collection size, i.e. does not
	/// lose elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, IOStreamOperatorsMaintainObject) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj1{ a, b, c };
		collection obj2{};

		ASSERT_NE(obj1, obj2);

		std::stringstream stream;
		stream << obj1;
		stream >> obj2;

		EXPECT_EQ(obj1, obj2);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		CollectionTests,
		DefaultConstructorCreatesEmptyObject,
		InitializationConstructorSetsContents,
		IteratorConstructorSetsContents,
		RangeConstructorSetsContents,
		SizeReturnsCorrectValue,
		EmptyObjectCanBeClearedWithoutError,
		ClearEmptiesObject,
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
		SwapSwitchesBetweenEmptyAndNonObjects,
		CollectionEqualityDependsOnSizeAndElements,
		IOStreamOperatorsMaintainObject
	);
}