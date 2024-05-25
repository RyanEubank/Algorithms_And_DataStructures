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

#include "../sequential_collection_test_fixture.h"

namespace collection_tests {

	template <class params> 
	class SequentialIndexTests : public SequentialCollectionTests<params> {
	protected:

		using collection = typename params::collection_t;
		using element = typename params::element_t;

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method checks the bounds of the index it is
		/// given and throws appropriate exceptions.
		/// </summary> 
		/// 
		/// <param name="safeCall">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodChecksIndexBounds(
			std::function<void(collection&, Index&)> func,
			std::function<size_t(collection&)> safeIndex,
			std::function<size_t(collection&)> unsafeIndex
		) {
			DECLARE_TEST_DATA();

			collection obj1{ a, b, c };
			collection obj2{ a, b, c };

			auto safe = Index(safeIndex(obj1));
			auto unsafe = Index(unsafeIndex(obj2));

			EXPECT_NO_THROW(func(obj1, safe));
			EXPECT_THROW(func(obj2, unsafe), std::out_of_range);
		}
	};
}