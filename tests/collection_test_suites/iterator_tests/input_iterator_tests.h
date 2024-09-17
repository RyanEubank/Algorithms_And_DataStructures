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

#include "concepts/sequential.h"
#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using InputIteratorTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(InputIteratorTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that pre-incrementing an iterator returns an iterator to the next
	/// value.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		InputIteratorTests, 
		PreIncrementIteratorResultsInExpectedValue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, c };
		auto actual = obj1.begin();
		auto expected = std::next(obj2.begin(), 1);

		++actual;
		EXPECT_EQ(*actual, *expected);

		++actual;
		++expected;
		EXPECT_EQ(*actual, *expected);

		++actual;
		EXPECT_EQ(actual, obj1.end());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that post-incrementing an iterator returns an iterator to the next 
	/// value.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		InputIteratorTests, 
		PostIncrementIteratorResultsInExpectedValue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, c };
		const collection_type obj3{ a, b, c };

		auto actual = obj1.begin();
		auto expected1 = std::next(obj2.begin(), 1);
		auto expected2 = std::next(obj3.begin(), 2);

		actual++;
		EXPECT_EQ(*actual, *expected1);

		actual++;
		EXPECT_EQ(*actual, *expected2);

		actual++;
		EXPECT_EQ(actual, obj1.end());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the regular begin/end iterators of a range cover all expected
	/// values within the object.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(InputIteratorTests, IteratorsCoverExpectedValues) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e };
		auto expected = { a, b, c, d, e };

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.begin(), obj.end(), expected);
		else
			this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the cbegin/cend iterators of a range cover all expected
	/// values within the object. 
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(InputIteratorTests, ConstIteratorsCoverExpectedValues) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e };
		auto expected = { a, b, c, d, e };

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.cbegin(), obj.cend(), expected);
		else
			this->expectSet(obj.cbegin(), obj.cend(), expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		InputIteratorTests,
		PreIncrementIteratorResultsInExpectedValue,
		PostIncrementIteratorResultsInExpectedValue,
		IteratorsCoverExpectedValues,
		ConstIteratorsCoverExpectedValues
	);
}