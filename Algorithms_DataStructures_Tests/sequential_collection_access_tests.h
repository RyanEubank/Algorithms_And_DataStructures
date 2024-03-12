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

#include "sequential_collection_tests.h"

namespace collection_tests {

	template <class params>
	class SequentialCollectionAccessTests : 
		public SequentialCollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialCollectionAccessTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(SequentialCollectionAccessTests, IteratorsCoverFullRange) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		const collection obj(collections::from_range, input);

		size_t i = 0;
		for (const auto& e : obj)
			EXPECT_EQ(e, input[i++]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the index operator returns the expected element at the 
	/// correct index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionAccessTests, 
		IndexOperatorReturnsCorrectElement
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		const collection obj(collections::from_range, input);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the at() method returns the expected element at the correct
	/// index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionAccessTests, 
		AtMethodReturnsCorrectElement
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		const collection obj(collections::from_range, input);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj.at(i), input[i]);
	}


	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the at method throws an exception when an out of bounds
	/// index is passed.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionAccessTests, 
		AtMethodPerformsBoundsChecking
	) {
		auto method = [](auto& obj, Index& index) {
			auto e = obj.at(index.get());
		};
		auto safeIndex = [](auto& obj) { return obj.size() - 1; };
		auto unsafeIndex = [](auto& obj) { return obj.size(); };
		this->testMethodChecksIndexBounds(method, safeIndex, unsafeIndex);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that calling front on a sequence returns the first element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionAccessTests, 
		FrontReturnsFirstInSequence
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		const collection obj(collections::from_range, input);
		EXPECT_EQ(obj.front(), input[0]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that calling back on a sequence returns the last element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionAccessTests, 
		BackReturnsLastInSequence
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		const collection obj(collections::from_range, input);
		EXPECT_EQ(obj.back(), input[input.size() - 1]);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialCollectionAccessTests,
		IteratorsCoverFullRange,
		IndexOperatorReturnsCorrectElement,
		AtMethodReturnsCorrectElement,
		AtMethodPerformsBoundsChecking,
		FrontReturnsFirstInSequence,
		BackReturnsLastInSequence
	);
}