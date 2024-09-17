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

#include "containers/DynamicArray.h"

#include "../../mocks/mock_allocator.h"
#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using ::testing::_;

	using test_params = 
		testing::Types<DynamicArray<std::string, MockAllocator<std::string>>>;

	template <class T>
	class AllocationTest : 
		public CollectionTest<T>, 
		public MockAllocatorTest<typename T::value_type> 
	{

	};

	TYPED_TEST_SUITE(AllocationTest, test_params);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the default constructor of a DynamicArray does not allocate
	/// any memory.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest,
		DefaultConstructorDoesNotAllocate
	) {
		FORWARD_TEST_TYPES();

		EXPECT_CALL(this->allocator(), allocate(_)).Times(0);

		const collection_type obj{};

		EXPECT_EQ(obj.capacity(), 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve constructor of a DynamicArray allocates the
	/// correct amount of memory requested.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest,
		ReserveConstructorAllocatesSpecifiedMemory
	) {
		FORWARD_TEST_TYPES();
		const size_t size = 30;

		EXPECT_CALL(this->allocator(), allocate(30)).Times(1);

		const collection_type obj(Reserve{ size });

		EXPECT_EQ(obj.capacity(), size);
		EXPECT_TRUE(obj.isEmpty());

		EXPECT_CALL(this->allocator(), deallocate(_, 30)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the trim method corrctly reallocates space to match the
	/// size of the array's actual contents.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(AllocationTest, TrimResizesArrayToMatchContents) {
		FORWARD_TEST_TYPES();

		auto size = Size(30);
		auto capacity = 50;

		ASSERT_GT(capacity, size.get());

		EXPECT_CALL(this->allocator(), allocate(30)).Times(1);
		collection_type obj(size);

		EXPECT_CALL(this->allocator(), allocate(50)).Times(1);
		EXPECT_CALL(this->allocator(), deallocate(_, 30)).Times(1);
		obj.reserve(50);

		ASSERT_EQ(obj.size(), size.get());
		ASSERT_EQ(obj.capacity(), capacity);

		EXPECT_CALL(this->allocator(), allocate(30)).Times(1);
		EXPECT_CALL(this->allocator(), deallocate(_, 50)).Times(1);
		obj.trim();

		EXPECT_EQ(obj.capacity(), size.get());
		EXPECT_CALL(this->allocator(), deallocate(_, 30)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve method correctly allocates the requested memory
	/// on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(AllocationTest, ReserveOnEmptyObjectAllocatesSpace) {
		FORWARD_TEST_TYPES();
		auto size = 10;

		collection_type obj{};

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), 0);
		EXPECT_CALL(this->allocator(), allocate(size)).Times(1);

		obj.reserve(size);

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.capacity(), size);

		EXPECT_CALL(this->allocator(), deallocate(_, size)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve method correctly reallocates the requested memory
	/// on non-empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest, 
		ReserveOnNonEmptyObjectAllocatesMoreSpace
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto newSize = 10;

		EXPECT_CALL(this->allocator(), allocate(3)).Times(1);
		collection_type obj{ a, b, c };

		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);
		EXPECT_CALL(this->allocator(), deallocate(_, 3)).Times(1);
		obj.reserve(newSize);

		EXPECT_EQ(obj.size(), 3);
		EXPECT_EQ(obj.capacity(), newSize);

		EXPECT_CALL(this->allocator(), deallocate(_, newSize)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly allocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest, 
		ResizeOnEmptyAllocatesAndSetsContents
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};

		EXPECT_CALL(this->allocator(), allocate(10)).Times(1);
		obj.resize(10, a);
		EXPECT_CALL(this->allocator(), deallocate(_, 10)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly reallocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest,
		ResizeOnNonEmptyAllocatesAndSetsContents
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto newSize = 10;

		EXPECT_CALL(this->allocator(), allocate(3)).Times(1);
		collection_type obj{ a, b, c };

		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);
		EXPECT_CALL(this->allocator(), deallocate(_, 3)).Times(1);
		obj.resize(newSize, d);
		EXPECT_CALL(this->allocator(), deallocate(_, newSize)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that insertion into an empty dynamic array correctly allocates
	/// space.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest, 
		InsertAllocatesMemoryWhenEmpty
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};

		EXPECT_CALL(this->allocator(), allocate(_)).Times(1);
		obj.insertBack(a);
		EXPECT_CALL(this->allocator(), deallocate(_, _)).Times(1);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertAtUnstable correctly inserts element
	/// swapping it with the a new end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		AllocationTest, 
		InsertDoublesCapacityWhenFull
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto size = 2u;
		auto newSize = size * 2;

		EXPECT_CALL(this->allocator(), allocate(size)).Times(1);
		collection_type obj(Size{size});

		EXPECT_CALL(this->allocator(), allocate(newSize)).Times(1);
		EXPECT_CALL(this->allocator(), deallocate(_, size)).Times(1);
		obj.insertBack(a);

		EXPECT_CALL(this->allocator(), deallocate(_, newSize)).Times(1);
	}
}
