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
	using AssociativeSearchTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(AssociativeSearchTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find method for the collection returns valid iterators
	/// for existing elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSearchTests, 
		FindReturnsCorrectIteratorForExistingElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, b, c };

		if constexpr (map<collection_type>) {
			EXPECT_NE(obj.find(a.key()), obj.end());
			EXPECT_NE(obj.find(b.key()), obj.end());
			EXPECT_NE(obj.find(c.key()), obj.end());
		}
		else {
			EXPECT_NE(obj.find(a), obj.end());
			EXPECT_NE(obj.find(b), obj.end());
			EXPECT_NE(obj.find(c), obj.end());
		}
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find method for the collection returns the end iterator
	/// for non existing elements (element was not found).
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSearchTests, 
		FindReturnsEndIteratorForNonExistingElement
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, b, c };

		if constexpr (map<collection_type>) {
			EXPECT_EQ(obj.find(d.key()), obj.end());
			EXPECT_EQ(obj.find(e.key()), obj.end());
			EXPECT_EQ(obj.find(f.key()), obj.end());
		}
		else {
			EXPECT_EQ(obj.find(d), obj.end());
			EXPECT_EQ(obj.find(e), obj.end());
			EXPECT_EQ(obj.find(f), obj.end());
		}
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the contains method for the collection returns true if the 
	/// searched element exists.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(AssociativeSearchTests, ContainsReturnsTrueForExistingItem) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, b, c };

		if constexpr (map<collection_type>) 
			EXPECT_TRUE(obj.contains(b.key()));
		else
			EXPECT_TRUE(obj.contains(b));
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the contains method for the collection returns false for
	/// elements that do not exist.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeSearchTests, 
		ContainsReturnsFalseForNonExistingItem
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj{ a, b, b, c };

		if constexpr (map<collection_type>)
			EXPECT_FALSE(obj.contains(d.key()));
		else
			EXPECT_FALSE(obj.contains(d));
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeSearchTests,
		FindReturnsCorrectIteratorForExistingElement,
		FindReturnsEndIteratorForNonExistingElement,
		ContainsReturnsTrueForExistingItem,
		ContainsReturnsFalseForNonExistingItem
	);
}