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

#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using test_params = testing::Types<DynamicArray<std::string>>;

	template <class T>
	using DynamicArrayInterfaceTest = CollectionTest<T>;

	TYPED_TEST_SUITE(DynamicArrayInterfaceTest, test_params);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the set size constructor of a DynamicArray correctly
	/// constructs and fills the array with the expected value.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest,
		SizeConstructorCreatesObjectFilledWithSetAmount
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj(Size{ 3u }, b);
		auto expected = { b, b, b };

		EXPECT_EQ(obj.capacity(), 3);
		EXPECT_EQ(obj.size(), 3);
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reserve constructor of a DynamicArray throws an
	/// exception when called with a negative amount.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest,
		ConstructorFailsWhenBuiltWithNegativeCapacity
	) {
		FORWARD_TEST_TYPES();

		auto test = []() {
			auto amount = Reserve(-1);
			const collection_type obj(amount);
		};

		EXPECT_THROW(test(), std::exception);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the trim method corrctly reallocates space to match the
	/// size of the array's actual contents.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(DynamicArrayInterfaceTest, TrimResizesArrayToMatchContents) {
		FORWARD_TEST_TYPES();

		auto size = Size(30);
		auto capacity = 50;

		ASSERT_GT(capacity, size.get());

		collection_type obj(size);
		obj.reserve(50);

		ASSERT_EQ(obj.size(), size.get());
		ASSERT_EQ(obj.capacity(), capacity);

		obj.trim();

		EXPECT_EQ(obj.capacity(), size.get());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly allocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest, 
		ResizeOnEmptyAllocatesAndSetsContents
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		auto expected = { a, a, a, a, a, a, a, a, a, a };
		obj.resize(10, a);

		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the resize method correctly reallocates and constructs the 
	/// requested elements on empty objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest,
		ResizeOnNonEmptyAllocatesAndSetsContents
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto newSize = 10;

		collection_type obj{ a, b, c };
		obj.resize(newSize, d);

		auto expected = { a, b, c, d, d, d, d, d, d, d };
		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertUnstable correctly inserts the requested 
	/// element by swapping the element at the current position to the end.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest, 
		UnstableInsertCorrectlyInsertsElementAtIndex
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		obj.insertUnstable(Index(1), d);
		auto expected = {a, d, c, b};

		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method insertUnstable correctly inserts the requested 
	/// element by swapping the element at the current position to the end.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest,
		UnstableInsertCorrectlyInsertsElementAtIterator
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		auto it = std::next(obj.begin());
		obj.insertUnstable(it, d);
		auto expected = {a, d, c, b};

		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method removeAtUnstable correctly removes the requested
	/// index swapping it with the end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest, 
		UnstableRemoveCorrectlyRemovesElementAtIndex
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		obj.removeUnstable(Index(0));
		auto expected = { c, b };

		this->expectSequence(obj.begin(), obj.end(), expected);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the method removeAtUnstable correctly removes the requested
	/// iterator swapping it with the end element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST(
		DynamicArrayInterfaceTest, 
		UnstableRemoveCorrectlyRemovesElementAtIterator
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, c };
		obj.removeUnstable(obj.begin());
		auto expected = { c, b };

		this->expectSequence(obj.begin(), obj.end(), expected);
	}
}
