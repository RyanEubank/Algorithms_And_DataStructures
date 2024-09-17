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
	using AssociativeBoundTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(AssociativeBoundTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns end() in an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeBoundTests, LowerBoundReturnsEndOnEmpty) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.lowerBound(a.key());
		else
			bound = obj.lowerBound(a);

		EXPECT_EQ(bound, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns an iterator to equivalent element when
	/// they exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeBoundTests, 
		LowerBoundReturnsIteratorToEqualElementWhenItExists
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ d, c, a, a, a, e, f };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.lowerBound(a.key());
		else
			bound = obj.lowerBound(a);

		EXPECT_EQ(*bound, a);

		if constexpr (collection_type::allow_duplicates) {
			auto upper = obj.end();

			if constexpr (map<collection_type>)
				upper = obj.upperBound(a.key());
			else
				upper = obj.upperBound(a);

			auto dist = std::distance(bound, upper);
			EXPECT_EQ(dist, 3);
		}
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns an iterator to greater element when
	/// an equal one does not exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeBoundTests, 
		LowerBoundReturnsIteratorToGreaterElementWhenItDoesNotExist
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ d, c, a, a, e, f };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.lowerBound(b.key());
		else
			bound = obj.lowerBound(b);

		EXPECT_EQ(*bound, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns the end() iterator when all elements
	/// in the collection are < the requested bound.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeBoundTests, LowerBoundReturnsEndIteratorWhenBoundIsGreater) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ d, c, a, a, e, b, b };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.lowerBound(f.key());
		else
			bound = obj.lowerBound(f);

		EXPECT_EQ(bound, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that upperBoundBound returns end() in an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeBoundTests, UpperBoundReturnsEndOnEmpty) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.upperBound(a.key());
		else
			bound = obj.upperBound(a);

		EXPECT_EQ(bound, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that upperBound returns an iterator to the first element > than
	/// the bound even when an equal element exists.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeBoundTests, 
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentExists
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ d, c, a, e, f, b, b };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.upperBound(b.key());
		else
			bound = obj.upperBound(b);

		EXPECT_EQ(*bound, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that upperBound returns an iterator to the first element > than
	/// the bound when an equivalent does not exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeBoundTests, 
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentDoesNotExist
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ d, c, a, a, e, f };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.upperBound(b.key());
		else
			bound = obj.upperBound(b);

		EXPECT_EQ(*bound, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns the end() iterator when all elements
	/// in the collection are < the requested bound.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeBoundTests, UpperBoundReturnsEndIteratorWhenBoundIsGreater) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ d, c, a, a, e, b, b };
		auto bound = obj.end();

		if constexpr (map<collection_type>)
			bound = obj.upperBound(f.key());
		else
			bound = obj.upperBound(f);

		EXPECT_EQ(bound, obj.end());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeBoundTests,
		LowerBoundReturnsEndOnEmpty,
		LowerBoundReturnsIteratorToEqualElementWhenItExists,
		LowerBoundReturnsIteratorToGreaterElementWhenItDoesNotExist,
		LowerBoundReturnsEndIteratorWhenBoundIsGreater,
		UpperBoundReturnsEndOnEmpty,
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentExists,
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentDoesNotExist,
		UpperBoundReturnsEndIteratorWhenBoundIsGreater
	);
}