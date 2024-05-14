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

#include "sequential_index_test_fixture.h"

namespace collection_tests {

	template <class params> 
	class SequentialIndexAccessTests : public SequentialIndexTests<params> {};

	TYPED_TEST_SUITE_P(SequentialIndexAccessTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the index operator returns the expected element at the 
	/// correct index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialIndexAccessTests, 
		IndexOperatorReturnsCorrectElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		EXPECT_EQ(obj[0], a);
		EXPECT_EQ(obj[1], b);
		EXPECT_EQ(obj[2], c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the at() method returns the expected element at the correct
	/// index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIndexAccessTests, AtMethodReturnsCorrectElement) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		EXPECT_EQ(obj.at(0), a);
		EXPECT_EQ(obj.at(1), b);
		EXPECT_EQ(obj.at(2), c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the at method throws an exception when an out of bounds
	/// index is passed.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialIndexAccessTests, AtMethodPerformsBoundsChecking) {
		auto method = [](auto& obj, Index& index) {
			auto e = obj.at(index.get());
		};
		auto safeIndex = [](auto& obj) { return obj.size() - 1; };
		auto unsafeIndex = [](auto& obj) { return obj.size(); };

		this->testMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialIndexAccessTests,
		IndexOperatorReturnsCorrectElement,
		AtMethodReturnsCorrectElement,
		AtMethodPerformsBoundsChecking
	);
}