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

#include "collection_tests.h"

namespace collection_tests {

	template <class params>
	class AssociativeCollectionInsertTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeCollectionInsertTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert on empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertPlacesElementInEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto value = this->testInput.control()[0];
		collection obj{};

		ASSERT_TRUE(obj.isEmpty());
		obj.insert(value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_TRUE(obj.size() == 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert values on 
	/// empty collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertWithLocationHintPlacesElementInEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto value = this->testInput.control()[0];
		collection obj{};

		ASSERT_TRUE(obj.isEmpty());
		obj.insert(obj.begin(), value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_TRUE(obj.size() == 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert on non-empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertPlacesElementInNonEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];
		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		obj.insert(value);
		EXPECT_TRUE(obj.size() == input.size() + 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert on non-empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertWithLocationHintPlacesElementInNonEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];
		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		obj.insert(obj.begin(), value);
		EXPECT_TRUE(obj.size() == input.size() + 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert a given range of
	/// elements into an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertPlacesRangeInEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		collection obj{};

		ASSERT_TRUE(obj.isEmpty());
		obj.insert(input.begin(), input.end());
		EXPECT_TRUE(obj.size() == input.size());
		EXPECT_NE(collections::find(obj, input[0]), obj.end());
		EXPECT_NE(collections::find(obj, input[1]), obj.end());
		EXPECT_NE(collections::find(obj, input[2]), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert a given range 
	/// of elements into an empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertWithLocationHintPlacesRangeInEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		collection obj{};

		ASSERT_TRUE(obj.isEmpty());
		obj.insert(obj.begin(), input.begin(), input.end());
		EXPECT_TRUE(obj.size() == input.size());
		EXPECT_NE(collections::find(obj, input[0]), obj.end());
		EXPECT_NE(collections::find(obj, input[1]), obj.end());
		EXPECT_NE(collections::find(obj, input[2]), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insert method will correctly insert a given range of
	/// elements into a non-empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertPlacesRangeInNonEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto new_elements = this->testInput.different_elements();
		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		obj.insert(new_elements.begin(), new_elements.end());
		EXPECT_TRUE(obj.size() == (input.size() + new_elements.size()));
		EXPECT_NE(collections::find(obj, new_elements[0]), obj.end());
		EXPECT_NE(collections::find(obj, new_elements[1]), obj.end());
		EXPECT_NE(collections::find(obj, new_elements[2]), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted insert method will correctly insert a given range 
	/// of elements into a non-empty collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionInsertTests, 
		InsertWithLocationHintPlacesRangeInNonEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto new_elements = this->testInput.different_elements();
		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		obj.insert(obj.begin(), new_elements.begin(), new_elements.end());
		EXPECT_TRUE(obj.size() == (input.size() + new_elements.size()));
		EXPECT_NE(collections::find(obj, new_elements[0]), obj.end());
		EXPECT_NE(collections::find(obj, new_elements[1]), obj.end());
		EXPECT_NE(collections::find(obj, new_elements[2]), obj.end());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeCollectionInsertTests,
		InsertPlacesElementInEmptyCollection,
		InsertWithLocationHintPlacesElementInEmptyCollection,
		InsertPlacesElementInNonEmptyCollection,
		InsertWithLocationHintPlacesElementInNonEmptyCollection,
		InsertPlacesRangeInEmptyCollection,
		InsertWithLocationHintPlacesRangeInEmptyCollection,
		InsertPlacesRangeInNonEmptyCollection,
		InsertWithLocationHintPlacesRangeInNonEmptyCollection
	);
}