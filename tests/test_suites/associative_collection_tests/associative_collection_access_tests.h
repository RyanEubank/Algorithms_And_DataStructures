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

#include "associative_collection_test_fixture.h"

namespace collection_tests {

	template <class params>
	class AssociativeCollectionAccessTests : 
		public AssociativeCollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeCollectionAccessTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find method for the collection returns valid iterators
	/// for existing elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionAccessTests, 
		FindReturnsCorrectIteratorForExistingElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		EXPECT_NE(obj.find(a), obj.end());
		EXPECT_NE(obj.find(b), obj.end());
		EXPECT_NE(obj.find(c), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find method for the collection returns the end iterator
	/// for non existing elements (element was not found).
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionAccessTests, 
		FindReturnsEndIteratorForNonExistingElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		EXPECT_EQ(obj.find(d), obj.end());
		EXPECT_EQ(obj.find(e), obj.end());
		EXPECT_EQ(obj.find(f), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find_if method for the collection returns valid 
	/// iterators for elements that match the passed predicate.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionAccessTests, 
		ContainsReturnsTrueForExistingItem
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		EXPECT_TRUE(obj.contains(b));
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find_if method for the collection returns the end 
	/// iterator when there are no elements that match the predicate.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionAccessTests, 
		ContainsReturnsFalseForNonExistingItem
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection obj{ a, b, c };

		EXPECT_FALSE(obj.contains(d));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeCollectionAccessTests,
		FindReturnsCorrectIteratorForExistingElement,
		FindReturnsEndIteratorForNonExistingElement,
		ContainsReturnsTrueForExistingItem,
		ContainsReturnsFalseForNonExistingItem
	);
}