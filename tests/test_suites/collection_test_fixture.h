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

#include "../mocks/mock_allocator.h"
#include "test_inputs.h"
#include "algorithms/collection_algorithms.h"

#define FORWARD_TEST_TYPES()											\
	using base						= CollectionTests<TypeParam>;		\
	using collection				= base::collection;					\
	using mocked_collection			= base::mocked_collection;			\
	using value_type				= base::value_type;					\
	using allocator_type			= base::allocator_type;				\
	using size_type					= base::size_type;					\
	using difference_type			= base::difference_type;			\
	using reference					= base::reference;					\
	using const_reference			= base::const_reference;			\
	using pointer					= base::pointer;					\
	using const_pointer				= base::const_pointer;				\
	using iterator					= base::iterator;					\
	using const_iterator			= base::const_iterator;				\
	using reverse_iterator			= base::reverse_iterator;			\
	using const_reverse_iterator	= base::const_reverse_iterator;		

#define DECLARE_TEST_DATA()												\
	const_reference a				= test_data<value_type>.a;			\
	const_reference b				= test_data<value_type>.b;			\
	const_reference c				= test_data<value_type>.c;			\
	const_reference d				= test_data<value_type>.d;			\
	const_reference e				= test_data<value_type>.e;			\
	const_reference f				= test_data<value_type>.f							

namespace collection_tests {

	using namespace collections;
	using testing::_;

	template <template <class> class T>
	using instantiate_with_elements = testing::Types<
		T<uint8_t>,
		T<uint16_t>,
		T<uint32_t>,
		T<uint64_t>,
		T<float>,
		T<double>,
		T<void*>,
		T<std::string>,
		T<typename T<uint8_t>::collection_t>
	>;

	template <class params> requires collection<typename params::collection_t>
	class CollectionTests :
		public testing::Test,
		public MockAllocatorTest<typename params::collection_t::value_type>
	{
	protected:

		using collection		= typename params::collection_t;
		using mocked_collection = typename params::mock_t;

		using value_type		= collection::value_type;
		using allocator_type	= collection::allocator_type;
		using size_type			= collection::size_type;
		using difference_type	= collection::difference_type;
		using reference			= collection::reference;
		using const_reference	= collection::const_reference;
		using pointer			= collection::pointer;
		using const_pointer		= collection::const_pointer;

		using iterator = std::conditional_t<
			std::ranges::input_range<collection>, 
			typename collection::iterator, 
			void
		>;

		using const_iterator = std::conditional_t<
			std::ranges::input_range<collection>, 
			typename collection::const_iterator, 
			void
		>;

		using reverse_iterator = std::conditional_t<
			std::ranges::bidirectional_range<collection>, 
			typename collection::reverse_iterator, 
			void
		>;

		using const_reverse_iterator = std::conditional_t<
			std::ranges::bidirectional_range<collection>, 
			typename collection::const_reverse_iterator, 
			void
		>;

		DECLARE_TEST_DATA();

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method returns an iterator to the expected
		/// element.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		void testMethodReturnsIteratorToExpectedElement(
			std::function<iterator(collection&)> func,
			std::function<value_type(collection&)> expected
		) requires std::ranges::input_range<collection> {
			collection obj{ a, b, c, d, e, f };
			auto exp = expected(obj);
			auto result = func(obj);
			EXPECT_EQ(*result, exp);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given object and expected sequence match size and
		/// element order.
		/// </summary>
		/// 
		/// <param name="obj">
		/// The actual object under test.
		/// </param>
		/// 
		/// <param name="expected">
		/// The expected result.
		/// </param> -----------------------------------------------------------
		void testCollectionEqualsExpectedSequence(
			const auto& obj, 
			const auto& expected
		) requires std::ranges::input_range<collection> {
			EXPECT_EQ(obj.size(), expected.size());

			auto it = obj.begin();

			for (const auto& e : expected)
				EXPECT_EQ(*it++,  e);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given object's contents match that of the specified
		/// expected set, order does not matter.
		/// </summary>
		/// 
		/// <param name="input">
		/// The input elements to match against.
		/// </param>
		/// 
		/// <param name="obj">
		/// The obj whose contents to examine.
		/// </param> -----------------------------------------------------------
		void testObjectContainsAllExpectedElements(
			const auto& obj, 
			const auto& input
		) requires std::ranges::input_range<collection> {
			EXPECT_EQ(obj.size(), input.size());

			for (const auto& e : input)
				EXPECT_NE(collections::find(obj, e), obj.end());
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places elements at the front of the
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		void testMethodPlacesElementFirst(
			std::function<void(collection&, const_reference)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto expected = { d, a, b, c };
			func(obj, d);

			this->testCollectionEqualsExpectedSequence(obj, expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method places elements at the back of the
		/// sequence.
		/// </summary> 
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		void testMethodPlacesElementLast(
			std::function<void(collection&, const_reference)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto expected = { a, b, c, d };
			func(obj, d);

			this->testCollectionEqualsExpectedSequence(obj, expected);
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
		void testMethodPlacesElementInTheMiddle(
			std::function<void(collection&, const_reference)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto expected = { a, d, b, c };
			func(obj, d);

			this->testCollectionEqualsExpectedSequence(obj, expected);
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
		void testMethodPlacesRangeAtBeginning(
			std::function<void(collection&, const_pointer, const_pointer)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { d, e, f, a, b, c };
			func(obj, new_elements.begin(), new_elements.end());

			this->testCollectionEqualsExpectedSequence(obj, expected);
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
		void testMethodPlacesRangeAtEnd(
			std::function<void(collection&, const_pointer, const_pointer)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { a, b, c, d, e, f };
			func(obj, new_elements.begin(), new_elements.end());

			this->testCollectionEqualsExpectedSequence(obj, expected);
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
		void testMethodPlacesRangeInTheMiddle(
			std::function<void(collection&, const_pointer, const_pointer)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto new_elements = { d, e, f };
			auto expected = { a, d, e, f, b, c };
			func(obj, new_elements.begin(), new_elements.end());

			this->testCollectionEqualsExpectedSequence(obj, expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a sequence.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		void testMethodRemovesFirstElement(
			std::function<void(collection&)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto expected = { b, c };
			func(obj);

			this->testCollectionEqualsExpectedSequence(obj, expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a sequence.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		void testMethodRemovesLastElement(
			std::function<void(collection&)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto expected = { a, b };
			func(obj);

			this->testCollectionEqualsExpectedSequence(obj, expected);
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Tests that the given method removes the first element in a sequence.
		/// </summary>
		/// 
		/// <param name="func">
		/// The method under test.
		/// </param> -----------------------------------------------------------
		void testMethodRemovesMiddleElement(
			std::function<void(collection&)> func
		) requires std::ranges::input_range<collection> {

			collection obj{ a, b, c };
			auto expected = { a, c };
			func(obj);

			this->testCollectionEqualsExpectedSequence(obj, expected);
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
		) requires sequential_collection<collection> {
			collection obj1{ a, b, c };
			collection obj2{ a, b, c };

			auto safe = Index(safeIndex(obj1));
			auto unsafe = Index(unsafeIndex(obj2));

			EXPECT_NO_THROW(func(obj1, safe));
			EXPECT_THROW(func(obj2, unsafe), std::out_of_range);
		}
	};
}