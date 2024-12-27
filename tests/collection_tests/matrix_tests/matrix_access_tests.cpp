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

#include <gtest/gtest.h>

#include "math/Matrix.h"
#include "matrix_test_fixture.h"

namespace collection_tests {

	using namespace collections;

	class MatrixAccessTest : public MatrixTest {
	protected:
		using MatrixTest::grid_initializer;

		static inline grid_initializer input = {
			{1, 2, 3},
			{4, 5, 6},
			{7, 8, 9}
		};

		Matrix<int, 3, 3, true> row_major_matrix{ input };

		Matrix<int, 3, 3, false> column_major_matrix{ input };

		Vector<int, 5>  vector = { Fill({5, 4, 3, 2, 1}) };
	};

	TEST_F(MatrixAccessTest, IndexOperatorReturnsExpectedElements) {
		EXPECT_EQ((this->row_major_matrix[0, 0]), 1);
		EXPECT_EQ((this->row_major_matrix[0, 1]), 2);
		EXPECT_EQ((this->row_major_matrix[0, 2]), 3);
		EXPECT_EQ((this->row_major_matrix[1, 0]), 4);
		EXPECT_EQ((this->row_major_matrix[1, 1]), 5);
		EXPECT_EQ((this->row_major_matrix[1, 2]), 6);
		EXPECT_EQ((this->row_major_matrix[2, 0]), 7);
		EXPECT_EQ((this->row_major_matrix[2, 1]), 8);
		EXPECT_EQ((this->row_major_matrix[2, 2]), 9);

		EXPECT_EQ((this->column_major_matrix[0, 0]), 1);
		EXPECT_EQ((this->column_major_matrix[0, 1]), 2);
		EXPECT_EQ((this->column_major_matrix[0, 2]), 3);
		EXPECT_EQ((this->column_major_matrix[1, 0]), 4);
		EXPECT_EQ((this->column_major_matrix[1, 1]), 5);
		EXPECT_EQ((this->column_major_matrix[1, 2]), 6);
		EXPECT_EQ((this->column_major_matrix[2, 0]), 7);
		EXPECT_EQ((this->column_major_matrix[2, 1]), 8);
		EXPECT_EQ((this->column_major_matrix[2, 2]), 9);

		EXPECT_EQ(this->vector[0], 5);
		EXPECT_EQ(this->vector[1], 4);
		EXPECT_EQ(this->vector[2], 3);
		EXPECT_EQ(this->vector[3], 2);
		EXPECT_EQ(this->vector[4], 1);
	}

	TEST_F(MatrixAccessTest, SettingIndexChangesExpectedElement) {
		this->row_major_matrix[1, 2] = 26;
		this->row_major_matrix[0, 0] = 18;
		this->column_major_matrix[1, 2] = 26;
		this->column_major_matrix[0, 0] = 18;
		this->vector[1] = 7;

		auto expectedRowMajorSequence = { 18, 2, 3, 4, 5, 26, 7, 8, 9 };
		auto expectedColumnMajorSequence = { 18, 4, 7, 2, 5, 8, 3, 26, 9 };
		auto expectedVector = { 5, 7, 3, 2, 1 };

		this->expectSequence(this->row_major_matrix, expectedRowMajorSequence);
		this->expectSequence(this->column_major_matrix, expectedColumnMajorSequence);
		this->expectSequence(this->vector, expectedVector);
	}
}