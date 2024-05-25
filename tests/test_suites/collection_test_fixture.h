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

#include "../test_data/test_inputs.h"
#include "algorithms/collection_algorithms.h"

#define FORWARD_TEST_TYPES()							\
	using element = typename TypeParam::element_t;		\
	using collection = typename TypeParam::collection_t

#define DECLARE_TEST_DATA()								\
	const auto& a = test_data<element>.a;				\
	const auto& b = test_data<element>.b;				\
	const auto& c = test_data<element>.c;				\
	const auto& d = test_data<element>.d;				\
	const auto& e = test_data<element>.e;				\
	const auto& f = test_data<element>.f 

namespace collection_tests {

	using namespace collections;

	template<class params> requires collection<typename params::collection_t>
	class CollectionTests : public testing::Test {
	protected:
		using collection = typename params::collection_t;
		using element = typename params::element_t;

		// --------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method returns an iterator to the expected
		/// element.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> ----------------------------------------------------------
		template <std::ranges::input_range c = collection>
		void testMethodReturnsIteratorToExpectedElement(
			std::function<typename c::iterator(collection&)> func,
			std::function<element(collection&)> expected
		) {
			DECLARE_TEST_DATA();

			collection obj{ a, b, c, d, e, f };
			auto exp = expected(obj);
			auto result = func(obj);

			auto eee = exp;
			auto r = *result;

			EXPECT_EQ(*result, exp);
		}
	};
}