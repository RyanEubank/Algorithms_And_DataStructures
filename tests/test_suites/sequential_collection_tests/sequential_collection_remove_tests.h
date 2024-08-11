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
	class SequentialCollectionRemoveTests :public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(SequentialCollectionRemoveTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeFront method removes the first element in the 
	/// collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests, 
		RemoveFrontDeletesFirstElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.removeFront(); };
		this->testMethodRemovesFirstElement(method);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeBack method removes the last element in the 
	/// collection.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(
		SequentialCollectionRemoveTests, 
		RemoveBackDeletesLastElementInTheSequence
	) {
		auto method = [](auto& obj) { obj.removeBack(); };
		this->testMethodRemovesLastElement(method);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		SequentialCollectionRemoveTests,
		RemoveFrontDeletesFirstElementInTheSequence,
		RemoveBackDeletesLastElementInTheSequence
	);
}