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

namespace collection_tests {

	using namespace collections;

	class MatrixTest : public testing::Test {
	protected:

		using grid_initializer 
			= std::initializer_list<std::initializer_list<int>>;

		testing::AssertionResult expectSequence(
			const auto& matrix,
			const auto& expected
		) const {
			EXPECT_EQ(matrix.size(), expected.size());

			auto begin = expected.begin();
			for (const auto& scalar : matrix)
				EXPECT_EQ(scalar, *begin++);

			EXPECT_EQ(begin, expected.end());
			return testing::AssertionSuccess();
		}

		testing::AssertionResult expectFilledBy(
			const auto& matrix,
			const auto& value
		) const {
			for (auto& scalar : matrix)
				EXPECT_EQ(scalar, value);
			return testing::AssertionSuccess();
		}
	};
}