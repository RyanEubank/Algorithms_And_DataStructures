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
		requires sequential_collection<typename params::collection_t>
	class SequentialCollectionTests : public CollectionTests<params> {
	protected:

		using collection = typename params::collection_t;
		using element = typename params::element_t; 
		using iterator = typename std::initializer_list<element>::iterator;

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given object and expected sequence match 
		/// size/elements.
		/// </summary>
		/// 
		/// <param name="obj">
		/// The actual object under test.
		/// </param>
		/// 
		/// <param name="expected">
		/// The expected result.
		/// </param> ----------------------------------------------------------
		void testObjectEqualsExpectedSequence(
			const auto& obj, 
			const auto& expected
		) {
			EXPECT_EQ(obj.size(), expected.size());

			auto it = obj.begin();

			for (const auto& e : expected)
				EXPECT_EQ(*it++,  e);
		}

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
			std::function<void(collection&, const element&)> func
		) {
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto expected = { d, a, b, c };
			func(obj, d);

			testObjectEqualsExpectedSequence(obj, expected);
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
			std::function<void(collection&, const element&)> func
		) {
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto expected = { a, b, c, d };
			func(obj, d);

			testObjectEqualsExpectedSequence(obj, expected);
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
			std::function<void(collection&, const element&)> func
		) {
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto expected = { a, d, b, c };
			func(obj, d);

			testObjectEqualsExpectedSequence(obj, expected);
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
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { d, e, f, a, b, c };
			func(obj, new_elements.begin(), new_elements.end());

			testObjectEqualsExpectedSequence(obj, expected);
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
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { a, b, c, d, e, f };
			func(obj, new_elements.begin(), new_elements.end());

			testObjectEqualsExpectedSequence(obj, expected);
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
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { a, d, e, f, b, c };
			func(obj, new_elements.begin(), new_elements.end());

			testObjectEqualsExpectedSequence(obj, expected);
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
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto expected = { b, c };
			func(obj);

			testObjectEqualsExpectedSequence(obj, expected);
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
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto expected = { a, b };
			func(obj);

			testObjectEqualsExpectedSequence(obj, expected);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a list.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		void testMethodRemovesMiddleElement(
			std::function<void(collection&)> func
		) {
			DECLARE_TEST_DATA();

			collection obj{ a, b, c };
			auto expected = { a, c };
			func(obj);

			testObjectEqualsExpectedSequence(obj, expected);
		}
	};
}