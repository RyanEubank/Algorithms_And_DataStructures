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
	class AssociativeCollectionAccessTests : public CollectionTests<params> {};

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
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);

		EXPECT_NE(obj.find(input[0]), obj.end());
		EXPECT_NE(obj.find(input[1]), obj.end());
		EXPECT_NE(obj.find(input[2]), obj.end());
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
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		auto nonexisting_elements = this->_test_data.different_elements();
		const collection obj(collections::from_range, input);

		EXPECT_EQ(obj.find(nonexisting_elements[0]), obj.end());
		EXPECT_EQ(obj.find(nonexisting_elements[1]), obj.end());
		EXPECT_EQ(obj.find(nonexisting_elements[2]), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find_if method for the collection returns valid 
	/// iterators for elements that match the passed predicate.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionAccessTests, 
		FindIfReturnsMatch
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);
		auto predicate = [&](auto& e) { return e == input[0]; };

		EXPECT_NE(obj.find_if(predicate), obj.end());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the find_if method for the collection returns the end 
	/// iterator when there are no elements that match the predicate.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		AssociativeCollectionAccessTests, 
		FindIfReturnEndIteratorForNoMatch
	) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		auto nonexisting_elements = this->_test_data.different_elements();
		const collection obj(collections::from_range, input);
		auto predicate = [&](auto& e) { return e == nonexisting_elements[0]; };

		EXPECT_EQ(obj.find_if(predicate), obj.end());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		AssociativeCollectionAccessTests,
		FindReturnsCorrectIteratorForExistingElement,
		FindReturnsEndIteratorForNonExistingElement,
		FindIfReturnsMatch,
		FindIfReturnEndIteratorForNoMatch
	);
}