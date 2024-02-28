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
#include "../Algorithms_DataStructures/Source/algorithms/collection_algorithms.h"
#include "../Algorithms_DataStructures/Source/concepts/list.h"

namespace collection_tests {

	template<class params> requires list<typename params::collection_t>
	class ListTest : public CollectionTest<params> {};

	TYPED_TEST_SUITE_P(ListTest);

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator pair in the range covers the full list.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, IteratorsCoverFullList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);
		
		size_t i = 0;
		for (const auto& e : obj)
			EXPECT_EQ(e, input[i++]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the index operator returns the expected element at the 
	/// correct index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, IndexOperatorReturnsCorrectElement) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the at() method returns the expected element at the correct
	/// index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, AtMethodReturnsCorrectElement) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj.at(i), input[i]);
	}


	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the at method throws an excepttion when an out of bounds
	/// index is passed.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, AtMethodPerformsBoundsChecking) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);

		auto safeCall = [&]() {auto e = obj.at(input.size() - 1); };
		auto unsafeCall = [&]() { auto e = obj.at(input.size()); };

		EXPECT_NO_THROW(safeCall());
		EXPECT_THROW(unsafeCall(), std::out_of_range);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that calling front on a list returns the first element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, FrontReturnsFirstInList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);
		EXPECT_EQ(obj.front(), input[0]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that calling back on a list returns the last element.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, BackReturnsLastInList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		const collection obj(collections::from_range, input);
		EXPECT_EQ(obj.back(), input[input.size() - 1]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertFront method inserts the given element at the
	/// top of the list.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertFrontPlacesElementFirstInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);
		ASSERT_NE(obj.front(), value);

		obj.insertFront(value);
		
		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.front(), value);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i + 1], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the insertBack method inserts the given element at the last 
	/// position in the list.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertBackPlacesElementLastInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);
		ASSERT_NE(obj.back(), value);

		obj.insertBack(value);

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.back(), value);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtIndexZeroPlacesElementFirstInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto index = Index(0);
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);
		ASSERT_NE(obj.front(), value);

		obj.insert(index, value);

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.front(), value);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i + 1], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method correctly places elements when
	/// called with an index of [size()].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtLastIndexPlacesElementLastInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);
		ASSERT_NE(obj.back(), value);

		auto index = Index(obj.size());
		obj.insert(index, value);

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.back(), value);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method inserts elements at the requested 
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtIndexPlacesElementAtCorrectPosition) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];
		auto index = Index(1);

		collection obj(collections::from_range, input);

		obj.insert(index, value);

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(obj[1], value);
		for (size_t i = 1, j = 2; i < input.size(); ++i, ++j)
			EXPECT_EQ(input[i], obj[j]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed insert method checks the bounds of the given 
	/// index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtIndexChecksBounds) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];
		auto index1 = Index(input.size());
		auto index2 = Index(input.size() + 1);

		collection obj1(collections::from_range, input);
		collection obj2(collections::from_range, input);

		auto safeCall = [&]() { obj1.insert(index1, value); };
		auto unsafeCall = [&]() { obj2.insert(index2, value); };

		EXPECT_NO_THROW(safeCall());
		EXPECT_THROW(unsafeCall(), std::out_of_range);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method correctly places elements first
	/// in the list when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtBeginIteratorPlacesElementFirstInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);
		ASSERT_NE(obj.front(), value);

		obj.insert(obj.begin(), value);

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.front(), value);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i + 1], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method correctly places elements last
	/// in the list when called with the end iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtEndIteratorPlacesElementLastInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);
		ASSERT_NE(obj.back(), value);

		auto index = Index(obj.size());
		obj.insert(obj.end(), value);

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj.back(), value);

		for (size_t i = 0; i < input.size(); ++i)
			EXPECT_EQ(obj[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator insert method inserts elements at the requested
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, InsertAtIteratorPlacesElementAtCorrectPosition) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto value = this->testInput.different_elements()[0];

		collection obj(collections::from_range, input);

		auto posBefore = obj.begin();
		posBefore++;

		obj.insert(posBefore, value);

		auto posAfter = obj.begin();
		posAfter++;

		EXPECT_EQ(obj.size(), input.size() + 1);
		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(*posAfter, value);
		for (size_t i = 1, j = 2; i < input.size(); ++i, ++j)
			EXPECT_EQ(input[i], obj[j]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeFront method removes the first element in the 
	/// list.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveFrontDeletesFirstElementInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);

		auto first_element = obj.front();
		obj.removeFront();

		EXPECT_FALSE(collections::contains(obj, first_element));
		EXPECT_EQ(obj.size(), input.size() - 1);
		for (size_t i = 0; i < input.size() - 1; ++i)
			EXPECT_EQ(obj[i], input[i + 1]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeBack method removes the last element in the 
	/// list.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveBackDeletesLastElementInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);

		auto last_element = obj.back();
		obj.removeBack();

		EXPECT_FALSE(collections::contains(obj, last_element));
		EXPECT_EQ(obj.size(), input.size() - 1);
		for (size_t i = 0; i < input.size() - 1; ++i)
			EXPECT_EQ(obj[i], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the first
	/// element in the list when called with an index of [0].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAtIndexZeroDeletesFirstElementInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);
		auto first_element = obj.front();
		obj.remove(Index(0));

		EXPECT_FALSE(collections::contains(obj, first_element));
		EXPECT_EQ(obj.size(), input.size() - 1);
		for (size_t i = 1; i < input.size(); ++i)
			EXPECT_EQ(obj[i - 1], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method correctly deletes the last element
	/// in the list when called with an index of [size()].
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAtLastIndexDeletesLastElementInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		
		collection obj(collections::from_range, input);
		auto last_element = obj.back();

		obj.remove(Index(obj.size() - 1));

		EXPECT_FALSE(collections::contains(obj, last_element));
		EXPECT_EQ(obj.size(), input.size() - 1);
		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(obj[1], input[1]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed remove method deletes elements at the requested 
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAtIndexDeletesElementAtCorrectPosition) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto index = Index(1);

		collection obj(collections::from_range, input);

		auto removed_element = obj[index.get()];
		obj.remove(index);

		EXPECT_FALSE(collections::contains(obj, removed_element));
		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(obj[1], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the removeAt method checks index bounds throwing an 
	/// exception if an out of bounds removal is attempted.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAtIndexChecksBounds) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();
		auto indexInBounds = Index(input.size() - 1);
		auto indexOutOfBounds = Index(input.size());

		collection obj1(collections::from_range, input);
		collection obj2(collections::from_range, input);

		auto safeRemove = [&]() { obj1.remove(indexInBounds); };
		auto unsafeRemove = [&]() { obj2.remove(indexOutOfBounds); };

		EXPECT_NO_THROW(safeRemove());
		EXPECT_THROW(unsafeRemove(), std::out_of_range);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method correctly deletes the first 
	/// element in the list when called with the begin iterator.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAtBeginIteratorDeletesFirstElementInTheList) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);
		auto first_element = obj.front();
		obj.remove(obj.begin());

		EXPECT_FALSE(collections::contains(obj, first_element));
		EXPECT_EQ(obj.size(), input.size() - 1);
		for (size_t i = 1; i < input.size(); ++i)
			EXPECT_EQ(obj[i - 1], input[i]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator remove method removes elements at the requested
	/// position.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAtIteratorDeletesElementAtCorrectPosition) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);

		auto iterator = obj.begin();
		iterator++;
		auto removedElement = *iterator;

		obj.remove(iterator);
		EXPECT_FALSE(collections::contains(obj, removedElement));
		EXPECT_EQ(obj.size(), input.size() - 1);

		EXPECT_EQ(obj[0], input[0]);
		EXPECT_EQ(obj[1], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed removeAll method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAllBetweenIndexCorrectlyRemovesElements) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);
		EXPECT_EQ(obj.size(), input.size());

		obj.removeAll({.begin = 0, .end = 2});
		EXPECT_EQ(obj.size(), input.size() - 2);
		EXPECT_EQ(obj[0], input[2]);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the indexed removeAll method throws an exception when the
	/// range is out of bounds or the begin index is > the end index.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAllBetweenIndexChecksBounds) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);
		ASSERT_GT(obj.size(), 1);

		auto safeRange = IndexRange{ .begin = 0, .end = input.size() - 1 };
		auto unsafeBegin = IndexRange{ .begin = input.size(), .end = input.size() };
		auto unsafeEnd = IndexRange{ .begin = 0, .end = input.size() + 1 };
		auto unsafeRange = IndexRange{ .begin = input.size(), .end = input.size() + 1 };
		auto unsafeOutOfOrder = IndexRange{ .begin = input.size() - 1, .end = 0 };

		EXPECT_NO_THROW(obj.removeAll(safeRange));
		EXPECT_THROW(obj.removeAll(unsafeBegin), std::out_of_range);
		EXPECT_THROW(obj.removeAll(unsafeEnd), std::out_of_range);
		EXPECT_THROW(obj.removeAll(unsafeRange), std::out_of_range);
		EXPECT_THROW(obj.removeAll(unsafeOutOfOrder), std::exception);
	}

	// ------------------------------------------------------------------------
	/// <summary>
	/// Tests that the iterator removeAll method correctly removes elements
	/// within the given range.
	/// </summary> ------------------------------------------------------------
	TYPED_TEST_P(ListTest, RemoveAllBetweenIteratorsCorrectlyRemovesElements) {
		using collection = typename TypeParam::collection_t;
		auto input = this->testInput.control();

		collection obj(collections::from_range, input);
		EXPECT_EQ(obj.size(), input.size());

		auto begin = obj.begin();
		auto end = begin;
		end++;
		end++;

		obj.removeAll(begin, end);

		EXPECT_EQ(obj.size(), input.size() - 2);
		EXPECT_EQ(obj[0], input[2]);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		ListTest,
		IteratorsCoverFullList,
		IndexOperatorReturnsCorrectElement,
		AtMethodReturnsCorrectElement,
		AtMethodPerformsBoundsChecking,
		FrontReturnsFirstInList,
		BackReturnsLastInList,
		InsertFrontPlacesElementFirstInTheList,
		InsertBackPlacesElementLastInTheList,
		InsertAtIndexZeroPlacesElementFirstInTheList,
		InsertAtLastIndexPlacesElementLastInTheList,
		InsertAtIndexPlacesElementAtCorrectPosition,
		InsertAtIndexChecksBounds,
		InsertAtBeginIteratorPlacesElementFirstInTheList,
		InsertAtEndIteratorPlacesElementLastInTheList,
		InsertAtIteratorPlacesElementAtCorrectPosition,
		RemoveFrontDeletesFirstElementInTheList,
		RemoveBackDeletesLastElementInTheList,
		RemoveAtIndexZeroDeletesFirstElementInTheList,
		RemoveAtLastIndexDeletesLastElementInTheList,
		RemoveAtIndexDeletesElementAtCorrectPosition,
		RemoveAtIndexChecksBounds,
		RemoveAtBeginIteratorDeletesFirstElementInTheList,
		RemoveAtIteratorDeletesElementAtCorrectPosition,
		RemoveAllBetweenIndexCorrectlyRemovesElements,
		RemoveAllBetweenIndexChecksBounds,
		RemoveAllBetweenIteratorsCorrectlyRemovesElements
	);
}