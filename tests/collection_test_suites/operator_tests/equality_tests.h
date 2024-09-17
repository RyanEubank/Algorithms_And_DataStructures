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
#include "../collection_test_fixture.h"

namespace collection_tests {

	template <class T>
	using EqualityTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(EqualityTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the equality operator returns equal for identical objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(EqualityTests, IdenticalObjectsAreEqual) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, c };

		EXPECT_TRUE(obj1 == obj2);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the equality operator returns not equal for strictly
	/// dissimilar objects.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(EqualityTests, ObjectsWithNoOverlappingElementsAreUnequal) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ d, e, f };

		EXPECT_TRUE(obj1 != obj2);
	}
	
	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the equality operator returns not equal for objects of 
	/// different size.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(EqualityTests, ObjectsWithUnequalSizeAreUnqual) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b };

		EXPECT_TRUE(obj1 != obj2);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the equality operator returns not equal for objects with
	/// some but not all elements in common. 
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		EqualityTests, 
		ObjectsWithOnlySomeEquivalentElementsAreUnequal
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ a, b, e };

		EXPECT_TRUE(obj1 != obj2);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the equality operator returns not equal for sequential
	/// objects with different ordering or equal for associative objects with
	/// different ordering.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		EqualityTests, 
		ObjectsWithDifferentOrderingAreEqualBasedOnOrderingRequirements
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		const collection_type obj2{ c, a, b };

		if constexpr (sequential<collection_type>) 
			EXPECT_TRUE(obj1 != obj2);
		else if constexpr (associative<collection_type>) 
			EXPECT_TRUE(obj1 == obj2);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		EqualityTests,
		IdenticalObjectsAreEqual,
		ObjectsWithNoOverlappingElementsAreUnequal,
		ObjectsWithUnequalSizeAreUnqual,
		ObjectsWithOnlySomeEquivalentElementsAreUnequal,
		ObjectsWithDifferentOrderingAreEqualBasedOnOrderingRequirements
	);
}