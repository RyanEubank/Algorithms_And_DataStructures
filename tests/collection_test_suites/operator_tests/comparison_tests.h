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

#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using ComparisonTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(ComparisonTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns equal for identical
	/// objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ComparisonTests, IdenticalObjectsCompareEqual) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, c };

		EXPECT_TRUE(obj1 <=> obj2 == 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns less than (<) when
	/// comparing to another object with strictly larger elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		ComparisonTests, 
		ObjectComparesLessThanAnotherWithGreaterElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ d, e, f };

		EXPECT_TRUE(obj1 <=> obj2 < 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns greater than (>) 
	/// when comparing to another object with strictly smaller elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		ComparisonTests, 
		ObjectComparesGreaterThanAnotherWithSmallerElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ d, e, f };

		EXPECT_TRUE(obj2 <=> obj1 > 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns greater than (>)
	/// when comparing a larger object on the left with a smaller object on the
	/// right that is strictly a subset of the larger.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ComparisonTests, ObjectWithMoreElementsComparesGreaterThan) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b };

		EXPECT_TRUE(obj1 <=> obj2 > 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns less than (<) when
	/// comparing a smaller object on the left with a larger object on the
	/// right that is a superset of the smaller.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ComparisonTests, ObjectWithFewerElementsComparesLessThan) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b };

		EXPECT_TRUE(obj2 <=> obj1 < 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns less than (<) when
	/// comparing an object with another equal size object that contains at
	/// least one lexicographically greater element in its ordering.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		ComparisonTests, 
		ObjectComparesLessThanAnotherWithGreaterElementAtSomePosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, e };

		EXPECT_TRUE(obj1 <=> obj2 < 0);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the three way comparison operator returns greater than (>) 
	/// when comparing an object with another equal size object that contains at
	/// least one lexicographically smaller element in its ordering.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		ComparisonTests, 
		ObjectComparesGreaterThanAnotherWithSmallerElementAtSomePosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, e };

		EXPECT_TRUE(obj2 <=> obj1 > 0);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ComparisonTests,
		IdenticalObjectsCompareEqual,
		ObjectComparesLessThanAnotherWithGreaterElements,
		ObjectComparesGreaterThanAnotherWithSmallerElements,
		ObjectWithMoreElementsComparesGreaterThan,
		ObjectWithFewerElementsComparesLessThan,
		ObjectComparesLessThanAnotherWithGreaterElementAtSomePosition,
		ObjectComparesGreaterThanAnotherWithSmallerElementAtSomePosition
	);
}