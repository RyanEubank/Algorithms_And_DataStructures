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

#include "../test_data/test_inputs.h"
#include "algorithms/collection_algorithms.h"

namespace collection_tests {

	using namespace collections;

	template<class params> requires collection<typename params::collection_t>
	class CollectionTests : public testing::Test {
	protected:
		struct test_case_data<typename params::element_t> testInput {};

		void testContentsMatchInput(auto input, auto obj) {
			EXPECT_EQ(obj.size(), input.size());
			if constexpr (
				std::ranges::input_range<typename params::collection_t>
			) {
				EXPECT_NE(collections::find(obj, input[0]), obj.end());
				EXPECT_NE(collections::find(obj, input[1]), obj.end());
				EXPECT_NE(collections::find(obj, input[2]), obj.end());
			}
		}
	};

	TYPED_TEST_SUITE_P(CollectionTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a default constructed collection is initially empty.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, DefaultConstructorCreatesEmptyObject) {
		using collection = typename TypeParam::collection_t;

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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto data = input.data();

		const collection obj{ data[0], data[1], data[2] };
		this->testContentsMatchInput(input, obj);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from an iterator
	/// pair.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, IteratorRangeConstructorSetsContents) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(input.begin(), input.end());
		this->testContentsMatchInput(input, obj);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from a range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, RangeConstructorSetsContents) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);
		this->testContentsMatchInput(input, obj);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that collection equality will correctly test for collection size.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, CollectionEqualityDependsOnSizeAndElements) {
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		auto diff_elements_input = this->testInput.different_elements();
		auto diff_size_input = this->testInput.different_size();

		const collection obj1(collections::from_range, control_input);
		const collection obj2(collections::from_range, control_input);
		const collection obj3(collections::from_range, diff_elements_input);
		const collection obj4(collections::from_range, diff_size_input);

		EXPECT_EQ(obj1, obj2);
		EXPECT_NE(obj1, obj3);
		EXPECT_NE(obj1, obj4);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's copy constructor makes a deep copy of 
	/// empty objects allowing the original to be destroyed without 
	/// effecting the copy.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, CopyConstructorCopiesEmptyObjects) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection target(collections::from_range, input);
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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection target(collections::from_range, input);
		const collection expected(collections::from_range, input);

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
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		auto diff_input = this->testInput.different_elements();

		const collection r1(collections::from_range, control_input);
		const collection r2(collections::from_range, diff_input);
		const collection r3{};
		
		collection l1(collections::from_range, control_input);
		collection l2(collections::from_range, control_input);
		collection l3(collections::from_range, control_input);
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
		using collection = typename TypeParam::collection_t;

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
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		auto diff_input = this->testInput.different_elements();

		collection src(collections::from_range, control_input);
		collection target(collections::from_range, diff_input);
		const collection expected(collections::from_range, diff_input);

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
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		auto diff_input = this->testInput.different_elements();

		const collection not_expected(collections::from_range, diff_input);
		collection src1(collections::from_range, control_input);
		collection target1{};
		const collection expected1{};

		ASSERT_NE(src1, expected1);
		src1 = std::move(target1);
		EXPECT_EQ(src1, expected1);
		EXPECT_NE(src1, not_expected);

		collection src2{};
		collection target2(collections::from_range, control_input);
		const collection expected2(collections::from_range, control_input);

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
		using collection = typename TypeParam::collection_t;

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
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		auto diff_input = this->testInput.different_elements();

		collection src(collections::from_range, control_input);
		collection target(collections::from_range, diff_input);
		const collection expected(collections::from_range, diff_input);

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
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		
		collection src1(collections::from_range, control_input);
		collection target1{};
		const collection expected1{};

		ASSERT_NE(src1, expected1);
		ASSERT_EQ(target1, expected1);
		swap(src1, target1);
		EXPECT_EQ(src1, expected1);
		EXPECT_NE(target1, expected1);

		collection src2{};
		collection target2(collections::from_range, control_input);
		const collection expected2(collections::from_range, control_input);

		ASSERT_NE(src2, expected2);
		ASSERT_EQ(target2, expected2);
		swap(src2, target2);
		EXPECT_EQ(src2, expected2);
		EXPECT_NE(target2, expected2);
	}


	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the stream operators correctly write to, and read from back
	/// into the collection maintaining the collection size, i.e. does not
	/// lose elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, IOStreamOperatorsMaintainObject) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj1(collections::from_range, input);
		collection obj2{};

		ASSERT_NE(obj1, obj2);

		std::stringstream stream;
		stream << obj1;
		stream >> obj2;

		EXPECT_EQ(obj1, obj2);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that an empty collection can be cleared without raising an 
	/// exception.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTests, EmptyObjectCanBeClearedWithoutError) {
		using collection = typename TypeParam::collection_t;

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
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		ASSERT_NE(obj.size(), 0);

		obj.clear();

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.size(), 0);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		CollectionTests,
		DefaultConstructorCreatesEmptyObject,
		InitializationConstructorSetsContents,
		IteratorRangeConstructorSetsContents,
		RangeConstructorSetsContents,
		CollectionEqualityDependsOnSizeAndElements,
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
		EmptyObjectCanBeClearedWithoutError,
		ClearEmptiesObject,
		IOStreamOperatorsMaintainObject
	);
}