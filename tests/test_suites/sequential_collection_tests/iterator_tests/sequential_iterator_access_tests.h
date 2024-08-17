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

#include "../../collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class SequentialIteratorAccessTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIteratorAccessTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIteratorAccessTests, IteratorsCoverFullRange) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto it = obj.begin();

		EXPECT_EQ(*it++, a);
		EXPECT_EQ(*it++, b);
		EXPECT_EQ(*it++, c);
		EXPECT_EQ(it, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the const_iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIteratorAccessTests, ConstIteratorsCoverFullRange) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto it = obj.cbegin();

		EXPECT_EQ(*it++, a);
		EXPECT_EQ(*it++, b);
		EXPECT_EQ(*it++, c);
		EXPECT_EQ(it, obj.cend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reverse_iterator pair in the range covers the full 
	/// range in reverse order.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorAccessTests,
		ReverseIteratorsCoverFullReverseRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		if constexpr (!bidirectional_collection<collection>)
			GTEST_SKIP();
		else {
			const collection obj{ a, b, c };
			auto it = obj.rbegin();

			EXPECT_EQ(*it++, c);
			EXPECT_EQ(*it++, b);
			EXPECT_EQ(*it++, a);
			EXPECT_EQ(it, obj.rend());
		}
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIteratorAccessTests, ConstIteratorsMatchNonConst) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto it = obj.begin();
		auto c_it = obj.cbegin();

		while (it != obj.end()) 
			EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(obj.end(), obj.cend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIteratorAccessTests, 
		ConstReverseIteratorsMatchNonConst
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		if constexpr (!bidirectional_collection<collection>)
			GTEST_SKIP();
		else {
			const collection obj{ a, b, c };
			auto it = obj.rbegin();
			auto c_it = obj.crbegin();

			while (it != obj.rend())
				EXPECT_EQ(*it++, *c_it++);
			EXPECT_EQ(obj.rend(), obj.crend());
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIteratorAccessTests,
		IteratorsCoverFullRange,
		ConstIteratorsCoverFullRange,
		ReverseIteratorsCoverFullReverseRange,
		ConstIteratorsMatchNonConst,
		ConstReverseIteratorsMatchNonConst
	);
}