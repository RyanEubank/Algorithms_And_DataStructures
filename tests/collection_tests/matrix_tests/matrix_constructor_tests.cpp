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

	class MatrixConstructorTest : public MatrixTest {
	protected:

		static constexpr auto columns = 3;
		static constexpr auto rows = 4;
	};

	class DefaultMatrixTest : public MatrixConstructorTest {
	protected:
		using MatrixConstructorTest::columns;
		using MatrixConstructorTest::rows;

		DynamicMatrix<int>				fully_dynamic_matrix;
		Matrix<int, DYNAMIC, columns>	dynamic_rows_matrix;
		Matrix<int, rows, DYNAMIC>		dynamic_columns_matrix;
		DynamicVector<int>				dynamic_vector;
		DynamicRowVector<int>			dynamic_row_vector;
		Matrix<int, rows, columns>		static_matrix;
		Vector<int, rows>				static_vector;
		RowVector<int, columns>			static_row_vector;
	};

	class DimensionInitializedMatrixTest : public MatrixConstructorTest {
	protected:
		using MatrixConstructorTest::columns;
		using MatrixConstructorTest::rows;

		DynamicMatrix<int>				fully_dynamic_matrix{ Rows(rows), Columns(columns) };
		Matrix<int, DYNAMIC, columns>	dynamic_rows_matrix{ Rows(rows)};
		Matrix<int, rows, DYNAMIC>		dynamic_columns_matrix{ Columns(columns) };
		DynamicVector<int>				dynamic_vector{ Rows(rows) };
		DynamicRowVector<int>			dynamic_row_vector{ Columns(columns) };
	};

	class SingleFillInitializedMatrixTest : public MatrixConstructorTest {
	protected:
		int value = 7;

		using MatrixConstructorTest::columns;
		using MatrixConstructorTest::rows;

		DynamicMatrix<int>				fully_dynamic_matrix{ Rows(rows), Columns(columns), Fill({value}) };
		Matrix<int, DYNAMIC, columns>	dynamic_rows_matrix{ Rows(rows), Fill({value}) };
		Matrix<int, rows, DYNAMIC>		dynamic_columns_matrix{ Columns(columns), Fill({value}) };
		DynamicVector<int>				dynamic_vector{ Rows(rows), Fill({value}) };
		DynamicRowVector<int>			dynamic_row_vector{ Columns(columns), Fill({value}) };
		Matrix<int, rows, columns>		static_matrix{ Fill({value}) };
		Vector<int, rows>				static_vector{ Fill({value}) };
		RowVector<int, columns>			static_row_vector{ Fill({value}) };
	};

	class FillListInitializedMatrixTest : public MatrixConstructorTest {
	protected:
		using MatrixConstructorTest::columns;
		using MatrixConstructorTest::rows;
		using MatrixTest::grid_initializer;

		static constexpr auto list = { 1, 2 };

		static constexpr std::initializer_list<int> expected_matrix = {
			 1, 2, 1, 
			 2, 1, 2, 
			 1, 2, 1,
			 2, 1, 2
		};

		static inline auto expected_column_vector = { 1, 2, 1, 2 };

		static inline auto expected_row_vector = { 1, 2, 1 };

		DynamicMatrix<int>				fully_dynamic_matrix{ Rows(rows), Columns(columns), Fill(list) };
		Matrix<int, DYNAMIC, columns>	dynamic_rows_matrix{ Rows(rows), Fill(list) };
		Matrix<int, rows, DYNAMIC>		dynamic_columns_matrix{ Columns(columns), Fill(list) };
		DynamicVector<int>				dynamic_vector{ Rows(rows), Fill(list) };
		DynamicRowVector<int>			dynamic_row_vector{ Columns(columns), Fill(list) };
		Matrix<int, rows, columns>		static_matrix{ Fill(list) };
		Vector<int, rows>				static_vector{ Fill(list) };
		RowVector<int, columns>			static_row_vector{ Fill(list) };
	};

	class GridInitializedMatrixTest : public MatrixConstructorTest {
	protected:
		using MatrixConstructorTest::columns;
		using MatrixConstructorTest::rows;
		using MatrixTest::grid_initializer;

		static inline grid_initializer input = {
			{ 0, 1, 2 }, 
			{ 3, 4, 5 }, 
			{ 6, 7, 8 },
			{ 9, 9, 9 }
		};

		static inline grid_initializer reversed_dimensions_grid = {
			{ 0, 1, 2, 3 }, 
			{ 4, 5, 6, 7 },
			{ 8, 9, 9, 9 }
		};

		static inline grid_initializer missing_row_grid = {
			{ 0, 1, 2 }, 
			{ 3, 4, 5 },
			{ 6, 7, 8 }
		};

		static inline grid_initializer missing_column_grid = {
			{ 0, 1 }, 
			{ 3, 4 },
			{ 6, 7 },
			{ 9, 9 }
		};

		static inline grid_initializer incomplete_grid = {
			{ 0, 1, 3 }, 
			{ 3, 4 },
			{ 6, 7, 8 },
			{ 9, 9, 9 }
		};


		static inline std::initializer_list<int> excpected_sequence = { 
			0, 1, 2, 
			3, 4, 5, 
			6, 7, 8, 
			9, 9, 9
		};

		DynamicMatrix<int>				fully_dynamic_matrix{ input };
		Matrix<int, DYNAMIC, columns>	dynamic_rows_matrix{ input };
		Matrix<int, rows, DYNAMIC>		dynamic_columns_matrix{ input };
		Matrix<int, rows, columns>		static_matrix{ input };
	};

	class ColumnMajorInitializedMatrixTest : public MatrixConstructorTest {
	protected:
		using MatrixTest::grid_initializer;

		static inline grid_initializer input_grid = {
			{ 0, 1, 2 }, 
			{ 3, 4, 5 }, 
			{ 6, 7, 8 },
			{ 9, 9, 9 }
		};

		static constexpr std::initializer_list<int> expected_grid_sequence = {
			0, 3, 6, 9,
			1, 4, 7, 9,
			2, 5, 8, 9
		};

		// filled matrix looks like:
		//	1, 2, 3, 
		//	2, 3, 1,
		//	3, 1, 2,
		//	1, 2, 3

		static constexpr std::initializer_list<int> input_fill = {
			1, 2, 3
		};

		static constexpr std::initializer_list<int> expected_fill_sequence = {
			1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3
		};

		DynamicMatrix<int, false>				fully_dynamic_matrix_grid{ input_grid };
		DynamicMatrix<int, false>				fully_dynamic_matrix_fill{ Rows(rows), Columns(columns), Fill(input_fill) };
		Matrix<int, DYNAMIC, columns, false>	dynamic_rows_matrix_grid{ input_grid };
		Matrix<int, DYNAMIC, columns, false>	dynamic_rows_matrix_fill{ Rows(rows), Fill(input_fill) };
		Matrix<int, rows, DYNAMIC, false>		dynamic_columns_matrix_grid{ input_grid };
		Matrix<int, rows, DYNAMIC, false>		dynamic_columns_matrix_fill{ Columns(columns), Fill(input_fill) };
		Matrix<int, rows, columns, false>		static_matrix_grid{ input_grid };
		Matrix<int, rows, columns, false>		static_matrix_fill{ Fill(input_fill) };
	};


	TEST_F(DefaultMatrixTest, DefaultDynamicMatrixHasZeroSize) {
		EXPECT_EQ(this->fully_dynamic_matrix.size(), 0);
		EXPECT_TRUE(this->fully_dynamic_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_rows_matrix.size(), 0);
		EXPECT_TRUE(this->dynamic_rows_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_columns_matrix.size(), 0);
		EXPECT_TRUE(this->dynamic_columns_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_vector.size(), 0);
		EXPECT_TRUE(this->dynamic_vector.isEmpty());	

		EXPECT_EQ(this->dynamic_row_vector.size(), 0);
		EXPECT_TRUE(this->dynamic_row_vector.isEmpty());
	}

	TEST_F(
		DefaultMatrixTest, 
		DefaultStaticMatrixHasSizeEqualToProductOfDimensions
	) {
		auto rows = MatrixConstructorTest::rows;
		auto columns = MatrixConstructorTest::columns;

		EXPECT_EQ(this->static_matrix.size(), columns * rows);
		EXPECT_FALSE(this->static_matrix.isEmpty());

		EXPECT_EQ(this->static_vector.size(), rows);
		EXPECT_FALSE(this->static_vector.isEmpty());

		EXPECT_EQ(this->static_row_vector.size(), columns);
		EXPECT_FALSE(this->static_row_vector.isEmpty());
	}

	TEST_F(DefaultMatrixTest, DefaultMatrixHasExpectedDimensions) {
		auto rows = MatrixConstructorTest::rows;
		auto columns = MatrixConstructorTest::columns;

		EXPECT_EQ(this->fully_dynamic_matrix.numRows(), 0);
		EXPECT_EQ(this->fully_dynamic_matrix.numColumns(), 0);

		EXPECT_EQ(this->dynamic_rows_matrix.numRows(), 0);
		EXPECT_EQ(this->dynamic_rows_matrix.numColumns(), columns);

		EXPECT_EQ(this->dynamic_columns_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_columns_matrix.numColumns(), 0);

		EXPECT_EQ(this->static_matrix.numRows(), rows);
		EXPECT_EQ(this->static_matrix.numColumns(), columns);

		EXPECT_EQ(this->dynamic_vector.numRows(), 0);
		EXPECT_EQ(this->dynamic_vector.numColumns(), 1);

		EXPECT_EQ(this->dynamic_row_vector.numRows(), 1);
		EXPECT_EQ(this->dynamic_row_vector.numColumns(), 0);

		EXPECT_EQ(this->static_vector.numRows(), rows);
		EXPECT_EQ(this->static_vector.numColumns(), 1);

		EXPECT_EQ(this->static_row_vector.numRows(), 1);
		EXPECT_EQ(this->static_row_vector.numColumns(), columns);
	}

	TEST_F(
		DefaultMatrixTest, 
		DefaultStaticMatrixIsFilledWithDefaultedScalarValue
	) {
		this->expectFilledBy(this->static_matrix, 0);
		this->expectFilledBy(this->static_vector, 0);
		this->expectFilledBy(this->static_row_vector, 0);
	}

	TEST_F(
		DimensionInitializedMatrixTest, 
		DimensionInitializedMatrixHasExpectedDimensions
	) {
		auto columns = MatrixConstructorTest::columns;
		auto rows = MatrixConstructorTest::rows;

		EXPECT_EQ(this->fully_dynamic_matrix.numColumns(), columns);
		EXPECT_EQ(this->fully_dynamic_matrix.numRows(), rows);
		EXPECT_EQ(this->fully_dynamic_matrix.size(), columns * rows);
		EXPECT_FALSE(this->fully_dynamic_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_rows_matrix.numColumns(), columns);
		EXPECT_EQ(this->dynamic_rows_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_rows_matrix.size(), columns * rows);
		EXPECT_FALSE(this->dynamic_rows_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_columns_matrix.numColumns(), columns);
		EXPECT_EQ(this->dynamic_columns_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_columns_matrix.size(), columns * rows);
		EXPECT_FALSE(this->dynamic_columns_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_vector.numColumns(), 1);
		EXPECT_EQ(this->dynamic_vector.numRows(), rows);
		EXPECT_EQ(this->dynamic_vector.size(), rows);
		EXPECT_FALSE(this->dynamic_vector.isEmpty());

		EXPECT_EQ(this->dynamic_row_vector.numColumns(), columns);
		EXPECT_EQ(this->dynamic_row_vector.numRows(), 1);
		EXPECT_EQ(this->dynamic_row_vector.size(), columns);
		EXPECT_FALSE(this->dynamic_row_vector.isEmpty());
	}

	TEST_F(
		DimensionInitializedMatrixTest, 
		DimensionnitializedMatrixIsFilledWithDefaultedScalarValue
	) {
		this->expectFilledBy(this->fully_dynamic_matrix, 0);
		this->expectFilledBy(this->dynamic_rows_matrix, 0);
		this->expectFilledBy(this->dynamic_columns_matrix, 0);
		this->expectFilledBy(this->dynamic_vector, 0);
		this->expectFilledBy(this->dynamic_row_vector, 0);
	}

	TEST_F(
		DimensionInitializedMatrixTest, 
		MatrixInitializedWithZeroSizeDimensionsThrowsException
	) {
		auto zero_rows_dynamic_matrix = [&]() {
			return DynamicMatrix<int>{ Rows(0), Columns(columns) };
		};

		auto zero_columns_dynamic_matrix = [&]() {
			return DynamicMatrix<int>{ Rows(rows), Columns(0) };
		};

		auto zero_dimensions_dynamic_matrix = [&]() {
			return DynamicMatrix<int>{ Rows(0), Columns(0) };
		};

		auto dynamic_columns_matrix = [&]() {
			return Matrix<int, rows, DYNAMIC>{ Columns(0) };
		};

		auto dynamic_rows_matrix = [&]() {
			return Matrix<int, DYNAMIC, columns>{ Rows(0) };
		};

		EXPECT_THROW(zero_rows_dynamic_matrix(), std::invalid_argument);
		EXPECT_THROW(zero_columns_dynamic_matrix(), std::invalid_argument);
		EXPECT_THROW(zero_dimensions_dynamic_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_columns_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_rows_matrix(), std::invalid_argument);
	}

	TEST_F(
		SingleFillInitializedMatrixTest, 
		MatrixInitializedWithSingleValueIsFilledWithGivenScalar
	) {
		this->expectFilledBy(this->fully_dynamic_matrix, this->value);
		this->expectFilledBy(this->dynamic_rows_matrix, this->value);
		this->expectFilledBy(this->dynamic_columns_matrix, this->value);
		this->expectFilledBy(this->dynamic_vector,  this->value);
		this->expectFilledBy(this->dynamic_row_vector, this->value);
		this->expectFilledBy(this->static_matrix, this->value);
		this->expectFilledBy(this->static_vector, this->value);
		this->expectFilledBy(this->static_row_vector, this->value);
	}

	TEST_F(
		FillListInitializedMatrixTest, 
		MatrixInitializedWithListHasExpectedDimensions
	) {
		auto columns = MatrixConstructorTest::columns;
		auto rows = MatrixConstructorTest::rows;

		EXPECT_EQ(this->fully_dynamic_matrix.numColumns(), columns);
		EXPECT_EQ(this->fully_dynamic_matrix.numRows(), rows);
		EXPECT_EQ(this->fully_dynamic_matrix.size(), columns * rows);
		EXPECT_FALSE(this->fully_dynamic_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_rows_matrix.numColumns(), columns);
		EXPECT_EQ(this->dynamic_rows_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_rows_matrix.size(), columns * rows);
		EXPECT_FALSE(this->dynamic_rows_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_columns_matrix.numColumns(), columns);
		EXPECT_EQ(this->dynamic_columns_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_columns_matrix.size(), columns * rows);
		EXPECT_FALSE(this->dynamic_columns_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_vector.numColumns(), 1);
		EXPECT_EQ(this->dynamic_vector.numRows(), rows);
		EXPECT_EQ(this->dynamic_vector.size(), rows);
		EXPECT_FALSE(this->dynamic_vector.isEmpty());

		EXPECT_EQ(this->dynamic_row_vector.numColumns(), columns);
		EXPECT_EQ(this->dynamic_row_vector.numRows(), 1);
		EXPECT_EQ(this->dynamic_row_vector.size(), columns);
		EXPECT_FALSE(this->dynamic_row_vector.isEmpty());
	}

	TEST_F(
		FillListInitializedMatrixTest, 
		MatrixInitializedWithListContainsRepeatedCopiedOfFillList
	) {
		this->expectSequence(this->fully_dynamic_matrix, this->expected_matrix);
		this->expectSequence(this->dynamic_columns_matrix, this->expected_matrix);
		this->expectSequence(this->dynamic_rows_matrix, this->expected_matrix);
		this->expectSequence(this->static_matrix, this->expected_matrix);
		this->expectSequence(this->dynamic_vector, this->expected_column_vector);
		this->expectSequence(this->dynamic_row_vector, this->expected_row_vector);
		this->expectSequence(this->static_vector, this->expected_column_vector);
		this->expectSequence(this->static_row_vector, this->expected_row_vector);
	}

	TEST_F(
		FillListInitializedMatrixTest, 
		MatrixInitializedWithOversizedListThrowsException
	) {
		static constexpr auto columns = MatrixConstructorTest::columns;
		static constexpr auto rows = MatrixConstructorTest::rows;

		std::initializer_list<int> ilist = { 
			7, 7, 7, 
			7, 7, 7, 
			7, 7, 7, 
			7, 7, 7, 
			7 
		};

		ASSERT_EQ(ilist.size(), ((columns * rows) + 1));

		auto fully_dynamic_matrix = [&]() {
			return DynamicMatrix<int>{ Rows(rows), Columns(columns), Fill(ilist) };
		};
		
		auto dynamic_columns_matrix = [&]() {
			return Matrix<int, rows, DYNAMIC>{ Columns(columns), Fill(ilist) };
		};

		auto dynamic_rows_matrix = [&]() {
			return Matrix<int, DYNAMIC, columns>{ Rows(rows), Fill(ilist) };
		};
		
		auto static_matrix = [&]() {
			return Matrix<int, rows, columns>{ Fill(ilist) };
		};
		
		auto static_vector = [&]() {
			return Vector<int, rows>{ Fill(ilist) };
		};
		
		auto static_row_vector = [&]() {
			return RowVector<int, columns>{ Fill(ilist) };
		};

		EXPECT_THROW(fully_dynamic_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_columns_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_rows_matrix(), std::invalid_argument);
		EXPECT_THROW(static_matrix(), std::invalid_argument);
		EXPECT_THROW(static_vector(), std::invalid_argument);
		EXPECT_THROW(static_row_vector(), std::invalid_argument);
	}

	TEST_F(
		GridInitializedMatrixTest, 
		MatrixInitializedWithGridHasExpectedDimensions
	) {
		auto columns = MatrixConstructorTest::columns;
		auto rows = MatrixConstructorTest::rows;

		EXPECT_EQ(this->fully_dynamic_matrix.numColumns(), columns);
		EXPECT_EQ(this->fully_dynamic_matrix.numRows(), rows);
		EXPECT_EQ(this->fully_dynamic_matrix.size(), columns * rows);
		EXPECT_FALSE(this->fully_dynamic_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_rows_matrix.numColumns(), columns);
		EXPECT_EQ(this->dynamic_rows_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_rows_matrix.size(), columns * rows);
		EXPECT_FALSE(this->dynamic_rows_matrix.isEmpty());

		EXPECT_EQ(this->dynamic_columns_matrix.numColumns(), columns);
		EXPECT_EQ(this->dynamic_columns_matrix.numRows(), rows);
		EXPECT_EQ(this->dynamic_columns_matrix.size(), columns * rows);
		EXPECT_FALSE(this->dynamic_columns_matrix.isEmpty());
	}

	TEST_F(
		GridInitializedMatrixTest, 
		MatrixInitializedWithGridContainsGivenElements
	) {
		this->expectSequence(this->fully_dynamic_matrix, this->excpected_sequence);
		this->expectSequence(this->dynamic_columns_matrix, this->excpected_sequence);
		this->expectSequence(this->dynamic_rows_matrix, this->excpected_sequence);
		this->expectSequence(this->static_matrix, this->excpected_sequence);
	}

	TEST_F(
		GridInitializedMatrixTest, 
		MatrixInitializedWithInvalidDimensionsGridThrowsException
	) {
		static constexpr auto columns = MatrixConstructorTest::columns;
		static constexpr auto rows = MatrixConstructorTest::rows;

		auto dynamic_columns_matrix = [&]() {
			return Matrix<int, rows, DYNAMIC>{ this->reversed_dimensions_grid };
		};

		auto dynamic_rows_matrix = [&]() {
			return Matrix<int, DYNAMIC, columns>{ this->reversed_dimensions_grid };
		};

		auto static_matrix = [&]() {
			return Matrix<int, rows, columns>{ this->reversed_dimensions_grid };
		};

		EXPECT_THROW(dynamic_columns_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_rows_matrix(), std::invalid_argument);
		EXPECT_THROW(static_matrix(), std::invalid_argument);
	}

	TEST_F(
		GridInitializedMatrixTest, 
		MatrixInitializedWithGridMissingColumnsThrowsException
	) {
		static constexpr auto columns = MatrixConstructorTest::columns;
		static constexpr auto rows = MatrixConstructorTest::rows;

		auto dynamic_rows_matrix = [&]() {
			return Matrix<int, DYNAMIC, columns>{ this->missing_column_grid };
		};

		auto static_matrix = [&]() {
			return Matrix<int, rows, columns>{ this->missing_column_grid };
		};

		EXPECT_THROW(dynamic_rows_matrix(), std::invalid_argument);
		EXPECT_THROW(static_matrix(), std::invalid_argument);
	}

	TEST_F(
		GridInitializedMatrixTest, 
		MatrixInitializedWithGridMissingRowsThrowsException
	) {
		static constexpr auto columns = MatrixConstructorTest::columns;
		static constexpr auto rows = MatrixConstructorTest::rows;

		auto dynamic_columns_matrix = [&]() {
			return Matrix<int, rows, DYNAMIC>{ this->missing_row_grid };
		};

		auto static_matrix = [&]() {
			return Matrix<int, rows, columns>{ this->missing_row_grid };
		};

		EXPECT_THROW(dynamic_columns_matrix(), std::invalid_argument);
		EXPECT_THROW(static_matrix(), std::invalid_argument);
	}

	TEST_F(
		GridInitializedMatrixTest, 
		MatrixInitializedWithIncompleteGridThrowsException
	) {
		static constexpr auto columns = MatrixConstructorTest::columns;
		static constexpr auto rows = MatrixConstructorTest::rows;

		auto fully_dynamic_matrix = [&]() {
			return DynamicMatrix<int>{ this->incomplete_grid };
		};

		auto dynamic_columns_matrix = [&]() {
			return Matrix<int, rows, DYNAMIC>{ this->incomplete_grid };
		};

		auto dynamic_rows_matrix = [&]() {
			return Matrix<int, DYNAMIC, columns>{ this->incomplete_grid };
		};

		auto static_matrix = [&]() {
			return Matrix<int, rows, columns>{ this->incomplete_grid };
		};

		EXPECT_THROW(fully_dynamic_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_columns_matrix(), std::invalid_argument);
		EXPECT_THROW(dynamic_rows_matrix(), std::invalid_argument);
		EXPECT_THROW(static_matrix(), std::invalid_argument);
	}

	TEST_F(
		ColumnMajorInitializedMatrixTest, 
		ColumnMajorMatrixStoresElementsInExpectedSequence
	) {
		this->expectSequence(this->fully_dynamic_matrix_grid, this->expected_grid_sequence);
		this->expectSequence(this->fully_dynamic_matrix_fill, this->expected_fill_sequence);
		this->expectSequence(this->dynamic_columns_matrix_grid, this->expected_grid_sequence);
		this->expectSequence(this->dynamic_columns_matrix_fill, this->expected_fill_sequence);
		this->expectSequence(this->dynamic_rows_matrix_grid, this->expected_grid_sequence);
		this->expectSequence(this->dynamic_rows_matrix_fill, this->expected_fill_sequence);
		this->expectSequence(this->static_matrix_grid, this->expected_grid_sequence);
		this->expectSequence(this->static_matrix_fill, this->expected_fill_sequence);
	}
}