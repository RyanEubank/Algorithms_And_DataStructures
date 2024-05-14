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

#include "../collection_test_fixture.h"
#include "algorithms/collection_algorithms.h"

namespace collection_tests {

	template<class params> 
		requires associative_collection<typename params::collection_t>
	class AssociativeCollectionTests : public CollectionTests<params> {
	protected:

		using collection = typename params::collection_t;
		using element = typename params::element_t; 
		using iterator = typename std::initializer_list<element>::iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given object and list of expected elements are the
		/// same. i.e. they are the same size and the obj contains each element
		/// in the list.
		/// </summary>
		/// 
		/// <param name="obj">
		/// The object under test.
		/// </param>
		/// 
		/// <param name="expected">
		/// The list of expected elements.
		/// </param> ----------------------------------------------------------
		void testObjectContainsExpectedElements(
			const auto& obj, 
			const auto& expected
		) {
			ASSERT_EQ(obj.size(), expected.size());

			for (const auto& e : expected)
				EXPECT_NE(collections::find(obj, e), obj.end());
		}
	};
}