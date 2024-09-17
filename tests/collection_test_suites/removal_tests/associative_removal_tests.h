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

#include <iterator>
#include <gtest/gtest.h>

#include "../collection_test_fixture.h"

namespace collection_tests {

	// TODO - implement keyed removal and tests

	template <class T>
	using AssociativeRemovalTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(AssociativeRemovalTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the remove method will remove the correct element from
	/// associative collections.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeRemovalTests, 
		RemoveAtIteratorDeletesCorrectElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, b, c};

		auto expected_size = collection_type::allow_duplicates ? 3 : 2;
		auto pos = std::find(obj.begin(), obj.end(), b);
		obj.remove(pos);

		EXPECT_EQ(obj.size(), expected_size);

		if constexpr (collection_type::allow_duplicates) {
			auto expected = { a, b, c };
			this->expectSet(obj.begin(), obj.end(), expected);
		}
		else {
			auto expected = { a, c };
			this->expectSet(obj.begin(), obj.end(), expected);
		}
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method returns an iterator to the
	/// element following the one that was deleted.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeRemovalTests,
		RemoveAtIteratorReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, b, c };

		auto expected = *std::next(obj.begin(), 2);
		auto result = obj.remove(std::next(obj.begin()));

		EXPECT_EQ(*result, expected);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the remove method has no effect when called to remove a range
	/// of elements from an empty collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeRemovalTests, 
		RemoveAllBetweenIteratorsOnEmptyCollectionHasNoEffect
	) {
		auto method = [](auto& obj) { obj.remove(obj.begin(), obj.end()); };
		this->expectMethodHasNoEffectOnEmptyCollection(method);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the remove method has no effect when called to remove an
	/// empty range (begin == end) from a non-empty collection.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeRemovalTests, 
		RemoveAllBetweenIteratorsHasNoEffectWhenCalledWithEmptyRange
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, b, c };
		auto method = [&]() { obj.remove(obj.end(), obj.end()); };
		auto expected_size = collection_type::allow_duplicates ? 4 : 3;

		EXPECT_NO_THROW(method());
		EXPECT_EQ(obj.size(), expected_size);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that theremove method correctly removes elements within the given 
	/// range.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeRemovalTests,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, b, c, d, e, f};

		auto begin = std::next(obj.begin());
		auto end = std::next(obj.begin(), 4);

		if constexpr (collection_type::allow_duplicates) {
			auto expected_size = 4;
			auto expected_contents = { 
				*obj.begin(), 
				*std::next(obj.begin(), 4),
				*std::next(obj.begin(), 5),
				*std::next(obj.begin(), 6)
			};

			obj.remove(begin, end);

			EXPECT_EQ(obj.size(), expected_size);
			this->expectSet(obj.begin(), obj.end(), expected_contents);
		}
		else {
			auto expected_size = 3;
			auto expected_contents = {
				*obj.begin(),
				*std::next(obj.begin(), 4),
				*std::next(obj.begin(), 5)
			};

			obj.remove(begin, end);

			EXPECT_EQ(obj.size(), expected_size);
			this->expectSet(obj.begin(), obj.end(), expected_contents);
		}
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method returns an iterator to the
	/// element following the last one that was deleted.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeRemovalTests,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{ a, b, b, c, d, e, f };

		auto expected = *std::next(obj.begin(), 3);
		auto begin = std::next(obj.begin());
		auto end = std::next(obj.begin(), 3);
		auto result = obj.remove(begin, end);

		EXPECT_EQ(*result, expected);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeRemovalTests,
		RemoveAtIteratorDeletesCorrectElement,
		RemoveAtIteratorReturnsIteratorToNextPosition,
		RemoveAllBetweenIteratorsOnEmptyCollectionHasNoEffect,
		RemoveAllBetweenIteratorsHasNoEffectWhenCalledWithEmptyRange,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements,
		RemoveAllBetweenIteratorsReturnsIteratorToNextPosition
	);
}