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

namespace collection_tests {

	template <class params>
	class SequentialCollectionEmplaceTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialCollectionEmplaceTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceFront method inserts the given element at the
	/// first position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests, 
		EmplaceFrontPlacesElementFirstInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.emplaceFront(value); };
		this->testMethodPlacesElementFirst(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the emplaceBack method inserts the given element at the 
	/// last position.
	/// </summary> -----------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionEmplaceTests, 
		EmplaceBackPlacesElementLastInTheSequence
	) {
		auto method = [](auto& obj, auto& value) { obj.emplaceBack(value); };
		this->testMethodPlacesElementLast(method);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialCollectionEmplaceTests,
		EmplaceFrontPlacesElementFirstInTheSequence,
		EmplaceBackPlacesElementLastInTheSequence
	);
}