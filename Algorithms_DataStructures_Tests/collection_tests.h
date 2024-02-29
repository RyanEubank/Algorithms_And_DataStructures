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

#include "pch.h"
#include "test_inputs.h"
#include "../Algorithms_DataStructures/Source/algorithms/collection_algorithms.h"

namespace collection_tests {

	using namespace collections;

	template<class params> requires collection<typename params::collection_t>
	class CollectionTest : public testing::Test {
	protected:
		struct test_case_data<typename params::element_t> testInput {};
	};

	TYPED_TEST_SUITE_P(CollectionTest);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a default constructed collection is initially empty.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, DefaultConstructorCreatesEmptyObject) {
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
	TYPED_TEST_P(CollectionTest, InitializationConstructorSetsContents) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto data = input.data();

		const collection obj{ data[0], data[1], data[2] };

		EXPECT_EQ(obj.size(), 3);
		EXPECT_TRUE(collections::contains(obj, input[0]));
		EXPECT_TRUE(collections::contains(obj, input[1]));
		EXPECT_TRUE(collections::contains(obj, input[2]));
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from an iterator
	/// pair.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, IteratorRangeConstructorSetsContents) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(input.begin(), input.end());

		EXPECT_EQ(obj.size(), 3);
		EXPECT_TRUE(collections::contains(obj, input[0]));
		EXPECT_TRUE(collections::contains(obj, input[1]));
		EXPECT_TRUE(collections::contains(obj, input[2]));
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from a range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, RangeConstructorSetsContents) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);
		
		EXPECT_EQ(obj.size(), 3);
		EXPECT_TRUE(collections::contains(obj, input[0]));
		EXPECT_TRUE(collections::contains(obj, input[1]));
		EXPECT_TRUE(collections::contains(obj, input[2]));
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that collection equality will correctly test for collection size.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, CollectionEqualityDependsOnSizeAndElements) {
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
	/// Tests that a collection's copy constructor makes a deep copy of the
	/// elements allowing the original to be destroyed without effecting the
	/// copy.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, CopyConstructorDeepCopiesObject) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection original(collections::from_range, input);
		collection copy(original);
		EXPECT_EQ(original, copy);

		copy.clear();
		EXPECT_NE(original, copy);
		EXPECT_NO_THROW({ original.clear(); });
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection's move constructor correctly transfers 
	/// ownership of the elements allowing the original to be destroyed without 
	/// effecting the new owner.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, MoveConstructorTransfersObject) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj1(collections::from_range, input);
		const collection obj2(collections::from_range, input);

		ASSERT_EQ(obj1, obj2);
		ASSERT_FALSE(obj1.isEmpty());

		collection obj3(std::move(obj1));
		EXPECT_NE(obj1, obj2);
		EXPECT_EQ(obj2, obj3);
		EXPECT_TRUE(obj1.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection correctly manages memory when assigned with
	/// copy assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, CopyAssignmentCorrectlyAssignsContents) {
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
	/// Tests that the collection correctly manages memory when moved with
	/// move assignment.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, MoveAssignmentTransfersObject) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj1(collections::from_range, input);
		const collection obj2(collections::from_range, input);
		collection obj3{};

		ASSERT_EQ(obj1, obj2);
		ASSERT_NE(obj1, obj3);

		obj3 = std::move(obj1);

		EXPECT_EQ(obj3, obj2);
		EXPECT_NE(obj1, obj2);
		EXPECT_TRUE(obj1.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that swapping collections correctly swaps elements such that
	/// a collection equal to another will also report equality in a transfer
	/// collection after a swap.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, SwapChangesElementsCorrectly) {
		using collection = typename TypeParam::collection_t;
		auto control_input = this->testInput.control();
		auto diff_elements_input = this->testInput.different_elements();

		collection obj1(collections::from_range, control_input);
		collection obj2(collections::from_range, control_input);
		collection obj3(collections::from_range, diff_elements_input);

		ASSERT_EQ(obj1, obj2);
		ASSERT_NE(obj1, obj3);

		swap(obj1, obj3);

		EXPECT_EQ(obj2, obj3);
		EXPECT_NE(obj1, obj2);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the stream operators correctly write to, and read from back
	/// into the collection maintaining the collection size, i.e. does not
	/// lose elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionTest, IOStreamOperatorsMaintainObject) {
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
	TYPED_TEST_P(CollectionTest, EmptyObjectCanBeClearedWithoutError) {
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
	TYPED_TEST_P(CollectionTest, ClearEmptiesObject) {
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
		CollectionTest,
		DefaultConstructorCreatesEmptyObject,
		InitializationConstructorSetsContents,
		IteratorRangeConstructorSetsContents,
		RangeConstructorSetsContents,
		CollectionEqualityDependsOnSizeAndElements,
		CopyConstructorDeepCopiesObject,
		MoveConstructorTransfersObject,
		CopyAssignmentCorrectlyAssignsContents,
		MoveAssignmentTransfersObject,
		SwapChangesElementsCorrectly,
		EmptyObjectCanBeClearedWithoutError,
		ClearEmptiesObject,
		IOStreamOperatorsMaintainObject
	);
}