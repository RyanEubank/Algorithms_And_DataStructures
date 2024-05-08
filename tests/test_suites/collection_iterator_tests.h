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

#include <list>

#include "collection_tests.h"

namespace collection_tests {

	template <class params>
	class CollectionIteratorTests : public CollectionTests<params> {};

	TYPED_TEST_SUITE_P(CollectionIteratorTests);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionIteratorTests, IteratorsCoverFullRange) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);

		size_t i = 0;
		for (auto e = obj.begin(); e != obj.end(); ++e)
			EXPECT_EQ(*e, input[i++]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the const_iterator pair in the range covers the full range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionIteratorTests, ConstIteratorsCoverFullRange) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);

		size_t i = 0;
		for (auto e = obj.cbegin(); e != obj.cend(); ++e)
			EXPECT_EQ(*e, input[i++]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the reverse_iterator pair in the range covers the full 
	/// range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionIteratorTests, ReverseIteratorsCoverFullRange) {
		FORWARD_TEST_TYPES;

		if constexpr (!bidirectional_ranged_collection<collection>)
			GTEST_SKIP();

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);

		size_t i = input.size() - 1;
		for (auto e = obj.rbegin(); e != obj.rend(); ++e)
			EXPECT_EQ(*e, input[i--]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionIteratorTests, ReverseIteratorsCoverReverseRange) {
		FORWARD_TEST_TYPES;

		if constexpr (!bidirectional_ranged_collection<collection>)
			GTEST_SKIP();

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);
		std::list<element> reversed_order;

		for (auto e : obj)
			reversed_order.push_front(e);

		auto actual = obj.rbegin();
		auto expected = reversed_order.begin();

		while (actual != obj.rend())
			EXPECT_EQ(*actual++, *expected++);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionIteratorTests, ConstIteratorsMatchNonConst) {
		FORWARD_TEST_TYPES;

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);
		auto it = obj.begin();
		auto c_it = obj.cbegin();

		while (it != obj.end()) 
			EXPECT_EQ(*it++, *c_it++);
		EXPECT_EQ(obj.end(), obj.cend());
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin/end and cbegin/cend methods cover the same 
	/// elements.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(CollectionIteratorTests, ConstReverseIteratorsMatchNonConst) {
		FORWARD_TEST_TYPES;

		if constexpr (!bidirectional_ranged_collection<collection>)
			GTEST_SKIP();

		auto input = this->_test_data.control();
		const collection obj(collections::from_range, input);

		auto rit = obj.rbegin();
		auto crit = obj.crbegin();

		while (rit != obj.rend()) 
			EXPECT_EQ(*rit++, *crit++);
		EXPECT_EQ(obj.rend(), obj.crend());
	}

	REGISTER_TYPED_TEST_SUITE_P(
		CollectionIteratorTests,
		IteratorsCoverFullRange,
		ConstIteratorsCoverFullRange,
		ReverseIteratorsCoverFullRange,
		ReverseIteratorsCoverReverseRange,
		ConstIteratorsMatchNonConst,
		ConstReverseIteratorsMatchNonConst
	);
}