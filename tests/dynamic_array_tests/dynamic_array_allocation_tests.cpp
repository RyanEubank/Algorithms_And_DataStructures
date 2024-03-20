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

#include "dynamic_array_test_fixture.h"

namespace collection_tests {

	using collection_types = instantiate_with_elements<DynamicArrayTestTypes>;

	GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(CollectionTests);

	TYPED_TEST_SUITE(DynamicArrayTest, collection_types);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the default constructor of a DynamicArray does not allocate
	/// any memory.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayTest,
		DefaultConstructorDoesNotAllocate
	) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;

		ASSERT_EQ(0, mock_t::DEFAULT_CAPACITY);
		EXPECT_CALL(this->allocator(), allocate(_)).Times(0);
		const mock_t obj{};
		EXPECT_EQ(obj.capacity(), mock_t::DEFAULT_CAPACITY);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve constructor of a DynamicArray allocates the
	/// correct amount of memory requested.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayTest,
		ReserveConstructorAllocatesSpecifiedMemory
	) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;

		const size_t size = 30;
		EXPECT_CALL(this->allocator(), allocate(30));
		const mock_t obj(Reserve{ size });
		EXPECT_EQ(obj.capacity(), size);
		EXPECT_TRUE(obj.isEmpty());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the set size constructor of a DynamicArray correctly
	/// constructs and fills the array with the expected value.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayTest,
		SizeConstructorCreatesObjectFilledWithSetAmount
	) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;
		using element = DynamicArrayTest<TypeParam>::element_t;
		auto value = this->testInput.control()[1];

		ASSERT_NE(element{}, value);
		const size_t size = 3;

		const collection obj(Size{ size }, value);
		EXPECT_EQ(obj.size(), size);

		auto actual = obj.asRawArray();
		element expected[] = { value, value, value };

		for (size_t i = 0; i < size; ++i)
			EXPECT_EQ(actual[i], expected[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve constructor of a DynamicArray throws an
	/// exception when called with a negative amount.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayTest,
		ConstructorFailsWhenBuiltWithNegativeCapacity
	) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;

		auto test = []() {
			auto amount = Reserve(-1);
			const collection obj(amount);
		};

		EXPECT_THROW(test(), std::exception);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the trim method corrctly reallocates space to match the
	/// size of the array's actual contents.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, TrimResizesArrayToMatchContents) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;

		auto size = Size(30);
		auto capacity = 50;

		ASSERT_GT(capacity, size.get());

		collection obj(size);
		obj.reserve(50);

		ASSERT_EQ(obj.size(), size.get());
		ASSERT_EQ(obj.capacity(), capacity);

		obj.trim();
		EXPECT_EQ(obj.capacity(), size.get());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve method correctly allocates the requested memory
	/// on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, ReserveOnEmptyObjectAllocatesSpace) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto size = 10;

		mock_t obj{};

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), 0);
		EXPECT_CALL(this->allocator(), allocate(size)).Times(1);

		obj.reserve(size);

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), size);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve method correctly reallocates the requested memory
	/// on non-empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, ReserveOnNonEmptyObjectAllocatesMoreSpace) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto newSize = 10;
		auto input = this->testInput.control();

		ASSERT_NE(input.size(), newSize);
		EXPECT_CALL(this->allocator(), allocate(input.size())).Times(1);

		mock_t obj(collections::from_range, input);

		EXPECT_EQ(obj.size(), input.size());
		EXPECT_EQ(obj.capacity(), input.size());
		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);

		obj.reserve(newSize);

		EXPECT_EQ(obj.size(), input.size());
		EXPECT_EQ(obj.capacity(), newSize);

		auto actual = obj.asRawArray();
		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(actual[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly allocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, ResizeOnEmptyAllocatesAndSetsContents) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto value = this->testInput.control()[1];
		auto size = 10;

		mock_t obj{};
		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), 0);

		EXPECT_CALL(this->allocator(), allocate(size)).Times(1);
		obj.resize(size, value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), size);

		auto actual = obj.asRawArray();
		for (size_t i = 0; i < size; ++i)
			EXPECT_EQ(actual[i], value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly reallocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, ResizeOnNonEmptyAllocatesAndSetsContents) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto input = this->testInput.control();
		auto value = input[1];
		auto newSize = 10;

		EXPECT_CALL(this->allocator(), allocate(input.size())).Times(1);
		mock_t obj(collections::from_range, input);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), input.size());

		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);
		obj.resize(newSize, value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), newSize);

		auto actual = obj.asRawArray();

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(actual[i], input[i]);

		for (size_t i = input.size(); i < newSize; ++i)
			EXPECT_EQ(actual[i], value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertUnstable correctly inserts the requested 
	/// element by swapping the element at the current position to the end.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, UnstableInsertCorrectlyInsertsElementAtIndex) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;
		using element = DynamicArrayTest<TypeParam>::element_t;
		auto input = this->testInput.control();
		auto index = 1;
		auto value = element{};

		collection obj(collections::from_range, input);
		ASSERT_NE(obj[index], value);

		auto expectedBack = obj[index];
		auto expectedElementAtIndex = obj.back();
		auto expected_size = obj.size() + 1;
		obj.insertUnstable(Index(index), value);

		EXPECT_EQ(expectedBack, obj.back());
		EXPECT_EQ(value, obj[index]);
		EXPECT_EQ(expected_size, obj.size());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertUnstable correctly inserts the requested 
	/// element by swapping the element at the current position to the end.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayTest,
		UnstableInsertCorrectlyInsertsElementAtIterator
	) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;
		using element = DynamicArrayTest<TypeParam>::element_t;
		auto input = this->testInput.control();
		auto value = element{};

		collection obj(collections::from_range, input);

		auto iterator = obj.begin();
		iterator++;
		ASSERT_NE(*iterator, value);

		auto expectedBack = *iterator;
		auto expectedElementAtPosition = obj.back();
		auto expected_size = obj.size() + 1;

		obj.insertUnstable(iterator, value);
		iterator = obj.begin();
		iterator++;

		EXPECT_EQ(expectedBack, obj.back());
		EXPECT_EQ(value, *iterator);
		EXPECT_EQ(expected_size, obj.size());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method removeAtUnstable correctly removes the requested
	/// index swapping it with the end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, UnstableRemoveCorrectlyRemovesElementAtIndex) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;
		using element = DynamicArrayTest<TypeParam>::element_t;
		auto input = this->testInput.control();
		auto index = 0;

		collection obj(collections::from_range, input);

		
		auto expectedElementAtIndex = obj.back();
		auto expected_size = obj.size() - 1;
		obj.removeUnstable(Index(index));

		EXPECT_EQ(expectedElementAtIndex, obj[index]);
		EXPECT_EQ(expected_size, obj.size());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method removeAtUnstable correctly removes the requested
	/// iterator swapping it with the end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayTest, 
		UnstableRemoveCorrectlyRemovesElementAtIterator
	) {
		using collection = DynamicArrayTest<TypeParam>::collection_t;
		using element = DynamicArrayTest<TypeParam>::element_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);

		auto iterator = obj.begin();
		auto expectedElementAtIndex = obj.back();
		auto expected_size = obj.size() - 1;

		obj.removeUnstable(iterator);
		iterator = obj.begin();

		EXPECT_EQ(expectedElementAtIndex, *iterator);
		EXPECT_EQ(expected_size, obj.size());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that insertion into an empty dynamic array correctly allocates
	/// space.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, InsertAllocatesMemoryWhenEmpty) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto value = this->testInput.control()[0];

		mock_t obj{};

		EXPECT_CALL(this->allocator(), allocate(_)).Times(1);
		obj.insertBack(value);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertAtUnstable correctly inserts element
	/// swapping it with the a new end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, InsertDoublesCapacityWhenFull) {
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto value = this->testInput.control()[0];
		auto num_elements = 2u;
		auto expected_allocation = num_elements * 2;

		EXPECT_CALL(this->allocator(), allocate(num_elements)).Times(1);
		mock_t obj(Size{num_elements});

		EXPECT_CALL(this->allocator(), allocate(expected_allocation)).Times(1);
		obj.insertBack(value);
	}
}
