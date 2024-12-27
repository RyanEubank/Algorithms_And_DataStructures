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

#define ERR_UNEQUAL_DIMENSIONS "Cannot add/subtract matricies with different dimensions."

#define ERR_MULTIPLICATION "Cannot multiply matricies if left matrix rows does" \
						   " not equal right matrix columns."

#include <algorithm>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <ranges>
#include <type_traits>

#include "../adapters/BackInserter.h"
#include "../algorithms/copy.h"
#include "../algorithms/fill.h"
#include "../algorithms/search.h"
#include "../containers/DynamicArray.h"
#include "../containers/StaticArray.h"
#include "../util/types.h"

namespace collections {

	static constexpr size_t DYNAMIC = 0;

	using Columns	= NamedType<size_t, struct ColumnSize>;
	using Rows		= NamedType<size_t, struct RowsSize>;

	template <class T>
	using Fill = NamedType<std::initializer_list<T>, struct FillType>;

	template <class T>
	using grid_initializer 
		= std::initializer_list<std::initializer_list<T>>;

	// ------------------------ Matrix Type Traits -------------------------- //

	template <class T>
	struct is_column_vector {
		static constexpr bool value = (T::columns_at_compile == 1);
	};
	
	template <class T>
	inline constexpr bool is_column_vector_v = is_column_vector<T>::value;

	template <class T>
	struct is_row_vector {
		static constexpr bool value = (T::rows_at_compile == 1);
	};

	template <class T>
	inline constexpr bool is_row_vector_v = is_row_vector<T>::value;

	template <class T>
	struct has_dynamic_rows {
		static constexpr bool value = (T::rows_at_compile == DYNAMIC);
	};

	template <class T>
	inline constexpr bool has_dynamic_rows_v = has_dynamic_rows<T>::value;

	template <class T>
	struct has_dynamic_columns {
		static constexpr bool value = (T::columns_at_compile == DYNAMIC);
	};

	template <class T>
	inline constexpr bool has_dynamic_columns_v = has_dynamic_columns<T>::value;

	template <class T>
	struct has_multiple_rows {
		static constexpr bool value = (T::rows_at_compile > 1);
	};

	template <class T>
	inline constexpr bool has_multiple_rows_v = has_multiple_rows<T>::value;

	template <class T>
	struct has_multiple_columns {
		static constexpr bool value = (T::columns_at_compile > 1);
	};

	template <class T>
	inline constexpr bool has_multiple_columns_v = has_multiple_columns<T>::value;

	template <class T>
	struct partially_dynamic {
		static constexpr bool value = 
			has_dynamic_columns_v<T> || has_dynamic_rows_v<T>;
	};

	template <class T>
	inline constexpr bool partially_dynamic_v = partially_dynamic<T>::value;

	template <class T>
	struct fully_dynamic {
		static constexpr bool value = 
			has_dynamic_columns_v<T> && has_dynamic_rows_v<T>;
	};

	template <class T>
	inline constexpr bool fully_dynamic_v = fully_dynamic<T>::value;

	template <class T>
	struct fixed_size {
		static constexpr bool value = !partially_dynamic_v<T>;
	};

	template <class T>
	inline constexpr bool fixed_size_v = fixed_size<T>::value;

	template <class T>
	struct unidimensional {
		static constexpr bool value = 
			is_column_vector_v<T> || is_row_vector_v<T>;
	};

	template <class T>
	inline constexpr bool unidimensional_v = unidimensional<T>::value;

	template <class T>
	struct multidimensional {
		static constexpr bool value = !unidimensional_v<T>;
	};

	template <class T>
	inline constexpr bool multidimensional_v = multidimensional<T>::value;

	template <class T, class U>
		requires (!has_dynamic_rows_v<T> && !has_dynamic_rows_v<U>)
	struct has_same_rows {
		static constexpr bool value = 
			(T::rows_at_compile == U::rows_at_compile);
	};

	template <class T, class U>
	inline constexpr bool has_same_rows_v = has_same_rows<T, U>::value;

	template <class T, class U>
		requires (!has_dynamic_columns_v<T> && !has_dynamic_columns_v<U>)
	struct has_same_columns {
		static constexpr bool value = 
			(T::columns_at_compile == U::columns_at_compile);
	};

	template <class T, class U>
	inline constexpr bool has_same_columns_v = has_same_columns<T, U>::value;

	// -------------------------------------------------------------------------
	/// <summary>
	/// 
	/// </summary>
	/// 
	/// <typeparam name="scalar_t">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="rows">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="columns">
	/// 
	/// </typeparam> 
	/// 
	/// <typeparam name="isRowMajorOrder">
	/// 
	/// </typeparam> -----------------------------------------------------------
	template <
		class scalar_t, 
		size_t rows, 
		size_t columns, 
		bool isRowMajorOrder = true
	>
	class Matrix {
	public:

		static constexpr size_t rows_at_compile	= rows;
		static constexpr size_t columns_at_compile = columns;
		static constexpr bool isRowMajor = isRowMajorOrder;

	private:

		using matrix_type = Matrix<scalar_t, rows, columns, isRowMajorOrder>;

		using array_type = std::conditional_t<
			partially_dynamic_v<matrix_type>,
			DynamicArray<scalar_t>,
			StaticArray<scalar_t, rows_at_compile * columns_at_compile>
		>;

		template <bool isConst, bool isRowMajorOrder>
		class MatrixIterator;

		template <bool isConst, bool isRowMajorOrder>
		class FlatView;

		class RowView;

		class ColumnView;

		struct SubmatrixShape;

		friend class Matrix; // friend of all matrix specializations

	public:

		using value_type		= array_type::value_type;
		using reference			= array_type::reference;
		using const_reference	= array_type::const_reference;
		using pointer			= array_type::pointer;
		using const_pointer		= array_type::const_pointer;
		using size_type			= array_type::size_type;
		using difference_type	= array_type::difference_type;
		using iterator			= array_type::iterator;
		using const_iterator	= array_type::const_iterator;

		using row_type = Matrix<value_type, rows_at_compile, 1, true>;
		using column_type = Matrix<value_type, 1, columns_at_compile, true>;

		// TODO implement dimensions struct for {.columns = ..., .rows = ... } aggregate initialization
	
		// TODO determine correct constexpr context for all methods
		
