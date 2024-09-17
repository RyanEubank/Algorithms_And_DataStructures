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

#include <cstdint>
#include <gtest/gtest.h>

#include "adapters/Stack.h"
#include "containers/LinkedList.h"
#include "containers/DynamicArray.h"

#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using stack_test_types = testing::Types<
		Stack<uint8_t, DynamicArray<uint8_t>>, 
		Stack<uint8_t, LinkedList<uint8_t>>
	>;

	template <class stack_t>
	using StackTest = CollectionTest<stack_t>;

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

		collection_type obj{};

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

		collection_type obj{ a, b, c };

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

		collection_type obj{ a, b, c };

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

		collection_type obj{ a, b, c, d };

		EXPECT_EQ(obj.peek(), d);
	}
}