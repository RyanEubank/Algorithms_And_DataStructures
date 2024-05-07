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

#include "collection_tests.h"
#include "algorithms/collection_algorithms.h"
#include "concepts/sequential.h"

namespace collection_tests {

	template<class params> requires sequential<typename params::collection_t>
	class SequentialCollectionTests : public CollectionTests<params> {
	protected:

		using collection = typename params::collection_t;
		using element = typename params::element_t; 
		using iterator = typename std::array<element, 3>::iterator;
		using collection_iterator = typename collection::iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places elements at the front of the
		/// list.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodPlacesElementFirst(
			std::function<void(collection&, element&)> func
		) {
			auto input = this->testInput.control();
			auto value = this->testInput.different_elements()[0];

			collection obj(collections::from_range, input);

			ASSERT_NE(obj.front(), value);
			func(obj, value);
			EXPECT_EQ(obj.front(), value);

			EXPECT_EQ(obj.size(), input.size() + 1);
			EXPECT_EQ(obj[1], input[0]);
			EXPECT_EQ(obj[2], input[1]);
			EXPECT_EQ(obj[3], input[2]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places elements at the back of the
		/// list.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodPlacesElementLast(
			std::function<void(collection&, element&)> func
		) {
			auto input = this->testInput.control();
			auto value = this->testInput.different_elements()[0];

			collection obj(collections::from_range, input);

			ASSERT_NE(obj.back(), value);
			func(obj, value);
			EXPECT_EQ(obj.back(), value);

			EXPECT_EQ(obj.size(), input.size() + 1);
			EXPECT_EQ(obj[0], input[0]);
			EXPECT_EQ(obj[1], input[1]);
			EXPECT_EQ(obj[2], input[2]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places elements at a specific iterator
		/// position.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodPlacesElementInTheMiddle(
			std::function<void(collection&, element&)> func
		) {
			auto input = this->testInput.control();
			auto value = this->testInput.different_elements()[0];

			collection obj(collections::from_range, input);

			func(obj, value);

			EXPECT_EQ(obj.size(), input.size() + 1);
			EXPECT_EQ(obj[0], input[0]);
			EXPECT_EQ(obj[1], value);
			EXPECT_EQ(obj[2], input[1]);
			EXPECT_EQ(obj[3], input[2]);
		}

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
			auto input = this->testInput.control();

			collection obj1(collections::from_range, input);
			collection obj2(collections::from_range, input);

			auto safe = Index(safeIndex(obj1));
			auto unsafe = Index(unsafeIndex(obj2));

			EXPECT_NO_THROW(func(obj1, safe));
			EXPECT_THROW(func(obj2, unsafe), std::out_of_range);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places a range at the beginning of the
		/// collection.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------]
		void testMethodPlacesRangeAtBeginning(
			std::function<void(collection&, iterator, iterator)> func
		) {
			auto existing_elements = this->testInput.control();
			auto new_elements = this->testInput.different_elements();

			collection obj(collections::from_range, existing_elements);
			for (auto i = 0; i < new_elements.size(); ++i)
				ASSERT_EQ(collections::find(obj, new_elements[i]), obj.end());

			func(obj, new_elements.begin(), new_elements.end());

			EXPECT_EQ(obj[0], new_elements[0]);
			EXPECT_EQ(obj[1], new_elements[1]);
			EXPECT_EQ(obj[2], new_elements[2]);
			EXPECT_EQ(obj[3], existing_elements[0]);
			EXPECT_EQ(obj[4], existing_elements[1]);
			EXPECT_EQ(obj[5], existing_elements[2]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places a range at the end of the
		/// collection.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodPlacesRangeAtEnd(
			std::function<void(collection&, iterator, iterator)> func
		) {
			auto existing_elements = this->testInput.control();
			auto new_elements = this->testInput.different_elements();

			collection obj(collections::from_range, existing_elements);
			for (auto i = 0; i < new_elements.size(); ++i)
				ASSERT_EQ(collections::find(obj, new_elements[i]), obj.end());

			func(obj, new_elements.begin(), new_elements.end());

			EXPECT_EQ(obj[0], existing_elements[0]);
			EXPECT_EQ(obj[1], existing_elements[1]);
			EXPECT_EQ(obj[2], existing_elements[2]);
			EXPECT_EQ(obj[3], new_elements[0]);
			EXPECT_EQ(obj[4], new_elements[1]);
			EXPECT_EQ(obj[5], new_elements[2]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places a range in the middle of the 
		/// collection.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodPlacesRangeInTheMiddle(
			std::function<void(collection&, iterator, iterator)> func
		) {
			auto existing_elements = this->testInput.control();
			auto new_elements = this->testInput.different_elements();

			collection obj(collections::from_range, existing_elements);
			for (auto i = 0; i < new_elements.size(); ++i)
				ASSERT_EQ(collections::find(obj, new_elements[i]), obj.end());

			func(obj, new_elements.begin(), new_elements.end());

			EXPECT_EQ(obj[0], existing_elements[0]);
			EXPECT_EQ(obj[1], new_elements[0]);
			EXPECT_EQ(obj[2], new_elements[1]);
			EXPECT_EQ(obj[3], new_elements[2]);
			EXPECT_EQ(obj[4], existing_elements[1]);
			EXPECT_EQ(obj[5], existing_elements[2]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method returns an iterator to the expected
		/// element.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodReturnsIteratorToExpectedElement(
			std::function<collection_iterator(collection&)> func,
			element expected
		) {
			using collection = typename params::collection_t;
			auto input = this->testInput.control();

			collection obj(collections::from_range, input);

			auto result = func(obj);
			EXPECT_EQ(*result, expected);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a list.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodRemovesFirstElement(
			std::function<void(collection&)> func
		) {
			auto input = this->testInput.control();
			collection obj(collections::from_range, input);
			auto first_element = obj.front();

			func(obj);

			EXPECT_EQ(collections::find(obj, first_element), obj.end());
			EXPECT_EQ(obj.size(), input.size() - 1);
			EXPECT_EQ(obj[0], input[1]);
			EXPECT_EQ(obj[1], input[2]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a list.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodRemovesLastElement(
			std::function<void(collection&)> func
		) {
			auto input = this->testInput.control();
			collection obj(collections::from_range, input);
			auto last_element = obj.back();

			obj.removeBack();

			EXPECT_EQ(collections::find(obj, last_element), obj.end());
			EXPECT_EQ(obj.size(), input.size() - 1);
			EXPECT_EQ(obj[0], input[0]);
			EXPECT_EQ(obj[1], input[1]);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method throws an exception when called on an
		/// empty object.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodThrowsExceptionOnEmptyObject(
			std::function<void(collection&)> func
		) {
			collection obj{};
			ASSERT_TRUE(obj.isEmpty());
			EXPECT_THROW(func(obj), std::exception());
		}
	};
}