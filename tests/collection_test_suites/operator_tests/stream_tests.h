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
	using StreamTests = CollectionTest<T>;

	TYPED_TEST_SUITE_P(StreamTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the stream operators correctly write to, and read from back
	/// into the collection maintaining the collection size, i.e. does not
	/// lose elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(StreamTests, StreamOperatorsMaintainObjectContents) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		const collection_type obj1{ a, b, c };
		collection_type obj2{};

		ASSERT_NE(obj1, obj2);

		std::stringstream stream;
		stream << obj1;
		stream >> obj2;

		EXPECT_EQ(obj1, obj2);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		StreamTests,
		StreamOperatorsMaintainObjectContents
	);
}