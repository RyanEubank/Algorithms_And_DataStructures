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

#include "collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class OrderedCollectionTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(OrderedCollectionTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns end() in an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(OrderedCollectionTests, LowerBoundReturnsEndOnEmpty) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ };
		auto bound = obj.lowerBound(a);
		EXPECT_EQ(bound, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns an iterator to equivalent element when
	/// they exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		OrderedCollectionTests, 
		LowerBoundReturnsIteratorToEqualElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ d, c, a, e, f };
		auto bound = obj.lowerBound(a);
		EXPECT_EQ(*bound, a);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns an iterator to greater element when
	/// an equal one does not exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		OrderedCollectionTests, 
		LowerBoundReturnsIteratorToGreaterElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ d, c, a, e, f };
		auto bound = obj.lowerBound(b);
		EXPECT_EQ(*bound, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns the end() iterator when all elements
	/// in the collection are < the requested bound.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		OrderedCollectionTests, 
		LowerBoundReturnsEndIteratorWhenBoundIsGreater
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ d, c, a, e, b };
		auto bound = obj.lowerBound(f);
		EXPECT_EQ(bound, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that upperBoundBound returns end() in an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(OrderedCollectionTests, UpperBoundReturnsEndOnEmpty) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ };
		auto bound = obj.upperBound(a);
		EXPECT_EQ(bound, obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that upperBound returns an iterator to the first element > than
	/// the bound even when an equal element exists.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		OrderedCollectionTests, 
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentExists
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ d, c, a, e, f, b };
		auto bound = obj.upperBound(b);
		EXPECT_EQ(*bound, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that upperBound returns an iterator to the first element > than
	/// the bound when an equivalent does not exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		OrderedCollectionTests, 
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentDoesNotExist
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ d, c, a, e, f };
		auto bound = obj.upperBound(b);
		EXPECT_EQ(*bound, c);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that lowerBound returns the end() iterator when all elements
	/// in the collection are < the requested bound.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		OrderedCollectionTests, 
		UpperBoundReturnsEndIteratorWhenBoundIsGreater
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ d, c, a, e, b };
		auto bound = obj.upperBound(f);
		EXPECT_EQ(bound, obj.end());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		OrderedCollectionTests,
		LowerBoundReturnsEndOnEmpty,
		LowerBoundReturnsIteratorToEqualElement,
		LowerBoundReturnsIteratorToGreaterElement,
		LowerBoundReturnsEndIteratorWhenBoundIsGreater,
		UpperBoundReturnsEndOnEmpty,
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentExists,
		UpperBoundReturnsIteratorToGreaterElementWhenEquivalentDoesNotExist,
		UpperBoundReturnsEndIteratorWhenBoundIsGreater
	);
}