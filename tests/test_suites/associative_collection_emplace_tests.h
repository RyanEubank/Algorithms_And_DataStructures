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
	class AssociativeCollectionEmplaceTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(AssociativeCollectionEmplaceTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplace method will correctly insert on empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionEmplaceTests, 
		EmplacePlacesElementInEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto value = this->testInput.control()[0];
		collection obj{};

		ASSERT_TRUE(obj.isEmpty());
		obj.emplace(value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_TRUE(obj.size() == 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will correctly insert values on 
	/// empty collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionEmplaceTests, 
		EmplaceWithLocationHintPlacesElementInEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto value = this->testInput.control()[0];
		collection obj{};

		ASSERT_TRUE(obj.isEmpty());
		obj.emplace(obj.begin(), value);
		EXPECT_FALSE(obj.isEmpty());
		EXPECT_TRUE(obj.size() == 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplace method will correctly insert on non-empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionEmplaceTests, 
		EmplacePlacesElementInNonEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];
		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		obj.emplace(value);
		EXPECT_TRUE(obj.size() == input.size() + 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hinted emplace method will correctly insert on non-empty 
	/// collections.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionEmplaceTests, 
		EmplaceWithLocationHintPlacesElementInNonEmptyCollection
	) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];
		collection obj(collections::from_range, input);

		ASSERT_FALSE(obj.isEmpty());
		obj.emplace(obj.begin(), value);
		EXPECT_TRUE(obj.size() == input.size() + 1);
		EXPECT_NE(collections::find(obj, value), obj.end());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeCollectionEmplaceTests,
		EmplacePlacesElementInEmptyCollection,
		EmplaceWithLocationHintPlacesElementInEmptyCollection,
		EmplacePlacesElementInNonEmptyCollection,
		EmplaceWithLocationHintPlacesElementInNonEmptyCollection
	);
}