		// ---------------------------------------------------------------------
		/// <summary>
		///  ~~~ Default Matrix/Vector Constructor ~~~
		/// 
		/// <para>
		/// Constructs a default matrix or vector. Fixed-sized matricies 
		/// are filled with default constructed values while dynamic matricies 
		/// do not allocate memory and are left empty.
		/// </para></summary> --------------------------------------------------
		constexpr Matrix() : _array(),  _rows(), _columns() {
			if constexpr (fixed_size_v<matrix_type>)
				collections::fill(_array, value_type{});
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically-sized matrix with the specified dimensions
		/// and initializes it by repeating the list of given values until full.
		/// </summary> 
		///
		/// <param name="rows">
		/// The number of rows to initialize the matrix with.
		/// </param>
		/// 
		/// <param name="columns">
		/// The number of columns to initialize the matrix with.
		/// </param>
		/// 
		/// <param name="values">
		/// The range of values to initialize the matrix with. If the list is
		/// smaller than the matrix size then the list is repeatedly copied
		/// until all values are initialized.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(
			Rows rows, 
			Columns columns,  
			Fill<value_type> values = Fill({ value_type{} })
		) requires fully_dynamic_v<matrix_type> :
			_array(Reserve(columns.get() * rows.get())),
			_rows(rows.get()),
			_columns(columns.get())
		{
			init(values.get());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically-sized matrix with the specified initializer 
		/// grid.
		/// </para></summary> 
		/// 
		/// <param name="grid">
		/// The initializer values to construct the matrix elements and 
		/// dimensions with.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(grid_initializer<value_type> grid) 
			requires fully_dynamic_v<matrix_type> :
			_array(Reserve(grid.size() * grid.begin()->size())),
			_rows(grid.size()),
			_columns(grid.begin()->size())
		{
			init(grid);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Rows Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a partially dynamically-sized matrix with the specified 
		/// number of rows and initializes it by repeating the list of given 
		/// values until full.
		/// </para></summary> 
		/// 
		/// <param name="rows">
		/// The number of rows to initialize the matrix with.
		/// </param>
		/// 
		/// <param name="values">
		/// The range of values to initialize the matrix with. If the list is
		/// smaller than the matrix size then the list is repeatedly copied
		/// until all values are initialized.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(
			Rows rows, 
			Fill<value_type> values = Fill({ value_type{} })
		) requires (
			has_dynamic_rows_v<matrix_type> &&
			has_multiple_columns_v<matrix_type>
		) :
			_array(Reserve(columns_at_compile * rows.get())),
			_rows(rows.get()),
			_columns()
		{
			init(values.get());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Rows Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a partially dynamically-sized matrix with the specified 
		/// initializer grid.
		/// </para></summary> 
		/// 
		/// <param name="grid">
		/// The initializer values to construct the matrix elements and row
		/// dimensions with. Row dimensions must match the static column size.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(grid_initializer<value_type> grid) requires (
			has_dynamic_rows_v<matrix_type> &&
			has_multiple_columns_v<matrix_type>
		) :
			_array(Reserve(grid.size() * grid.begin()->size())),
			_rows(grid.size()),
			_columns()
		{
			init(grid);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Columns Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a partially dynamically-sized matrix with the specified 
		/// number of columns and initializes it by repeating the list of given 
		/// values until full.
		/// </para></summary> 
		/// 
		/// <param name="columns">
		/// The number of columns to initialize the matrix with.
		/// </param>
		/// 
		/// <param name="values">
		/// The range of values to initialize the matrix with. If the list is
		/// smaller than the matrix size then the list is repeatedly copied
		/// until all values are initialized.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(
			Columns columns, 
			Fill<value_type> values = Fill({ value_type{} })
		) requires (
			has_multiple_rows_v<matrix_type> &&
			has_dynamic_columns_v<matrix_type>
		) : 
			_array(Reserve(columns.get() * rows_at_compile)),
			_rows(),
			_columns(columns.get())
		{
			init(values.get());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Columns Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a partially dynamically-sized matrix with the specified 
		/// initializer grid.
		/// </para></summary> 
		/// 
		/// <param name="grid">
		/// The initializer values to construct the matrix elements and 
		/// column dimensions with. Row dimensions must match the static row 
		/// size.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(grid_initializer<value_type> grid) requires (
			has_multiple_rows_v<matrix_type> &&
			has_dynamic_columns_v<matrix_type>
		) :
			_array(Reserve(grid.size() * grid.begin()->size())),
			_rows(),
			_columns(grid.begin()->size())
		{
			init(grid);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Fixed Size Matrix/Vector Constructor ~~~
		/// 
		/// <para>
		/// Constructs a fixed-sized matrix initializing its element by
		/// repeating the given list of values until full.
		/// </para></summary>
		/// 
		/// <param name="values">
		/// The range of values to initialize the matrix with. If the list is
		/// smaller than the matrix size then the list is repeatedly copied
		/// until all values are initialized.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(Fill<value_type> values) 
			requires fixed_size_v<matrix_type> : 
			_array(), _rows(), _columns() 
		{
			init(values.get());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Static Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically-sized matrix with the specified initializer 
		/// grid.
		/// </para></summary> 
		/// 
		/// <param name="grid">
		/// The initializer values to construct the matrix elements with. Grid 
		/// dimensions must match the static column and row size.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(grid_initializer<value_type> grid) requires (
			fixed_size_v<matrix_type> && multidimensional_v<matrix_type>
		) : _array(), _rows(), _columns() {
			init(grid);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Column Vector Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically sized column vector with the specified 
		/// initializer list.
		/// </para></summary>
		/// 
		/// <param name="ilist">
		/// The list of values to initialize the vector with.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(std::initializer_list<value_type> ilist) requires (
			has_dynamic_rows_v<matrix_type> && is_column_vector_v<matrix_type>
		) : _array(ilist), _rows(ilist.size()), _columns() {

		}


		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Column Vector Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically sized column vector with the specified row
		/// dimensions and fill list.
		/// </para></summary>
		/// 
		/// <param name="ilist">
		/// The list of values to initialize the vector with. If the list is
		/// smaller than the vector size then the list is repeatedly copied 
		/// until all values are initialized.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(
			Rows rows, 
			Fill<value_type> values = Fill({ value_type{} })
		) requires (
			has_dynamic_rows_v<matrix_type> && is_column_vector_v<matrix_type>
		) :
			_array(Reserve(rows.get())), 
			_rows(rows.get()),
			_columns()
		{
			init(values.get());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Row Vector Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically sized row vector with the specified 
		/// initializer list.
		/// </para></summary>
		/// 
		/// <param name="ilist">
		/// The list of values to initialize the vector with.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(std::initializer_list<value_type> ilist) requires (
			is_row_vector_v<matrix_type> && has_dynamic_columns_v<matrix_type>
		) : _array(ilist), _rows(), _columns(ilist.size()) {

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Row Vector Constructor ~~~
		/// 
		/// <para>
		/// Constructs a dynamically sized column vector with the specified 
		/// column dimensions and fill list.
		/// </para></summary>
		/// 
		/// <param name="ilist">
		/// The list of values to initialize the vector with. If the list is
		/// smaller than the vector size then the list is repeatedly copied 
		/// until all values are initialized.
		/// </param> -----------------------------------------------------------
		constexpr Matrix(
			Columns columns, 
			Fill<value_type> values = Fill({ value_type{} })
		) requires (
			is_row_vector_v<matrix_type> && has_dynamic_columns_v<matrix_type>
		) :
			_array(Reserve(columns.get())), 
			_rows(),
			_columns(columns.get())
		{
			init(values.get());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Dynamic Matrix Constructor ~~~
		/// 
		/// <para>
		/// Constructs an empty dynamically-sized matrix with memory 
		/// pre-reserved for the specified amount of elements.
		/// </summary> 
		///
		/// <param name="reserve">
		/// The number of elements to allocate memory for up front.
		/// </param> -----------------------------------------------------------
		Matrix(Reserve amount) requires partially_dynamic_v<matrix_type> :
			_array(amount), _rows(), _columns()
		{

		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the size or number of elements in the matrix.
		/// </summary>
		/// 
		/// <returns>
		/// Returns te total size of the matrix, or number of rows * columns.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr size_type size() const {
			return numRows() * numColumns();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the number of rows in the matrix.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of rows, or the size of the vertical dimension 
		/// of the matrix.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr size_type numRows() const {
			if constexpr (has_dynamic_rows_v<matrix_type>)
				return _rows;
			else
				return rows_at_compile;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the number of columns in the matrix.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the number of columns, or the size of the horizontal 
		/// dimension of the matrix.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr size_type numColumns() const {
			if constexpr (has_dynamic_columns_v<matrix_type>)
				return _columns;
			else
				return columns_at_compile;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Reserves space internally in the matrix for the specified rows and
		/// columns.
		/// </summary>
		/// 
		/// <param name="r">
		/// The number of rows to reserve space for. This is ignored if the rows
		/// are fixed size.
		/// </param>
		/// 
		/// <param name="c">
		/// The number of columns to reserve space for. This is ignored if the
		/// columns are fixed size.
		/// </param> -----------------------------------------------------------
		void reserve(Rows r, Columns c) 
			requires partially_dynamic_v<matrix_type> 
		{
			size_type rows = numRows();
			size_type cols = numColumns();

			if constexpr (has_dynamic_rows_v<matrix_type>) 
				rows = r.get();
			if constexpr (has_dynamic_columns_v<matrix_type>) 
				cols = c.get();

			_array.reserve(rows * cols);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns whether the matrix is empty.
		/// </summary>
		/// 
		/// <returns>
		/// Returns true if the array is dynamically sized and does not yet
		/// contain elements (was default constructed). Always returns false
		/// if the matrix is a fixed size.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] constexpr bool isEmpty() const {
			if constexpr (fixed_size_v<matrix_type>)
				return false;
			else
				return _array.isEmpty();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returna the internal array managing the data for the matrix.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the matricies internal storage.
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] constexpr array_type& data(this T&& self) {
			return std::forward<T>(self)._array;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the first element of the matrix.
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the element in the to left position in the 
		/// matrix with index [0, 0].
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] constexpr auto begin(this T&& self) {
			return std::forward<T>(self)._array.begin();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator to the end of the matrix.
		/// </summary>
		/// 
		/// <returns>
		/// Returns an iterator to the position after the last element in the
		/// matrix, or the element in the bottom right position with index 
		/// [m, n].
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] constexpr auto end(this T&& self) {
			return std::forward<T>(self)._array.end();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a flat view for the matrix to iterate over its elements row 
		/// by row.
		/// </summary>
		/// 
		/// <returns>
		/// Return the matrix itself if it already stores its element in row
		/// major ordering, otherwise returns an adapter to iterator row by row.
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] auto flattenedRows(this T&& self) 
			requires multidimensional_v<matrix_type> 
		{
			using view_type = 
				FlatView<std::is_const_v<std::remove_reference_t<T>>, true>;

			if constexpr (isRowMajor)
				return std::forward<T>(self);
			else
				return view_type{std::addressof(self)};
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a flat view for the matrix to iterate over its elements 
		/// column by column.
		/// </summary>
		/// 
		/// <returns>
		/// Return the matrix itself if it already stores its element in column
		/// major ordering, otherwise returns an adapter to iterate column by
		/// column.
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] auto flattenedColumns(this T&& self) 
			requires multidimensional_v<matrix_type> 
		{
			using view_type = 
				FlatView<std::is_const_v<std::remove_reference_t<T>>, false>;

			if constexpr (isRowMajor)
				return view_type{std::addressof(self)};
			else
				return std::forward<T>(self);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a copy of the row at the given index.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the row to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a row vector as a copy of the row at the requested index
		/// in the matrix.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] row_type row(size_type index) const {
			row_type row{};
			submatrix({ .rows = index }, row);
			return row;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns a copy of the column at the given index.
		/// </summary>
		/// 
		/// <param name="index">
		/// The index of the column to retrieve.
		/// </param>
		/// 
		/// <returns>
		/// Returns a column vector as a copy of the column at the requested 
		/// index in the matrix.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] column_type column(size_type index) const {
			row_type column{};
			submatrix({ .columns = index }, column);
			return column;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Copies the matrix row by row to a list of row vectors and returns
		/// the range of rows as if by calling 
		/// array{ row(0), row(1), ... row(n) };
		/// </summary>
		/// 
		/// <returns>
		/// Returns a RowView range constructed from the rows of the matrix.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] RowView rows() const {
			return RowView{ *this };
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Copies the matrix column by column to a list of column vectors and 
		/// returns the range of columns as if by calling 
		/// array{ column(0), column(1), ... column(m) };
		/// </summary>
		/// 
		/// <returns>
		/// Returns a ColumnView range constructed from the columns of the 
		/// matrix.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] ColumnView columns() const {
			return ColumnView{ *this };
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// 
		/// </summary>
		/// 
		/// <param name="options">
		/// 
		/// </param>
		/// 
		/// <returns>
		/// 
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] auto submatrix(const SubmatrixShape& shape) const {
			return SubmatrixFactory::create(*this, shape);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// 
		/// </summary>
		/// 
		/// <typeparam name="submatrix_type">
		///
		/// </typeparam>
		/// 
		/// <param name="options">
		/// 
		/// </param>
		/// 
		/// <returns>
		/// 
		/// </returns> ---------------------------------------------------------
		template <class submatrix_type>
		void submatrix(const SubmatrixShape& shape, submatrix_type& out) const {
			SubmatrixFactory::create(*this, shape, out);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the elenent at the specified indecies in the matrix.
		/// </summary>
		///
		/// <param name="i">
		/// The index to the row.
		/// </param>
		/// 
		/// <param name="j">
		/// The index to the column.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified row and column
		/// index.
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] constexpr auto& operator[](
			this T&& self, 
			size_type i, 
			size_type j
		) {
			if constexpr (isRowMajor) {
				auto columns = self.numColumns();
				return std::forward<T>(self)._array[(i * columns) + j];
			}
			else {
				auto rows = self.numRows();
				return std::forward<T>(self)._array[i + (j * rows)];
			}
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the elenent at the specified index in the vector.
		/// </summary>
		///
		/// <param name="i">
		/// The index to the element.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to the element at the specified index.
		/// </returns> ---------------------------------------------------------
		template <class T>
		[[nodiscard]] constexpr auto& operator[](this T&& self, size_type i) 
			requires unidimensional_v<matrix_type>
		{
			return std::forward<T>(self)._array[i];
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Input Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The type of the character stream read by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being written to.
		/// </param>
		/// <param name="arr">
		/// The list being read from.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream after writing.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const matrix_type& matrix
		) {
			if constexpr (isRowMajor) 
				os << "Order: row major\n";
			else 
				os << "Order: column major\n";

			size_type numColumns = matrix.numColumns();
			size_type numRows = matrix.numRows();

			os << "Rows: " << numRows << "\n";
			os << "Columns: " << numColumns << "\n\n";


			for (size_type row = 0; row < numRows; ++row) {
				for (size_type column = 0; column < numColumns; ++column)
					os << matrix[row, column] << " ";
				os << std::endl;
			}

			return os;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Input Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The type of the character stream read by the operator.
		/// </typeparam>
		/// 
		/// <param name="is">
		/// The stream being read from.
		/// </param>
		/// <param name="arr">
		/// The list being written to.
		/// </param>
		/// 
		/// <returns>
		/// Returns the input stream after reading.
		/// </returns> --------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			Matrix& list
		) {
			throw std::exception("Not yet implemented.");
			return is;
		}

		// TODO custom implementation of transform algorithm and std::plus

		// TODO division.
		
		// TODO optimize arithmetic to return expression objects that are only 
		// evaluated once on assignment
		
		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Addition Operator ~~~
		/// 
		/// <para>
		/// Performs the addition of the specified value to each of the matrix
		/// elements and stores the result back in the matrix.
		/// </para></summary>
		/// 
		/// <param name="rhs">
		/// The value being added to each of the matrix's elements.
		/// </param>
		/// 
		/// <returns>
		/// Return this matrix after adding the value.
		/// </returns> ---------------------------------------------------------
		matrix_type& operator+=(const_reference val) 
			requires std::is_arithmetic_v<value_type> 
		{
			auto addVal = [&](auto& element) { return element + val; };
			std::ranges::transform(*this, begin(), addVal);
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Addition Operator ~~~
		/// 
		/// <para>
		/// Performs the addition of the specified matrix to this one and 
		/// returns the result.
		/// </para></summary>
		/// 
		/// <typeparam name="rows">
		/// The compile time row size of the matrix being added to this one.
		/// </typeparam>
		/// 
		/// <typeparam name="cols">
		/// The compile time column size of the matrix being added to this one.
		/// </typeparam>
		/// 
		/// <typeparam name="order">
		/// The storage ordering of the matrix being added to this one.
		/// </typeparam>
		///
		/// <param name="other">
		/// The matrix being added to this one - must have equal dimensions.
		/// </param>
		/// 
		/// <returns>
		/// Return this matrix after adding the other element by element.
		/// </returns> ---------------------------------------------------------
		template <size_t rows, size_t cols, bool rowMajor> 
		matrix_type& operator+=(
			const Matrix<value_type, rows, cols, rowMajor>& other
		) requires std::is_arithmetic_v<value_type> {
			validateSameDimensions(other);
			elementWiseTransform(*this, other, begin(), std::plus{});
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Addition Operator ~~~
		/// 
		/// <para>
		/// Performs the addition of the specified value to the matrix and
		/// returns the result as a new matrix.
		/// </para></summary>
		/// 
		/// <param name="lhs">
		/// The matrix being added on the left.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The value being added on the right.
		/// </param>
		/// 
		/// <returns>
		/// Return a new matrix from the resulting addition, which is always the
		/// type of the matrix appearing on the left side of the + operator.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] friend matrix_type operator+(
			const matrix_type& lhs, 
			const_reference rhs
		) requires std::is_arithmetic_v<value_type> {
			return add(lhs, rhs);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Addition Operator ~~~
		/// 
		/// <para>
		/// Performs the addition of the specified matricies and return the
		/// result as a new matrix.
		/// </para> </summary>
		/// 
		/// <typeparam name="rows">
		/// The compile time row size of the matrix being added on the right.
		/// </typeparam>
		/// 
		/// <typeparam name="cols">
		/// The compile time column size of the matrix being added on the right.
		/// </typeparam>
		/// 
		/// <typeparam name="order">
		/// The storage ordering of the matrix being added on the right.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The matrix being added on the left.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The matrix being added on the right.
		/// </param>
		/// 
		/// <returns>
		/// Return a new matrix from the resulting addition, which is always the
		/// type of the matrix appearing on the left side of the + operator.
		/// </returns> ---------------------------------------------------------
		template <size_t rows, size_t cols, bool rowMajor> 
		[[nodiscard]] friend matrix_type operator+(
			const matrix_type& lhs, 
			const Matrix<value_type, rows, cols, rowMajor>& rhs
		) requires std::is_arithmetic_v<value_type> {
			return add(lhs, rhs);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Subtraction Operator ~~~
		/// 
		/// <para>
		/// Performs the subtraction of the specified value from each of the 
		/// matrix elements and stores the result back in the matrix.
		/// </para></summary>
		/// 
		/// <param name="rhs">
		/// The value being subtracted from each of the matrix's elements.
		/// </param>
		/// 
		/// <returns>
		/// Return this matrix after subtracting the value.
		/// </returns> ---------------------------------------------------------
		matrix_type& operator-=(const_reference val) 
			requires std::is_arithmetic_v<value_type> 
		{
			auto subVal = [&](auto& element) { return element - val; };
			std::ranges::transform(*this, begin(), subVal);
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Subtraction Operator ~~~
		/// 
		/// <para>
		/// Performs the subtraction of the specified matrix from this one and 
		/// returns the result.
		/// </para></summary>
		/// 
		/// <typeparam name="rows">
		/// The compile time row size of the matrix being subtracted from this 
		/// one.
		/// </typeparam>
		/// 
		/// <typeparam name="cols">
		/// The compile time column size of the matrix being subtracted from 
		/// this one.
		/// </typeparam>
		/// 
		/// <typeparam name="order">
		/// The storage ordering of the matrix being subtracted from this one.
		/// </typeparam>
		///
		/// <param name="other">
		/// The matrix being subtracted from this one - must have equal 
		/// dimensions.
		/// </param>
		/// 
		/// <returns>
		/// Return this matrix after subtracting the other element by element.
		/// </returns> ---------------------------------------------------------
		template <size_t rows, size_t cols, bool rowMajor> 
		matrix_type& operator-=(
			const Matrix<value_type, rows, cols, rowMajor>& other
		) requires std::is_arithmetic_v<value_type> {
			validateSameDimensions(other);
			elementWiseTransform(*this, other, begin(), std::minus{});
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Subtraction Operator ~~~
		/// 
		/// <para>
		/// Performs the subtraction of the specified value from the matrix and
		/// returns the result as a new matrix.
		/// </para></summary>
		/// 
		/// <param name="lhs">
		/// The matrix being subtracted from on the left.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The value being subtracted on the right.
		/// </param>
		/// 
		/// <returns>
		/// Return a new matrix from the resulting subtraction, which is always 
		/// the type of the matrix appearing on the left side of the - operator.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] friend matrix_type operator-(
			const matrix_type& lhs, 
			const_reference rhs
		) requires std::is_arithmetic_v<value_type> {
			return subtract(lhs, rhs);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Performs the subtraction of the specified matricies and return the
		/// result as a new matrix.
		/// </summary>
		/// 
		/// <typeparam name="rows">
		/// The compile time row size of the matrix being subtracted on the 
		/// right.
		/// </typeparam>
		/// 
		/// <typeparam name="cols">
		/// The compile time column size of the matrix being subtracted on the 
		/// right.
		/// </typeparam>
		/// 
		/// <typeparam name="order">
		/// The storage ordering of the matrix being subtracted on the right.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The matrix being subtracted from on the left.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The matrix being subtracted on the right.
		/// </param>
		/// 
		/// <returns>
		/// Return a new matrix from the resulting subtraction, which is always 
		/// the type of the matrix appearing on the left side of the - operator.
		/// </returns> ---------------------------------------------------------
		template <size_t rows, size_t cols, bool rowMajor> 
		[[nodiscard]] friend matrix_type operator-(
			const matrix_type& lhs, 
			const Matrix<value_type, rows, cols, rowMajor>& rhs
		) requires std::is_arithmetic_v<value_type> {
			return subtract(lhs, rhs);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Multiplication Operator ~~~
		/// 
		/// <para>
		/// Performs the multiplication of the specified value to each of the 
		/// matrix elements and stores the result back in the matrix.
		/// </para></summary>
		/// 
		/// <param name="rhs">
		/// The value being multiplied to this matrix.
		/// </param>
		/// 
		/// <returns>
		/// Returns this matrix after multiplying the value element by element.
		/// </returns> ---------------------------------------------------------
		matrix_type& operator*=(const_reference rhs) 
			requires std::is_arithmetic_v<value_type> 
		{
			auto multiply = [&](auto& element) { return element * rhs; };
			std::ranges::transform(*this, begin(), multiply);
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Multiplication Operator ~~~
		/// 
		/// <para>
		/// Performs the multiplication of the specified value to each of the 
		/// matrix
		/// elements and stores the result back in the matrix.
		/// </para></summary>
		/// 
		/// <typeparam name="rows">
		/// The compile time row size of the matrix being multiplied to this 
		/// one.
		/// </typeparam>
		/// 
		/// <typeparam name="cols">
		/// The compile time column size of the matrix being multiplied to this 
		/// one.
		/// </typeparam>
		/// 
		/// <typeparam name="order">
		/// The storage ordering of the matrix being multiplied to this one.
		/// </typeparam>
		///
		/// <param name="other">
		/// The matrix being multiplied to this one - other's columns size must
		/// equal this row size.
		/// </param>
		/// 
		/// <returns>
		/// Returns this matrix after multiplying it with the other.
		/// </returns> ---------------------------------------------------------
		template <size_t rows, size_t cols, bool rowMajor>
		matrix_type& operator*=(
			const Matrix<value_type, rows, cols, rowMajor>& rhs
		) requires std::is_arithmetic_v<value_type> {
			// must delegate to operator+ because matrix multiplication may
			// result in a new matrix whose dimensions are different from both
			// the lhs and rhs. May not be possible if statically sized

			throw std::exception("Not yet implemented.");
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Multiplication Operator ~~~
		/// 
		/// <para>
		/// Performs the multiplication of each of the matrix elements with the 
		/// specified value.
		/// </para></summary>
		/// 
		/// <param name="lhs">
		/// The matrix being multiplied on the left.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The scalar to multiply by.
		/// </param>
		/// 
		/// <returns>
		/// Returns a new matrix from the result of the multiplication.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] friend matrix_type operator*(
			const matrix_type& lhs, 
			const_reference rhs
		) requires std::is_arithmetic_v<value_type> {
			matrix_type result{ lhs };
			result *= rhs;
			return result;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Multiplication Operator ~~~
		/// 
		/// <para>
		/// Performs the multiplication of each of the matrix elements with the 
		/// specified value.
		/// </para></summary>
		/// 
		/// <param name="lhs">
		/// The scalar to multiply by.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The matrix being multiplied on the right.
		/// </param>
		/// 
		/// <returns>
		/// Returns a new matrix from the result of the multiplication.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] friend matrix_type operator*(
			const_reference lhs, 
			const matrix_type& rhs
		) requires std::is_arithmetic_v<value_type> {
			matrix_type result{ rhs };
			result *= lhs;
			return result;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Multiplication Operator ~~~
		/// 
		/// <para>
		/// Performs the multiplication of each of the two given matricies from
		/// left to right.
		/// </para></summary>
		/// 
		/// <typeparam name="rows">
		/// The compile time row size of the matrix being multiplied on the 
		/// right.
		/// </typeparam>
		/// 
		/// <typeparam name="cols">
		/// The compile time column size of the matrix being multiplied on the
		/// right.
		/// </typeparam>
		/// 
		/// <typeparam name="order">
		/// The storage ordering of the matrix being multiplied on the right.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The matrix being multiplied on the left.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The matrix being multiplied on the right.
		/// </param>
		/// 
		/// <returns>
		/// Return the result of the multiplication as a new matrix, whose
		/// compile time row size and storage ordering comes from the left 
		/// operand and compile time column size from the right operand.
		/// </returns> ---------------------------------------------------------
		template <size_t rows, size_t cols, bool rowMajor> 
		[[nodiscard]] friend auto operator*(
			const matrix_type& lhs, 
			const Matrix<value_type, rows, cols, rowMajor>& rhs
		) requires std::is_arithmetic_v<value_type> {
			using result_type = Matrix<
				value_type, 
				matrix_type::rows_at_compile, 
				cols, 
				matrix_type::isRowMajor
			>;

			lhs.validateMultiplication(rhs);
			result_type result{};

			if constexpr (fixed_size_v<result_type>) 
				multiply(lhs, rhs, result.begin());
			else {
				result.reserve(Rows(lhs.numRows()), Columns(rhs.numColumns()));
				result.setDimensions(lhs.numRows(), rhs.numColumns());
				auto destination = collections::back_inserter(result.data());
				multiply(lhs, rhs, destination);
			}

			return result;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// ~~~ Unary Negation Operator ~~~
		/// 
		/// <para>
		/// Performs the negation of the matrix, as if by calling matrix * -1;
		/// </para></summary>
		/// 
		/// <returns>
		/// Return a new matrix with each of its elements as the negated values
		/// of this matrix.
		/// </returns> ---------------------------------------------------------
		matrix_type operator-() requires std::is_arithmetic_v<value_type> {
			matrix_type result{ *this };
			std::ranges::transform(result, result.begin(), std::negate{});
			return result;
		}

	private:

		struct EMPTY {}; // empty tag type for object size optimization

		using column_size = std::conditional_t<
			has_dynamic_columns_v<matrix_type>,
			size_type,
			EMPTY
		>;

		using row_size = std::conditional_t<
			has_dynamic_rows_v<matrix_type>,
			size_type,
			EMPTY
		>;

		array_type _array;

		[[no_unique_address, msvc::no_unique_address]]
		row_size _rows;

		[[no_unique_address, msvc::no_unique_address]]
		column_size	_columns;

		void init(std::initializer_list<value_type> list) {
			if (list.size() > this->size())
				throw std::invalid_argument("Initializer list is too long.");

			if constexpr (fixed_size_v<matrix_type>)
				collections::fill(_array, list);
			else {
				auto dest = collections::back_inserter(_array);
				collections::fill_n(dest, size(), list);
			}
		}

		void init(grid_initializer<value_type> grid) 
			requires multidimensional_v<matrix_type> 
		{
			validateGrid(grid);

			if constexpr (isRowMajor) 
				initRowMajor(grid);
			else 
				initColumnMajor(grid);
		}

		void validateGrid(grid_initializer<value_type> grid) {
			validateRowSize(grid.size(), "Initializer has invalid row size.");

			for (auto& row : grid) {
				validateColumnSize(
					row.size(), "Initializer has invalid column size.");
			}
		}

		void validateRowSize(size_type rows, auto error_msg) {
			if (rows != numRows())
				throw std::invalid_argument(error_msg);
		}

		void validateColumnSize(size_type columns, auto error_msg) {
			if (columns != numColumns())
				throw std::invalid_argument(error_msg);
		}

		void initRowMajor(grid_initializer<value_type> grid) 
			requires multidimensional_v<matrix_type>
		{
			size_type index = 0;

			for (auto& row : grid) {
				for (auto& scalar : row) 
					initElement(index++, scalar);
			}
		}

		void initColumnMajor(grid_initializer<value_type> grid) 
			requires multidimensional_v<matrix_type> 
		{
			size_type index = 0;

			for (size_type column = 0; column < numColumns(); ++column) {
				for (auto& row : grid) {
					auto scalar = *std::next(row.begin(), column);
					initElement(index++, scalar);
				}
			}
		}

		void initElement(size_type index, const_reference scalar) {
			if constexpr (fixed_size_v<matrix_type>)
				_array[index] = scalar;
			else
				_array.insertBack(scalar);
		}
		
		void setDimensions(size_type rows, size_type columns) {
			if constexpr (has_dynamic_rows_v<matrix_type>)
				_rows = rows;
			if constexpr (has_dynamic_columns_v<matrix_type>)
				_columns = columns;
		}

		template <
			size_t lrows,
			size_t lcols,
			bool lIsRowMajor,
			size_t rrows,
			size_t rcols,
			bool rIsRowMajor
		>
		static void elementWiseTransform(
			const Matrix<value_type, lrows, lcols, lIsRowMajor>& lhs, 
			const Matrix<value_type, rrows, rcols, rIsRowMajor>& rhs,
			auto destination,
			auto operation
		) {
			if constexpr (lIsRowMajor != rIsRowMajor) {
				auto lrange = lhs.flattenedRows();
				auto rrange = rhs.flattenedRows();
				std::ranges::transform(lrange, rrange, destination, operation);
			}
			else
				std::ranges::transform(lhs, rhs, destination, operation);
		}

		template <class T>
		static matrix_type add(const matrix_type& lhs, T&& rhs) {
			matrix_type result{ lhs };
			result += std::forward<T>(rhs);
			return result;
		}

		template <class T>
		static matrix_type subtract(const matrix_type& lhs, T&& rhs) {
			matrix_type result{ lhs };
			result -= std::forward<T>(rhs);
			return result;
		}

		template <
			size_t lrows,
			size_t lcols,
			bool lIsRowMajor,
			size_t rrows,
			size_t rcols,
			bool rIsRowMajor
		>
		static void multiply(
			const Matrix<value_type, lrows, lcols, lIsRowMajor>& lhs, 
			const Matrix<value_type, rrows, rcols, rIsRowMajor>& rhs,
			auto& destination
		) {
			// destination's storage order should always be the same as the lhs
			// - must calc and store each index of the result in the correct 
			// order either row-by-row first or column-by-column
			if constexpr (lIsRowMajor)
				onMultiply(lhs.rows(), rhs.columns(), destination);
			else
				onMultiply(rhs.columns(), lhs.rows(), destination);
		}

		static void onMultiply(const auto& lhs, const auto& rhs, auto& dest) {
			for (const auto vec1 : lhs) {
				for (const auto vec2 : rhs) {
					dest++ = std::transform_reduce(
						vec1.begin(), vec1.end(), vec2.begin(), value_type{}
					);
				}
			}
		}

		template <size_t rows, size_t cols, bool rowMajor> 
		constexpr void validateSameDimensions(
			const Matrix<value_type, rows, cols, rowMajor>& other
		) const {
			using other_type = Matrix<value_type, rows, cols, rowMajor>;

			if constexpr (
				has_dynamic_rows_v<matrix_type> || 
				has_dynamic_rows_v<other_type>
			) {
				if (other.numRows() != numRows())
					throw std::invalid_argument(ERR_UNEQUAL_DIMENSIONS);
			}
			else
				static_assert(rows_at_compile == rows, ERR_UNEQUAL_DIMENSIONS);

			if constexpr (
				has_dynamic_columns_v<matrix_type> ||
				has_dynamic_columns_v<other_type>
			) {
				if (other.numColumns() != numColumns())
					throw std::invalid_argument(ERR_UNEQUAL_DIMENSIONS);
			}
			else 
				static_assert(columns_at_compile == cols, ERR_UNEQUAL_DIMENSIONS);
		}

		template <size_t rows, size_t cols, bool rowMajor> 
		constexpr void validateMultiplication(
			const Matrix<value_type, rows, cols, rowMajor>& rhs
		) const {
			using other_type = Matrix<value_type, rows, cols, rowMajor>;

			if constexpr (
				!has_dynamic_columns_v<matrix_type> &&
				!has_dynamic_rows_v<other_type>
			) {
				static_assert(columns_at_compile == rows, ERR_MULTIPLICATION);
			}
			else {
				if (numColumns() != rhs.numRows())
					throw std::invalid_argument(ERR_MULTIPLICATION);
			}
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// MatrixIterator is a class that implements random access iteration 
		/// over a matrix in stable row-by-row or column-by-column order 
		/// independent of the matrix's storage ordering.
		/// </summary>
		/// 
		/// <typeparam name="isConst">
		/// Whether the iterator is a const_iterator (iterates over const 
		/// elements) or not.
		/// </typeparam> 
		/// 
		/// <typeparam name="isRowView">
		/// Indicates whether the iterator will visit elements row-by-row (true
		/// by default) or column by column.
		/// </typeparam> -------------------------------------------------------
		template <bool isConst, bool isRowMajorOrder>
		class MatrixIterator {
		public:

			using value_type = std::conditional_t<
				isConst, 
				const scalar_t, 
				scalar_t
			>;

			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::random_access_iterator_tag;

			MatrixIterator() = default;
			~MatrixIterator() = default;
			MatrixIterator(const MatrixIterator& copy) = default;
			MatrixIterator(MatrixIterator&& copy) = default;
			MatrixIterator& operator=(const MatrixIterator& other) = default;
			MatrixIterator& operator=(MatrixIterator&& other) = default;

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given MatrixIterator for implicit 
			/// conversion from non-const version to a const MatrixIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const MatrixIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided MatrixIterator to copy.
			/// </typeparam> ---------------------------------------------------
			template<
				bool wasConst, 
				class = std::enable_if_t<isConst && !wasConst>
			>
			MatrixIterator(MatrixIterator<wasConst, isRowMajorOrder> copy)
				: MatrixIterator(copy._matrix) {}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element in the matrix the iterator is
			/// pointing to in its current state.
			///	</returns> -----------------------------------------------------
			[[nodiscard]] reference operator*() const {
				return (*_matrix)[_row, _column];
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Arrow Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a pointer to the element in the matrix the iterator is
			/// pointing to in its current state.
			///	</returns> -----------------------------------------------------
			[[nodiscard]] pointer operator->() const {
				return &((*_matrix)[_row, _column]);
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Increments the iterator to the next element and returns the 
			/// iterator after updating.
			///	</returns> -----------------------------------------------------
			MatrixIterator& operator++() {
				(*this) += 1;
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Increments the iterator to the next element and returns a copy 
			/// of the iterator before updating.
			///	</returns> -----------------------------------------------------
			MatrixIterator operator++(int) {
				auto copy = *this;
				(*this) += 1;
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Decrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Decrements the iterator to the previous element and returns the 
			/// iterator after updating.
			///	</returns> -----------------------------------------------------
			MatrixIterator& operator--() {
				(*this) -= 1;
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Deccrement Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the previous element and returns a copy of 
			/// the iterator before updating.
			///	</returns> -----------------------------------------------------
			MatrixIterator operator--(int) {
				auto copy = *this;
				(*this) -= 1;
				return copy;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Addition Operator ~~~
			/// </summary>
			/// 
			/// <param name="n">
			/// The count or number of times to increment the iterator.
			/// </param>
			/// 
			/// <returns>
			/// Returns a reference to this iterator after incrementing it.
			/// </returns> -----------------------------------------------------
			MatrixIterator& operator+=(size_type n) {
				if constexpr (isRowMajorOrder) {
					size_type numColumns = _matrix->numColumns();
					size_type total = (_row * numColumns) + _column + n;
					_row = total / numColumns;
					_column = total % numColumns;
				}
				else {
					size_type numRows = _matrix->numRows();
					size_type total = (_column * numRows) + _row + n;
					_row = total % numRows;
					_column = total / numRows;
				}
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			///  ~~~ Addition Operator ~~~
			/// </summary>
			/// 
			/// <param name="lhs">
			/// The iterator being incremented.
			/// </param>
			/// 
			/// <param name="n">
			/// The count or number of times to increment the iterator.
			/// </param>
			/// 
			/// <returns>
			/// Returns a new iterator from the result of incrementing the given
			/// iterator the specified number of times.
			/// </returns> -----------------------------------------------------
			[[nodiscard]] friend MatrixIterator operator+(
				const MatrixIterator& lhs, 
				size_type n
			) {
				MatrixIterator result = lhs;
				result += n;
				return result;
			}

			// -----------------------------------------------------------------
			/// <summary>
			///  ~~~ Addition Operator ~~~
			/// </summary>
			/// 
			/// <param name="n">
			/// The count or number of times to increment the iterator.
			/// </param>
			/// 
			/// <param name="rhs">
			/// The iterator being incremented.
			/// </param>
			/// 
			/// <returns>
			/// Returns a new iterator from the result of incrementing the given
			/// iterator the specified number of times.
			/// </returns> -----------------------------------------------------
			[[nodiscard]] friend MatrixIterator operator+(
				size_type n,
				const MatrixIterator& rhs
			) {
				MatrixIterator result = rhs;
				result += n;
				return result;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Subraction Operator ~~~
			/// </summary>
			/// 
			/// <param name="n">
			/// The count or number of times to decrement the iterator.
			/// </param>
			/// 
			/// <returns>
			/// Returns a reference to this iterator after decrementing it.
			/// </returns> -----------------------------------------------------
			MatrixIterator& operator-=(size_type n) {
				if constexpr (isRowMajorOrder) {
					size_type numColumns = _matrix->numColumns();
					size_type total = (_row * numColumns) + _column - n;
					_row = total / numColumns;
					_column = total % numColumns;
				}
				else {
					size_type numRows = _matrix->numRows();
					size_type total = (_column * numRows) + _row - n;
					_row = total % numRows;
					_column = total / numRows;
				}
				return *this;
			}

			// -----------------------------------------------------------------
			/// <summary>
			///  ~~~ Subraction Operator ~~~
			/// </summary>
			/// 
			/// <param name="lhs">
			/// The iterator being decremented.
			/// </param>
			/// 
			/// <param name="n">
			/// The count or number of times to decrement the iterator.
			/// </param>
			/// 
			/// <returns>
			/// Returns a new iterator from the result of decrementing the given
			/// iterator the specified number of times.
			/// </returns> -----------------------------------------------------
			[[nodiscard]] friend MatrixIterator operator-(
				const MatrixIterator& lhs, 
				size_type n
			) {
				MatrixIterator result = lhs;
				result -= n;
				return result;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Returns the element at the specified index from where the
			/// iterator points in the matrix.
			/// </summary>
			/// 
			/// <param name="index">
			/// The index of the element to access.
			/// </param>
			/// 
			/// <returns>
			/// Returns the element at index distance from the current location
			/// pointed to by the iterator, as if by calling *(iter + index).
			/// </returns> -----------------------------------------------------
			[[nodiscard]] reference operator[](size_type index) const {
				return *(*this + index);
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Returns the comparison of the specified iterators.
			/// </summary>
			/// 
			/// <param name="lhs">
			/// The iterator being compared on the left.
			/// </param>
			/// 
			/// <param name="rhs">
			/// The iterator being compared on the right.
			/// </param>
			/// 
			/// <returns>
			/// Returns the ordering of the addresses pointed to by the 
			/// specified iterators. This is undefined if the underlying matrix
			/// for each is not the same object.
			/// </returns> -----------------------------------------------------
			[[nodiscard]] friend auto operator<=>(
				const MatrixIterator& lhs,
				const MatrixIterator& rhs
			) {
				return std::addressof(*lhs) <=> std::addressof(*rhs);
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Computes the difference between the specified iterators, or the
			/// size of the range between them.
			/// </summary>
			/// 
			/// <param name="lhs">
			/// The iterator being subtracted from on the left.
			/// </param>
			/// 
			/// <param name="rhs">
			/// The iterator being subtracted on the right.
			/// </param>
			/// 
			/// <returns>
			/// Returns the direct difference in addresses pointed to by the
			/// iterators.
			/// </returns> -----------------------------------------------------
			[[nodiscard]] friend difference_type operator-(
				const MatrixIterator& lhs, 
				const MatrixIterator& rhs
			) {
				return std::addressof(*lhs) - std::addressof(*rhs);
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The MatrixIterator appearing on the left-hand side of the 
			/// '==' operator.
			/// </param>
			/// <param name="rhs">
			/// The MatrixIterator appearing on the right-hand side of the 
			/// '==' operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the MatrixIterators are both pointing to the
			/// same element, false otherwise.
			///	</returns> -----------------------------------------------------
			[[nodiscard]] friend bool operator==(
				const MatrixIterator& lhs,
				const MatrixIterator& rhs
			) {
				return lhs._matrix == rhs._matrix && 
					   lhs._row == rhs._row && 
					   lhs._column == rhs._column;
			}

		private:

			using matrix_ptr = std::conditional_t<
				isConst, 
				const matrix_type*,
				matrix_type*
			>;

			matrix_ptr _matrix;
			size_type _row;
			size_type _column;

			explicit MatrixIterator(
				matrix_ptr m, 
				size_type row,
				size_type column
			) : _matrix(m), _row(row), _column(column) {
			
			}

			friend class matrix_type;
		};

		static_assert(
			std::random_access_iterator<MatrixIterator<false, false>>,
			"MatrixIterator does not meet the requirements of a random access iterator."
		);

		// ---------------------------------------------------------------------
		/// <summary>
		/// FlatView implements the std::view interface for matricies
		/// by iterating over elements in either row by row ordering or column-
		/// by-column order independent of the matrix's internal storage order.
		/// </summary> 
		/// 
		/// <typeparam name="isConst">
		/// Whether the view is a const and allows changing the elements it]
		/// iterates over or not.
		/// </typeparam>
		/// 
		/// <typeparam name="isRowView">
		/// Boolean indicating the iteration order of the iterators returned by
		/// the adapater. True indicates a view iterating row-by-row and false
		/// indicates column-by-colmun order.
		/// </typeparam> -------------------------------------------------------
		template <bool isConst, bool isRowMajorOrder>
		class FlatView {
		public:

			using iterator = std::conditional_t<
				isConst,
				MatrixIterator<true, isRowMajorOrder>,
				MatrixIterator<false, isRowMajorOrder>
			>;

			// -----------------------------------------------------------------
			/// <summary>
			/// Returns the begin iterator of the view.
			/// </summary>
			/// 
			/// <typeparam name="T">
			/// Universal reference for this type (FlatView).
			/// </typeparam>
			/// 
			/// <param name="self">
			/// Auto-deduced 'this' parameter.
			/// </param>
			/// 
			/// <returns>
			/// Returns the begin iterator, or an iterator to the first element
			/// in the matrix at index [0, 0].
			/// </returns> -----------------------------------------------------
			template <class T>
			[[nodiscard]] iterator begin(this T&& self) {
				return iterator{ std::forward<T>(self)._matrix, 0, 0 };
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Returns the end iterator of the view.
			/// </summary>
			/// 
			/// <typeparam name="T">
			/// Universal reference for this type (FlatView).
			/// </typeparam>
			/// 
			/// <param name="self">
			/// Auto-deduced 'this' parameter.
			/// </param>
			/// 
			/// <returns>
			/// Returns the end iterator for the view, or the position at index
			/// [n, 0] or (depending on iteration order) [0, m] - past the end 
			/// of the matrix bounds one element after the last element in the
			/// matrix.
			/// </returns> -----------------------------------------------------
			template <class T>
			[[nodiscard]] iterator end(this T&& self) {
				if constexpr (isRowMajorOrder) {
					size_type maxRow = self._matrix->numRows();
					return iterator{ std::forward<T>(self)._matrix, maxRow, 0 };
				}
				else {
					size_type maxColumn = self._matrix->numColumns();
					return iterator{ std::forward<T>(self)._matrix, 0, maxColumn };
				}
			}

		private:

			using matrix_ptr = std::conditional_t<
				isConst, 
				const matrix_type*,
				matrix_type*
			>;

			friend class matrix_type;
			matrix_ptr _matrix;
			FlatView(matrix_ptr m) : _matrix(m) {}
		};

		static_assert(
			std::ranges::random_access_range<FlatView<true, false>>, 
			"FlatView does not meet the requirements of a random access range."
		);

		// ---------------------------------------------------------------------
		/// <summary>
		/// RowView implements a view over matrix rows as a range of row vectors 
		/// copied from the original matrix.
		/// </summary> ---------------------------------------------------------
		class RowView {
		public:

			// -----------------------------------------------------------------
			/// <summary>
			/// Returns the first row in the matrix.
			/// </summary>
			/// 
			/// <typeparam name="T">
			/// Universal reference for this type (RowView).
			/// </typeparam>
			/// 
			/// <param name="self">
			/// Auto-deduced 'this' parameter.
			/// </param>
			/// 
			/// <returns>
			/// Returns a forward_iterator to the first row in the matrix.
			/// </returns> -----------------------------------------------------
			template <class T>
			[[nodiscard]] auto begin(this T&& self) {
				return std::forward<T>(self)._rows.begin();
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Return the end of the matrix rows.
			/// </summary>
			/// 
			/// <typeparam name="T">
			/// Universal reference for this type (RowView).
			/// </typeparam>
			/// 
			/// <param name="self">
			/// Auto-deduced 'this' parameter.
			/// </param>
			/// 
			/// <returns>
			/// Returns a sentinel marking the end of the matrix rows.
			/// </returns> -----------------------------------------------------
			template <class T>
			[[nodiscard]] auto end(this T&& self) {
				return std::forward<T>(self)._rows.end();
			}

		private:

			DynamicArray<row_type> _rows;

			RowView(const matrix_type& m) : _rows(Reserve(m.numRows())) {
				for (size_type i = 0; i < m.numRows(); ++i) 
					_rows.insertBack(m.row(i));
			}

			friend class matrix_type;
		};

		// ---------------------------------------------------------------------
		/// <summary>
		/// ColumnView implements a view over matrix columns as a range of 
		/// column vectors copied from the original matrix.
		/// </summary> ---------------------------------------------------------
		class ColumnView {
		public:

			using value_type = column_type;

			// -----------------------------------------------------------------
			/// <summary>
			/// Returns the first columns in the matrix.
			/// </summary>
			/// 
			/// <typeparam name="T">
			/// Universal reference for this type (ColumnView).
			/// </typeparam>
			/// 
			/// <param name="self">
			/// Auto-deduced 'this' parameter.
			/// </param>
			/// 
			/// <returns>
			/// Returns a forward_iterator to the first columns in the matrix.
			/// </returns> -----------------------------------------------------
			template <class T>
			[[nodiscard]] auto begin(this T&& self) {
				return std::forward<T>(self)._columns.begin();
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Return the end of the matrix columns.
			/// </summary>
			/// 
			/// <typeparam name="T">
			/// Universal reference for this type (ColumnView).
			/// </typeparam>
			/// 
			/// <param name="self">
			/// Auto-deduced 'this' parameter.
			/// </param>
			/// 
			/// <returns>
			/// Returns a sentinel marking the end of the matrix columns.
			/// </returns> -----------------------------------------------------
			template <class T>
			[[nodiscard]] auto end(this T&& self) {
				return std::forward<T>(self)._columns.end();
			}

		private:

			DynamicArray<column_type> _columns;

			ColumnView(const matrix_type& m) : _columns(Reserve(m.numColumns())) {
				for (size_type i = 0; i < m.numColumns(); ++i) 
					_columns.insertBack(m.column(i));
			}

			friend class matrix_type;
		};

		// ---------------------------------------------------------------------
		/// <summary>
		/// Defines a list of dimension indicies to include from a parent matrix
		/// as either a whitelist or blacklist when constructing sub-matricies.
		/// </summary> ---------------------------------------------------------
		struct SubmatrixDimension {
			DynamicArray<size_type> indicies;
			bool isBlacklist;
		};

		// ---------------------------------------------------------------------
		/// <summary>
		/// Defines the list of rows and columns to include/exclude when
		/// constructing a submatrix.
		/// </summary> ---------------------------------------------------------
		struct SubmatrixShape {
			SubmatrixDimension rows;
			SubmatrixDimension columns;
		};

		// ---------------------------------------------------------------------
		/// <summary>
		/// SubmatrixFactory is a class designed to build smaller sub-matricies
		/// from other matricies given a filter of rows and columns to 
		/// include/exclude from the result.
		/// </summary> ---------------------------------------------------------
		class SubmatrixFactory {
		public:

			// -----------------------------------------------------------------
			/// <summary>
			/// 
			/// </summary>
			/// 
			/// <param name="from">
			/// 
			/// </param>
			/// 
			/// <param name="shape">
			/// 
			/// </param>
			/// 
			/// <returns>
			/// 
			/// </returns> -----------------------------------------------------
			[[nodiscard]] static auto create(
				const matrix_type& from, 
				const SubmatrixShape& shape
			) {
				using submatrix_type = 
					Matrix<value_type, DYNAMIC, DYNAMIC, isRowMajor>;

				submatrix_type result{};
				create(from, shape, result);
				return result;
			}

			// -----------------------------------------------------------------
			/// <summary>
			/// Sets the specified output matrix equal to a submatrix created 
			/// from the given parent and submatrix shape. Use this method to 
			/// control the type of the submatrix produced if the desired 
			/// submatrix should not be fully dynamic.
			/// </summary>
			/// 
			/// <typeparam name="submatrix_type">
			/// The type of the matrix to be assigned.
			/// </typeparam>
			/// 
			/// <param name="from">
			/// The parent matrix to create the submatrix from.
			/// </param>
			/// 
			/// <param name="shape">
			/// The list of rows and columns to build the submatrix from.
			/// </param>
			/// 
			/// <param name="out">
			/// The output parameter being assigned.
			/// </param> -------------------------------------------------------
			template <class submatrix_type>
			static void create(
				const matrix_type& from, 
				const SubmatrixShape& shape,
				submatrix_type& out

			) {
				size_type rows = calcSubmatrixDim(from.numRows(), shape.rows);
				size_type cols = calcSubmatrixDim(from.numColumns(), shape.columns);

				if constexpr (partially_dynamic_v<submatrix_type>) {
					out.reserve(Rows(rows), Columns(cols));
					out.setDimensions(rows, cols);
					auto destination = collections::back_inserter(out._array);
					copyToSubmatrix(from, shape, out, destination);
				}

				copyToSubmatrix(from, shape, out, out.begin());
			}

		private:

			static size_type calcSubmatrixDim(
				size_type parentSize, 
				const SubmatrixDimension& list
			) {
				if (list.isBlacklist)
					return parentSize - list.indicies.size();
				else if (list.indicies.isEmpty())
					return parentSize;
				else
					return list.indicies.size();
			}

			static bool isIncludedInSubmatrix(
				size_type index, 
				const SubmatrixDimension& filter
			) {
				bool isEmpty = filter.indicies.isEmpty();
				bool isPresent = collections::contains(filter.indicies, index);

				return isEmpty || (isPresent && !filter.isBlacklist) || 
					(!isPresent && filter.isBlacklist);
			}

			template <class submatrix_type>
			static void copyToSubmatrix(
				const matrix_type& from,
				const SubmatrixShape& shape,
				const submatrix_type& submatrix,
				auto dest
			) {
				if constexpr (submatrix_type::isRowMajor)
					copyToRowOrderSubmatrix(from, shape, dest);
				else
					copyToColumnOrderSubmatrix(from, shape, dest);
			}

			static void copyToRowOrderSubmatrix(
				const matrix_type& from,
				const SubmatrixShape& shape,
				auto destination
			) {
				for (size_type i = 0; i < from.numRows(); ++i) {
					if (isIncludedInSubmatrix(i, shape.rows)) {
						for (size_type j = 0; j < from.numColumns(); ++j) {
							if (isIncludedInSubmatrix(j, shape.columns)) 
								*destination++ = from[i, j];
						}
					}
				}
			}

			static void copyToColumnOrderSubmatrix(
				const matrix_type& from,
				const SubmatrixShape& shape,
				auto destination
			) {
				for (size_type j = 0; j < from.numColumns(); ++j) {
					if (isIncludedInSubmatrix(j, shape.columns)) {
						for (size_type i = 0; i < from.numRows(); ++i) {
							if (isIncludedInSubmatrix(i, shape.rows)) 
								*destination++ = from[i, j];
						}
					}
				}
			}
		};
	};

	static_assert(
		std::ranges::forward_range<Matrix<int, DYNAMIC, DYNAMIC, false>::RowView>, 
		"RowView does not meet the requirements of a forward range."
	);

	static_assert(
		std::ranges::forward_range<Matrix<int, DYNAMIC, DYNAMIC, true>::ColumnView>, 
		"ColumnView does not meet the requirements of a forward range."
	);
	
	// ------------------------ Convienience Aliases ------------------------ //

	template <class scalar, bool isRowMajorOrder = true>
	using DynamicMatrix = Matrix<scalar, DYNAMIC, DYNAMIC, isRowMajorOrder>;

	template <class scalar, int64_t rows>
	using Vector = Matrix<scalar, rows, 1>;

	template <class scalar>
	using DynamicVector = Vector<scalar, DYNAMIC>;

	using Vector2i = Vector<int32_t, 2>;

	using Vector3i = Vector<int32_t, 3>;

	using Vector4i = Vector<int32_t, 4>;

	using Vector2f = Vector<float, 2>;

	using Vector3f = Vector<float, 3>;

	using Vector4f = Vector<float, 4>;

	using Vector2d = Vector<double, 2>;

	using Vector3d = Vector<double, 3>;

	using Vector4d = Vector<double, 4>;
	
	template <class scalar, int64_t columns>
	using RowVector = Matrix<scalar, 1, columns>;

	template <class scalar>
	using DynamicRowVector = RowVector<scalar, DYNAMIC>;

	using RowVector2i = RowVector<int32_t, 2>;

	using RowVector3i = RowVector<int32_t, 3>;

	using RowVector4i = RowVector<int32_t, 4>;

	using RowVector2f = RowVector<float, 2>;

	using RowVector3f = RowVector<float, 3>;

	using RowVector4f = RowVector<float, 4>;

	using RowVector2d = RowVector<double, 2>;

	using RowVector3d = RowVector<double, 3>;

	using RowVector4d = RowVector<double, 4>;
}