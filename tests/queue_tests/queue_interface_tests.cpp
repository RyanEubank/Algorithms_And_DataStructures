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

	template <class T, class container>
	struct TestTypes {
		using element_t = T;
		using collection_t = Queue<T, container>;
	};

	using queue_test_types = testing::Types<
		TestTypes<uint8_t, DynamicArray<uint8_t>>, 
		TestTypes<uint8_t, LinkedList<uint8_t>>,
		TestTypes<uint16_t, DynamicArray<uint16_t>>, 
		TestTypes<uint16_t, LinkedList<uint16_t>>,
		TestTypes<uint32_t, DynamicArray<uint32_t>>, 
		TestTypes<uint32_t, LinkedList<uint32_t>>,
		TestTypes<uint64_t, DynamicArray<uint64_t>>, 
		TestTypes<uint64_t, LinkedList<uint64_t>>,
		TestTypes<void*, DynamicArray<void*>>, 
		TestTypes<void*, LinkedList<void*>>,
		TestTypes<std::string, DynamicArray<std::string>>, 
		TestTypes<std::string, LinkedList<std::string>>
	>;

	GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(CollectionTests);

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
		using queue = TypeParam::collection_t;
		auto value = this->testInput.control()[0];

		queue obj{};
		ASSERT_TRUE(obj.isEmpty());

		obj.enqueue_front(value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.front(), value);
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
		using queue = TypeParam::collection_t;
		auto value = this->testInput.control()[0];

		queue obj{};
		ASSERT_TRUE(obj.isEmpty());

		obj.enqueue_back(value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.back(), value);
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
		using queue = TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		queue obj(collections::from_range, input);
		ASSERT_FALSE(obj.isEmpty());

		obj.enqueue_front(value);
		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.front(), value);
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
		using queue = TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		queue obj(collections::from_range, input);
		ASSERT_FALSE(obj.isEmpty());

		obj.enqueue_back(value);
		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.back(), value);
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
		using stack = TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = input[input.size() - 1];

		stack obj(collections::from_range, input);
		ASSERT_FALSE(obj.isEmpty());

		obj.dequeue_front();
		EXPECT_EQ(obj.size(), input.size() - 1);
		EXPECT_NE(obj.front(), value);
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
		using stack = TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = input[input.size() - 1];

		stack obj(collections::from_range, input);
		ASSERT_FALSE(obj.isEmpty());

		obj.dequeue_back();
		EXPECT_EQ(obj.size(), input.size() - 1);
		EXPECT_NE(obj.back(), value);
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
		using queue = TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = input[0];

		queue obj(collections::from_range, input);
		auto expected = obj.front();
		EXPECT_EQ(expected, value);
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
		using queue = TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = input[input.size() - 1];

		queue obj(collections::from_range, input);
		auto expected = obj.back();
		EXPECT_EQ(expected, value);
	}
}