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

#include "../../collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class AssociativeIteratorAccessTests : public CollectionTests<params> {
	protected:

		void testIteratorCoversFullRange(auto begin, auto end, auto& range) {
			while (begin != end)
				EXPECT_NE(collections::find(range, *begin++), range.end());
		}

		void testIteratorPairsCoverSameRange(
			auto actual,
			auto actual_end,
			auto expected,
			auto expected_end
		) {
			while (actual != actual_end && expected != expected_end)
				EXPECT_EQ(*actual++, *expected++);

			EXPECT_EQ(actual, actual_end);
			EXPECT_EQ(expected, expected_end);
		}
	};

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

		this->testIteratorCoversFullRange(obj.begin(), obj.end(), input);
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

		this->testIteratorCoversFullRange(obj.cbegin(), obj.cend(), input);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reverse_iterator pair in the range covers the full 
	/// range in reverse order.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeIteratorAccessTests, ReverseIteratorsCoverFullRange) {
		FORWARD_TEST_TYPES();

		if constexpr (!bidirectional_collection<collection>)
			GTEST_SKIP();

		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto input = { a, b, c };

		this->testIteratorCoversFullRange(obj.rbegin(), obj.rend(), input);
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

		if constexpr (!bidirectional_collection<collection>)
			GTEST_SKIP();

		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };
		auto list = std::list(obj.begin(), obj.end());

		this->testIteratorPairsCoverSameRange(
			obj.rbegin(), obj.rend(), list.rbegin(), list.rend()
		);
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

		this->testIteratorPairsCoverSameRange(
			obj.cbegin(), obj.cend(), obj.begin(), obj.end()
		);
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

		if constexpr (!bidirectional_collection<collection>)
			GTEST_SKIP();

		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		this->testIteratorPairsCoverSameRange(
			obj.crbegin(), obj.crend(), obj.rbegin(), obj.rend()
		);
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