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
	using SequentialAccessTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(SequentialAccessTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that calling front on a sequence returns the first element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialAccessTests, 
		FrontReturnsFirstInSequence
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();
		const collection_type obj{ a, b, c };
		EXPECT_EQ(obj.front(), a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that calling back on a sequence returns the last element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialAccessTests, 
		BackReturnsLastInSequence
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();
		const collection_type obj{ a, b, c };
		EXPECT_EQ(obj.back(), c);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialAccessTests,
		FrontReturnsFirstInSequence,
		BackReturnsLastInSequence
	);
}