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

#include <cstdlib>
#include <initializer_list>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "concepts/associative.h"
#include "concepts/collection.h"
#include "concepts/map.h"
#include "concepts/sequential.h"
#include "util/types.h"

#include "test_inputs.h"

#define FORWARD_TEST_TYPES()												\
	using test_fixture				= CollectionTest<TypeParam>;			\
	using collection_type			= test_fixture::collection_type;		\
	using value_type				= test_fixture::value_type;				\
	using key_type					= test_fixture::key_type;				\
	using mapped_type				= test_fixture::mapped_type;			\
	using allocator_type			= test_fixture::allocator_type;			\
	using size_type					= test_fixture::size_type;				\
	using difference_type			= test_fixture::difference_type;		\
	using reference					= test_fixture::reference;				\
	using const_reference			= test_fixture::const_reference;		\
	using pointer					= test_fixture::pointer;				\
	using const_pointer				= test_fixture::const_pointer;			\
	using iterator					= test_fixture::iterator;				\
	using const_iterator			= test_fixture::const_iterator;			\
	using reverse_iterator			= test_fixture::reverse_iterator;		\
	using const_reverse_iterator	= test_fixture::const_reverse_iterator;	\
	using count_list				= std::initializer_list<std::pair<value_type, int>>

#define DECLARE_TEST_DATA()												\
	const_reference a				= test_data<value_type>.a;			\
	const_reference b				= test_data<value_type>.b;			\
	const_reference c				= test_data<value_type>.c;			\
	const_reference d				= test_data<value_type>.d;			\
	const_reference e				= test_data<value_type>.e;			\
	const_reference f				= test_data<value_type>.f;			\
	const_reference g				= test_data<value_type>.g;			\
	const_reference h				= test_data<value_type>.h;			\
	const_reference i				= test_data<value_type>.i;			\
	const_reference j				= test_data<value_type>.j	

namespace collection_tests {

	using namespace collections;


	template <collection T>
	class CollectionTest : public testing::Test {
	protected:

		template<class U>
		struct conditional_traits {
			using reverse_iterator = void;
			using const_reverse_iterator = void;
			using key_type = void;
			using mapped_type = void;
		};

		template<class U> 
			requires (std::ranges::bidirectional_range<U> && !associative<U>)
		struct conditional_traits<U> {
			using reverse_iterator = U::reverse_iterator;
			using const_reverse_iterator = U::reverse_iterator;
			using key_type = void;
			using mapped_type = void;
		};

		template<class U> 
			requires (!std::ranges::bidirectional_range<U> && associative<U>)
		struct conditional_traits<U> {
			using reverse_iterator = void;
			using const_reverse_iterator = void;
			using key_type = U::key_type;
			using mapped_type = U::key_type;
		};

		template<class U> 
			requires (std::ranges::bidirectional_range<U> && associative<U>)
		struct conditional_traits<U> {
			using reverse_iterator = U::reverse_iterator;
			using const_reverse_iterator = U::reverse_iterator;
			using key_type = U::key_type;
			using mapped_type = U::key_type;
		};

		using collection_type	= T;
		using value_type		= collection_type::value_type;
		using allocator_type	= collection_type::allocator_type;
		using size_type			= collection_type::size_type;		
		using difference_type	= collection_type::difference_type;
		using reference			= collection_type::reference;		
		using const_reference	= collection_type::const_reference;
		using pointer			= collection_type::pointer;		
		using const_pointer		= collection_type::const_pointer;
		using iterator			= collection_type::iterator;
		using const_iterator	= collection_type::const_iterator;

		using reverse_iterator 
			= conditional_traits<collection_type>::reverse_iterator;

		using const_reverse_iterator 
			= conditional_traits<collection_type>::const_reverse_iterator;

		using key_type 
			= conditional_traits<collection_type>::key_type;

		using mapped_type 
			= conditional_traits<collection_type>::mapped_type;

