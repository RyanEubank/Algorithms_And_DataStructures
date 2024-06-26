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

	template <class T, class container>
	struct TestTypes {
		using element_t = T;
		using collection_t = Stack<T, container>;
	};

	using stack_test_types = testing::Types<
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