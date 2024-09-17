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

#include <gtest/gtest.h>

#include "collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using SizeTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SizeTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the size method correctly reports the size of the 
	/// collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SizeTests, SizeReturnsCorrectValue) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type empty_obj{};
		const collection_type non_empty_obj { a, b, c };

		EXPECT_EQ(empty_obj.size(), 0);
		EXPECT_EQ(non_empty_obj.size(), 3);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the max_size method returns a sensible maximum value <=
	/// the theoretical maximum for an array of value_type. Sanity check just to
	/// ensure the method compiles and can be called without error.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SizeTests, MaxSizeReturnsReasonableResult) {
		FORWARD_TEST_TYPES();

		const collection_type obj{};

		EXPECT_LE(obj.max_size(), std::numeric_limits<difference_type>::max());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that an empty collection can be cleared without raising an 
	/// exception.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SizeTests, EmptyObjectCanBeClearedWithoutError) {
		FORWARD_TEST_TYPES();

		collection_type obj{};

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_NO_THROW(obj.clear());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that a non-empty collection can be cleared without raising an 
	/// exception.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(SizeTests, ClearEmptiesObject) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj { a, b, c };

		ASSERT_FALSE(obj.isEmpty());
		ASSERT_NE(obj.size(), 0);

		obj.clear();

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.size(), 0);
	}


	REGISTER_TYPED_TEST_SUITE_P(
		SizeTests,
		SizeReturnsCorrectValue,
		MaxSizeReturnsReasonableResult,
		EmptyObjectCanBeClearedWithoutError,
		ClearEmptiesObject
	);
}