		DECLARE_TEST_DATA();

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given iterator range covers the expected sequence.
		/// </summary>
		/// 
		/// <param name="begin">
		/// The beginning iterator to the range under test.
		/// </param>
		/// 
		/// <param name="end">
		/// The end iterator to the range under test.
		/// </param>
		/// 
		/// <param name="expected">
		/// The expected sequence.
		/// </param> -----------------------------------------------------------
		template <std::input_iterator iter, std::sentinel_for<iter> sentinel>
		testing::AssertionResult expectSequence(
			iter begin, 
			sentinel end, 
			auto expected
		) {
			if (begin == end)
				return testing::AssertionFailure() << "Actual range is empty.";

			for (const auto& e : expected)
				EXPECT_EQ(*begin++,  e);

			EXPECT_EQ(begin, end) << "Expected and actual range differ in size.";
			return testing::AssertionSuccess();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given iterator range covers the expected sequence.
		/// </summary>
		/// 
		/// <param name="begin">
		/// The beginning iterator to the range under test.
		/// </param>
		/// 
		/// <param name="end">
		/// The end iterator to the range under test.
		/// </param>
		/// 
		/// <param name="expected">
		/// The expected sequence.
		/// </param> -----------------------------------------------------------
		template <std::input_iterator iter, std::sentinel_for<iter> sentinel>
		testing::AssertionResult expectSet(
			iter begin, 
			sentinel end,
			auto expected
		) {
			if (begin == end)
				return testing::AssertionFailure() << "Actual range is empty.";

			auto current = begin;
			for (const auto& e : expected) {
				++current;
				EXPECT_NE(std::find(begin, end, e), end)
					<< "Expected element " << e << " not found.";
			}

			EXPECT_EQ(current, end) << "Expected and actual range differ in size.";
			return testing::AssertionSuccess();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method inserts elements at the front of the
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodInsertsAtBeginningOfSequence(
			std::function<void(collection_type&, const_reference)> func
		) {
			collection_type obj{ a, b, c };
			auto expected = { d, a, b, c };
			func(obj, d);
			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method inserts elements at the back of the
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodInsertsAtEndOfSequence(
			std::function<void(collection_type&, const_reference)> func
		) {
			collection_type obj{ a, b, c };
			auto expected = { a, b, c, d };
			func(obj, d);
			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places elements at a specific iterator
		/// position.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodInsertsAtGivenPositionInSequence(
			collection_type& obj,
			std::function<void(collection_type&, const_reference)> func,
			size_t position
		) {
			std::vector<value_type> expected;
			expected.reserve(obj.size() + 1);

			for (auto& value : obj)
				expected.push_back(value);

			func(obj, a);

			auto location = std::next(expected.begin(), position);
			expected.insert(location, a);

			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places a range at the beginning of the
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodInsertsRangeAtBeginningOfSequence(
			std::function<void(collection_type&, const_pointer, const_pointer)> func
		) {
			collection_type obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { d, e, f, a, b, c };
			func(obj, new_elements.begin(), new_elements.end());
			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places a range at the end of the
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodInsertsRangeAtEndOfSequence(
			std::function<void(collection_type&, const_pointer, const_pointer)> func
		) {
			collection_type obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { a, b, c, d, e, f };
			func(obj, new_elements.begin(), new_elements.end());
			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places a range in the middle of the 
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodInsertsRangeAtGivenPositionInSequence(
			collection_type& obj,
			std::function<void(collection_type&, const_pointer, const_pointer)> func,
			size_t position
		) {
			std::vector<value_type> expected;
			expected.reserve(obj.size() + 3);

			for (auto& value : obj)
				expected.push_back(value);

			auto new_elements = { d, e, f };
			func(obj, new_elements.begin(), new_elements.end());

			auto location = std::next(expected.begin(), position);
			expected.insert(location, new_elements.begin(), new_elements.end());

			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a sequence.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodRemovesFirstElementInSequence(
			std::function<void(collection_type&)> func
		) {
			collection_type obj{ a, b, c, d };
			auto expected = { b, c, d };
			func(obj);
			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a sequence.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodRemovesLastElementInSequence(
			std::function<void(collection_type&)> func
		) {
			collection_type obj{ a, b, c, d };
			auto expected = { a, b, c };
			func(obj);
			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the expected element in a 
		/// sequence.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodRemovesElementAtGivenPositionInSequence(
			collection_type& obj,
			std::function<void(collection_type&)> func,
			size_t position
		) {
			std::vector<value_type> expected;
			expected.reserve(obj.size());

			for (auto& value : obj)
				expected.push_back(value);

			func(obj);

			auto location = std::next(expected.begin(), position);
			expected.erase(location);

			return this->expectSequence(obj.begin(), obj.end(), expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method checks the bounds of the index it is
		/// given and throws appropriate exceptions.
		/// </summary> 
		/// 
		/// <param name="safeCall">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodChecksIndexBounds(
			std::function<void(collection_type&, Index&)> func,
			std::function<size_t(collection_type&)> safeIndex,
			std::function<size_t(collection_type&)> unsafeIndex
		) requires sequential<collection_type> {
			collection_type obj1{ a, b, c };
			collection_type obj2{ a, b, c };

			auto safe = Index(safeIndex(obj1));
			auto unsafe = Index(unsafeIndex(obj2));

			EXPECT_NO_THROW(func(obj1, safe));
			EXPECT_THROW(func(obj2, unsafe), std::out_of_range);
			return testing::AssertionSuccess();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method is effectively a NOP when called on
		/// an empty collection, such as clear or remove. This method shall not
		/// throw and has observable side effects on the object leaving it
		/// unchanged.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		testing::AssertionResult expectMethodHasNoEffectOnEmptyCollection(
			std::function<void(collection_type&)> func
		) {
			collection_type obj{ };
			EXPECT_NO_THROW(func(obj));
			EXPECT_TRUE(obj.isEmpty());
			return testing::AssertionSuccess();
		}

		testing::AssertionResult expectCount(
			const collection_type& obj, 
			const std::initializer_list<std::pair<value_type, int>>& expected
		) {
			if constexpr (map<collection_type>) {
				for (const auto& e : expected)
					EXPECT_EQ(obj.count(e.first.key()), e.second);
			}
			else {
				for (const auto& e : expected)
					EXPECT_EQ(obj.count(e.first), e.second);
			}
			return testing::AssertionSuccess();
		}
	};
}