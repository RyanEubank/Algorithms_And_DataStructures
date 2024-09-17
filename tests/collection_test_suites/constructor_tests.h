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

#include "concepts/associative.h"
#include "concepts/sequential.h"

#include "collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using ConstructorTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(ConstructorTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that a default constructed collection is initially empty.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(ConstructorTests, DefaultConstructorCreatesEmptyObject) {
		FORWARD_TEST_TYPES();

		const collection_type obj{};

		EXPECT_TRUE(obj.isEmpty());
		EXPECT_EQ(obj.size(), 0);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that a collection constructed with an initialization list
	/// correctly sets the contents.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(ConstructorTests, InitializationConstructorSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const auto expected = { a, b, c };
		const collection_type obj{ a, b, c };

		EXPECT_EQ(obj.size(), 3);

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.begin(), obj.end(), expected);
		else if constexpr (associative<collection_type>)
			this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from an iterator
	/// pair.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(ConstructorTests, IteratorConstructorSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const auto expected = { a, b, c };
		const collection_type obj(expected.begin(), expected.end());

		EXPECT_EQ(obj.size(), 3);

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.begin(), obj.end(), expected);
		else if constexpr (associative<collection_type>)
			this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly from a range.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(ConstructorTests, RangeConstructorSetsContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const auto expected = { a, b, c };
		const collection_type obj(collections::from_range, expected);

		EXPECT_EQ(obj.size(), 3);

		if constexpr (sequential<collection_type>)
			this->expectSequence(obj.begin(), obj.end(), expected);
		else if constexpr (associative<collection_type>)
			this->expectSet(obj.begin(), obj.end(), expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the collection can be constructed correctly with duplicate
	/// elements in initialization.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(ConstructorTests, ConstructorsAcceptsDuplicateElements) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto expected = { a, a, b, b, c, d, e };
		const collection_type obj(collections::from_range, expected);

		if constexpr (sequential<collection_type>) {
			EXPECT_EQ(obj.size(), 7);
			this->expectSequence(obj.begin(), obj.end(), expected);
		}
		else if constexpr (associative<collection_type>) {

			auto set = { a, b, c, d, e };
			count_list counts = { {a, 2}, {b, 2}, {c, 1}, {d, 1}, {e, 1} };

			size_type size = collection_type::allow_duplicates 
				? expected.size() : set.size();

			EXPECT_EQ(obj.size(), size);

			if constexpr (collection_type::allow_duplicates)
				this->expectCount(obj, counts);
			else
				this->expectSet(obj.begin(), obj.end(), set);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ConstructorTests,
		DefaultConstructorCreatesEmptyObject,
		InitializationConstructorSetsContents,
		IteratorConstructorSetsContents,
		RangeConstructorSetsContents,
		ConstructorsAcceptsDuplicateElements
	);
}