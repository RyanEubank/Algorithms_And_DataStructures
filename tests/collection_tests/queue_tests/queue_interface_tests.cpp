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

#include "containers/LinkedList.h"
#include "containers/DynamicArray.h"

#include "queue_test_fixture.h"

namespace collection_tests {

	template <class container, class mock>
	struct TestTypes {
		using collection_t	= Queue<typename container::value_type, container>;
		using mock_t		= Queue<typename container::value_type, mock>;
	};

	using queue_test_types = testing::Types<
		TestTypes<
			DynamicArray<uint8_t, std::allocator<uint8_t>>, 
			DynamicArray<uint8_t, MockAllocator<uint8_t>>
		>, 
		TestTypes<
			LinkedList<uint8_t, std::allocator<uint8_t>>, 
			LinkedList<uint8_t, MockAllocator<uint8_t>>
		>,
		TestTypes<
			DynamicArray<uint16_t, std::allocator<uint16_t>>, 
			DynamicArray<uint16_t, MockAllocator<uint16_t>>
		>,
		TestTypes<
			LinkedList<uint16_t, std::allocator<uint16_t>>, 
			LinkedList<uint16_t, MockAllocator<uint16_t>>
		>,
		TestTypes<
			DynamicArray<uint32_t, std::allocator<uint32_t>>, 
			DynamicArray<uint32_t, MockAllocator<uint32_t>>
		>,
		TestTypes<
			LinkedList<uint32_t, std::allocator<uint32_t>>, 
			LinkedList<uint32_t, MockAllocator<uint32_t>>
		>,
		TestTypes<
			DynamicArray<uint64_t, std::allocator<uint64_t>>, 
			DynamicArray<uint64_t, MockAllocator<uint64_t>>
		>,
		TestTypes<
			LinkedList<uint64_t, std::allocator<uint64_t>>, 
			LinkedList<uint64_t, MockAllocator<uint64_t>>
		>,
		TestTypes<
			DynamicArray<void*, std::allocator<void*>>, 
			DynamicArray<void*, MockAllocator<void*>>
		>,
		TestTypes<
			LinkedList<void*, std::allocator<void*>>, 
			LinkedList<void*, MockAllocator<void*>>
		>,

		TestTypes<
			DynamicArray<std::string, std::allocator<std::string>>, 
			DynamicArray<std::string, MockAllocator<std::string>>
		>,
		TestTypes<
			LinkedList<std::string, std::allocator<std::string>>, 
			LinkedList<std::string, MockAllocator<std::string>>
		>
	>;

	template <class params>
	class QueueTest :public CollectionTests<params> {};

	TYPED_TEST_SUITE(QueueTest, queue_test_types);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the enqueue_front method inserts an element into an 
	/// empty queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		EnqueueFrontOnEmptyInsertsElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};

		ASSERT_TRUE(obj.isEmpty());

		obj.enqueue_front(a);

		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.front(), a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the enqueue_back method inserts an element into an empty 
	/// queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		EnqueueBackOnEmptyInsertsElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};

		ASSERT_TRUE(obj.isEmpty());

		obj.enqueue_back(a);

		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.back(), a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the enqueue_front method inserts elements at the front of 
	/// the queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		EnqueueFrontInsertsElementAtFront
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_FALSE(obj.isEmpty());

		obj.enqueue_front(d);

		EXPECT_EQ(obj.size(), 4);
		EXPECT_EQ(obj.front(), d);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the enqueue_back method inserts elements at the back of the 
	/// queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		EnqueueBackInsertsElementAtBack
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_FALSE(obj.isEmpty());

		obj.enqueue_back(d);

		EXPECT_EQ(obj.size(), 4);
		EXPECT_EQ(obj.back(), d);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the dequeue_front method removes the element at the front of 
	/// the queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		DequeueFrontRemovesElementAtFront
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };
		ASSERT_FALSE(obj.isEmpty());

		obj.dequeue_front();

		EXPECT_EQ(obj.size(), 2);
		EXPECT_EQ(obj.front(), b);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the dequeue_back method removes the element at the back of 
	/// the queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		DequeueBackRemovesElementAtFront
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_FALSE(obj.isEmpty());

		obj.dequeue_back();

		EXPECT_EQ(obj.size(), 2);
		EXPECT_EQ(obj.back(), b);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the front method returns the element at the front of the 
	/// queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		FrontReturnsElementAtFrontOfQueue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};

		obj.enqueue_front(a);
		EXPECT_EQ(obj.front(), a);

		obj.enqueue_front(b);
		EXPECT_EQ(obj.front(), b);

		obj.enqueue_front(c);
		EXPECT_EQ(obj.front(), c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the back method returns the element at the back of the 
	/// queue.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		QueueTest,
		BackReturnsElementAtBackOfQueue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};

		obj.enqueue_front(a);
		EXPECT_EQ(obj.back(), a);

		obj.enqueue_front(b);
		EXPECT_EQ(obj.back(), a);

		obj.enqueue_back(c);
		EXPECT_EQ(obj.back(), c);
	}
}