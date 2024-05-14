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

#include <list>

#include "../associative_collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class AssociativeIteratorAccessTests : 
		public AssociativeCollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeIteratorAccessTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeIteratorAccessTests, IteratorsCoverFullRange) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto input = { a, b, c };
		auto it = obj.begin();

		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_EQ(it, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the const_iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeIteratorAccessTests, ConstIteratorsCoverFullRange) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto input = { a, b, c };
		auto it = obj.cbegin();

		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_EQ(it, obj.cend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reverse_iterator pair in the range covers the full 
	/// range in reverse order.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeIteratorAccessTests, ReverseIteratorsCoverFullRange) {
		FORWARD_TEST_TYPES();

		if constexpr (!bidirectionally_iterable_collection<collection>)
			GTEST_SKIP();

		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto input = { a, b, c };
		auto it = obj.rbegin();

		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_NE(collections::find(input, *it++), input.end());
		EXPECT_EQ(it, obj.rend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reverse_iterator pair in the range covers the full 
	/// range in reverse order.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorAccessTests, 
		ReverseIteratorsCoverReverseSequence
	) {
		FORWARD_TEST_TYPES();

		if constexpr (!bidirectionally_iterable_collection<collection>)
			GTEST_SKIP();

		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto list = std::list(obj.begin(), obj.end());

		auto actual = obj.rbegin();
		auto expected = list.rbegin();

		EXPECT_EQ(*actual++, *expected++);
		EXPECT_EQ(*actual++, *expected++);
		EXPECT_EQ(*actual++, *expected++);
		EXPECT_EQ(actual, obj.rend());
		EXPECT_EQ(expected, list.rend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeIteratorAccessTests, ConstIteratorsMatchNonConst) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto it = obj.begin();
		auto c_it = obj.cbegin();

		EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(it, obj.end());
		EXPECT_EQ(it, obj.cend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeIteratorAccessTests, 
		ConstReverseIteratorsMatchNonConst
	) {
		FORWARD_TEST_TYPES();

		if constexpr (!bidirectionally_iterable_collection<collection>)
			GTEST_SKIP();

		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto it = obj.rbegin();
		auto c_it = obj.crbegin();

		EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(it, obj.rend());
		EXPECT_EQ(it, obj.crend());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeIteratorAccessTests,
		IteratorsCoverFullRange,
		ConstIteratorsCoverFullRange,
		ReverseIteratorsCoverFullRange,
		ReverseIteratorsCoverReverseSequence,
		ConstIteratorsMatchNonConst,
		ConstReverseIteratorsMatchNonConst
	);
}