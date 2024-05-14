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
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj(Size{ 3u }, b);
		auto expected = { b, b, b };
		
		this->testObjectEqualsExpectedSequence(obj, expected);
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
		FORWARD_TEST_TYPES();

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
		FORWARD_TEST_TYPES();

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
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto newSize = 10;

		EXPECT_CALL(this->allocator(), allocate(3)).Times(1);
		mock_t obj{ a, b, c };

		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);
		obj.reserve(newSize);

		EXPECT_EQ(obj.size(), 3);
		EXPECT_EQ(obj.capacity(), newSize);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly allocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, ResizeOnEmptyAllocatesAndSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		mock_t obj{};
		auto expected = { a, a, a, a, a, a, a, a, a, a };

		EXPECT_CALL(this->allocator(), allocate(10)).Times(1);
		obj.resize(10, a);

		this->testObjectEqualsExpectedSequence(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly reallocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, ResizeOnNonEmptyAllocatesAndSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		using mock_t = DynamicArrayTest<TypeParam>::mock_t;
		auto newSize = 10;

		EXPECT_CALL(this->allocator(), allocate(3)).Times(1);
		mock_t obj{ a, b, c };

		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);
		obj.resize(newSize, d);

		auto expected = { a, b, c, d, d, d, d, d, d, d };
		this->testObjectEqualsExpectedSequence(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertUnstable correctly inserts the requested 
	/// element by swapping the element at the current position to the end.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, UnstableInsertCorrectlyInsertsElementAtIndex) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };
		obj.insertUnstable(Index(1), d);
		auto expected = {a, d, c, b};

		this->testObjectEqualsExpectedSequence(obj, expected);
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
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };
		auto iterator = std::next(obj.begin());
		obj.insertUnstable(iterator, d);
		auto expected = {a, d, c, b};

		this->testObjectEqualsExpectedSequence(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method removeAtUnstable correctly removes the requested
	/// index swapping it with the end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, UnstableRemoveCorrectlyRemovesElementAtIndex) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };
		obj.removeUnstable(Index(0));
		auto expected = { c, b };

		this->testObjectEqualsExpectedSequence(obj, expected);
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
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };
		obj.removeUnstable(obj.begin());
		auto expected = { c, b };

		this->testObjectEqualsExpectedSequence(obj, expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that insertion into an empty dynamic array correctly allocates
	/// space.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, InsertAllocatesMemoryWhenEmpty) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;

		mock_t obj{};

		EXPECT_CALL(this->allocator(), allocate(_)).Times(1);
		obj.insertBack(a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertAtUnstable correctly inserts element
	/// swapping it with the a new end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayTest, InsertDoublesCapacityWhenFull) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();
		using mock_t = DynamicArrayTest<TypeParam>::mock_t;

		auto size = 2u;
		auto expected_allocation = size * 2;

		EXPECT_CALL(this->allocator(), allocate(size)).Times(1);
		mock_t obj(Size{size});

		EXPECT_CALL(this->allocator(), allocate(expected_allocation)).Times(1);
		obj.insertBack(a);
	}
}
