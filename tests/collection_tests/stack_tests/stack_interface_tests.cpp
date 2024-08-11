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

#include "stack_test_fixture.h"

namespace collection_tests {

	template <class container, class mock>
	struct TestTypes {
		using collection_t	= Stack<typename container::value_type, container>;
		using mock_t		= Stack<typename container::value_type, mock>;
	};

	using stack_test_types = testing::Types<
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
	class StackTest :public CollectionTests<params> {};

	TYPED_TEST_SUITE(StackTest, stack_test_types);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the push method inserts an element into an empty stack.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		StackTest,
		PushOnEmptyStackInsertsElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{};

		ASSERT_TRUE(obj.isEmpty());

		obj.push(a);

		EXPECT_FALSE(obj.isEmpty());
		EXPECT_EQ(obj.peek(), a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the push method inserts elements on the top of the stack.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		StackTest,
		PushInsertsElementOnTop
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_FALSE(obj.isEmpty());

		obj.push(d);

		EXPECT_EQ(obj.size(), 4);
		EXPECT_EQ(obj.peek(), d);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the pop method removes the element at the top of the stack.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		StackTest,
		PopRemovesElementOnTop
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c };

		ASSERT_FALSE(obj.isEmpty());
		ASSERT_EQ(obj.peek(), c);

		obj.pop();

		EXPECT_EQ(obj.size(), 2);
		EXPECT_EQ(obj.peek(), b);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the peek method returns the element at the top of the stack.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		StackTest,
		PeekReturnsElementOnTop
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection obj{ a, b, c, d };

		EXPECT_EQ(obj.peek(), d);
	}
}