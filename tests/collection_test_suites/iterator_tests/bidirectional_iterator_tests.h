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

#include <algorithm>
#include <iterator>
#include <ranges>
#include <vector>
#include <gtest/gtest.h>

#include "concepts/associative.h"
#include "concepts/sequential.h"
#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using BidirectionalIteratorTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(BidirectionalIteratorTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that pre-decrementing a bidirectional iterator returns an 
	/// iterator to the previous value.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BidirectionalIteratorTests, 
		PreDecrementIteratorResultsInExpectedValue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, c };
		const collection_type obj3{ a, b, c };

		auto actual = obj1.end();
		auto expected1 = std::prev(obj2.end(), 1);
		auto expected2 = std::prev(obj3.end(), 2);

		actual--;
		EXPECT_EQ(*actual, *expected1);

		actual--;
		EXPECT_EQ(*actual, *expected2);

		actual--;
		EXPECT_EQ(actual, obj1.begin());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that post-decrementing a bidirectional iterator returns an 
	/// iterator to the previous value.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BidirectionalIteratorTests, 
		PostDecrementIteratorResultsInExpectedValue
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, c };
		const collection_type obj3{ a, b, c };

		auto actual = obj1.end();
		auto expected1 = std::prev(obj2.end(), 1);
		auto expected2 = std::prev(obj3.end(), 2);

		--actual;
		EXPECT_EQ(*actual, *expected1);

		--actual;
		EXPECT_EQ(*actual, *expected2);

		--actual;
		EXPECT_EQ(actual, obj1.begin());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the rbegin/rend iterators of a bidirectional range cover all 
	/// expected values within the object. 
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BidirectionalIteratorTests, 
		ReverseIteratorsCoverExpectedValues
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e };
		auto expected = { e, d, c, b, a };

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.rbegin(), obj.rend(), expected);
		else if (associative<collection_type>)
			this->expectSet(obj.rbegin(), obj.rend(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the crbegin/crend iterators of a bidirectional range cover
	/// all expected values within the object. 
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BidirectionalIteratorTests, 
		ConstReverseIteratorsCoverExpectedValues
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e };
		auto expected = { e, d, c, b, a };

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.crbegin(), obj.crend(), expected);
		else if (associative<collection_type>)
			this->expectSet(obj.crbegin(), obj.crend(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the rbegin/rend iterators cover the reverse sequence of
	/// the same begin/end iterator pair.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		BidirectionalIteratorTests, 
		ReverseIteratorsCoverReverseSequence
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e };

		std::vector<value_type> expected;
		expected.reserve(obj.size());

		for (const auto& e : std::ranges::reverse_view(obj))
			expected.push_back(e);


		this->expectSequence(obj.rbegin(), obj.rend(), expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		BidirectionalIteratorTests,
		PreDecrementIteratorResultsInExpectedValue,
		PostDecrementIteratorResultsInExpectedValue,
		ReverseIteratorsCoverExpectedValues,
		ConstReverseIteratorsCoverExpectedValues,
		ReverseIteratorsCoverReverseSequence
	);
}