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

#include <iterator>
#include <gtest/gtest.h>

#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using RandomAccessIteratorTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(RandomAccessIteratorTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that adding an integral value to a random-access iterator returns
	/// an iterator to the expected value.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		RandomAccessIteratorTests, 
		AddToIteratorReturnsIteratorToExpectedValue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c, d, e };
		const collection_type obj2{ a, b, c, d, e };
		const collection_type obj3{ a, b, c, d, e };
		const collection_type obj4{ a, b, c, d, e };

		auto expected1 = std::next(obj2.begin(), 1);
		auto expected2 = std::next(obj3.begin(), 2);

		auto actual = obj1.begin() + 1;
		EXPECT_EQ(*actual, *expected1);

		actual += 1;
		EXPECT_EQ(*actual, *expected2);

		actual = obj4.begin();
		actual += 5;
		EXPECT_EQ(actual, obj4.end());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that subtracting an integral value to a random-access iterator 
	/// returns an iterator to the expected value.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		RandomAccessIteratorTests, 
		SubtractFromIteratorReturnsIteratorToExpectedValue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c, d, e };
		const collection_type obj2{ a, b, c, d, e };
		const collection_type obj3{ a, b, c, d, e };
		const collection_type obj4{ a, b, c, d, e };

		auto expected1 = std::prev(obj2.end(), 1);
		auto expected2 = std::prev(obj3.end(), 2);

		auto end = obj1.end();
		auto actual = end - 1;
		EXPECT_EQ(*actual, *expected1);

		actual -= 1;
		EXPECT_EQ(*actual, *expected2);

		actual = obj4.end();
		actual -= 5;
		EXPECT_EQ(actual, obj4.begin());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that subtracting two random access iterators results in the same
	/// distance between them in terms of increments/decrements.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		RandomAccessIteratorTests, 
		SubtractIteratorsReturnsDistance
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e };
		auto begin = obj.begin();
		auto end = obj.end();
		auto size = end - begin;

		EXPECT_EQ(obj.size(), size);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		RandomAccessIteratorTests,
		AddToIteratorReturnsIteratorToExpectedValue,
		SubtractFromIteratorReturnsIteratorToExpectedValue,
		SubtractIteratorsReturnsDistance
	);
}