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
	using ForwardIteratorTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(ForwardIteratorTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that iterators can be copied and all copies dereferenced.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(ForwardIteratorTests, IteratorsCanBeCopied) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e, f };
		auto first = obj.begin();
		auto second = first;
		
		EXPECT_EQ(first, second);
		EXPECT_EQ(*first, *second);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that iterators can be copied and dereferenced multiple times after 
	/// being iterated through.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		ForwardIteratorTests, 
		IteratorsCanBeDereferencedMutipleTimes
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, c, d, e, f };
		auto first = obj.begin();
		auto second = first;
		auto last = second;

		while (second != obj.end()) {
			last = second++;
		}

		EXPECT_NE(first, second);
		EXPECT_NE(*first, *last);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ForwardIteratorTests,
		IteratorsCanBeCopied,
		IteratorsCanBeDereferencedMutipleTimes
	);
}