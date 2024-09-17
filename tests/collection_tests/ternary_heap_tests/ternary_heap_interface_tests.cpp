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

#include <iterator>
#include <ranges>
#include <gtest/gtest.h>

#include "adapters/Heap.h"

#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using TernaryHeapTest = CollectionTest<TernaryHeap<int>>;

	TEST_F(TernaryHeapTest, BuildHeapOrdersElementsCorrectly) {
		TernaryHeap<int> heap 
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };
		auto expected 
			= { 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 };

		this->expectSequence(heap.begin(), heap.end(), expected);
	}

	TEST_F(TernaryHeapTest, TopReturnsRootOfTheHeap) {
		TernaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		EXPECT_EQ(heap.top(), 0);
	}

	TEST_F(TernaryHeapTest, PushOntoHeapMaintainsStructure) {
		TernaryHeap<int> heap{};
		
		heap.push(2);
		auto expected = { 2 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(5);
		expected = { 2, 5 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(1);
		expected = { 1, 5, 2 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(7);
		expected = { 1, 5, 2, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(2);
		expected = { 1, 2, 2, 7, 5 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(8);
		expected = { 1, 2, 2, 7, 5, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);
		
		heap.push(6);
		expected = { 1, 2, 2, 7, 5, 8, 6 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(6);
		expected = { 1, 2, 2, 7, 5, 8, 6, 6 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(1);
		expected = { 1, 2, 1, 7, 5, 8, 6, 6, 2 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(0);
		expected = { 0, 2, 1, 7, 5, 8, 6, 6, 2, 1 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(2);
		expected = { 0, 2, 1, 2, 5, 8, 6, 6, 2, 1, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(5);
		expected = { 0, 2, 1, 2, 5, 8, 6, 6, 2, 1, 7, 5 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(0);
		expected = { 0, 2, 1, 0, 5, 8, 6, 6, 2, 1, 7, 5, 2 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(1);
		expected = { 0, 1, 1, 0, 2, 8, 6, 6, 2, 1, 7, 5, 2, 5 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(7);
		expected = { 0, 1, 1, 0, 2, 8, 6, 6, 2, 1, 7, 5, 2, 5, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(9);
		expected = { 0, 1, 1, 0, 2, 8, 6, 6, 2, 1, 7, 5, 2, 5, 7, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.push(3);
		expected = { 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 7, 5, 2, 5, 7, 9, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);
	}

	TEST_F(TernaryHeapTest, PopFromHeapMaintainsStructure) {
		TernaryHeap<int> heap 
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };

		// expected heap order start 
		// 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 
		heap.pop();
		auto expected = { 0, 1, 1, 2, 2, 3, 6, 6, 2, 1, 8, 5, 7, 5, 7, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 1, 2, 1, 2, 5, 3, 6, 6, 2, 1, 8, 5, 7, 9, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 1, 2, 1, 2, 5, 3, 6, 6, 2, 7, 8, 5, 7, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 1, 2, 2, 2, 5, 3, 6, 6, 9, 7, 8, 5, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 2, 3, 2, 2, 5, 7, 6, 6, 9, 7, 8, 5 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 2, 3, 5, 2, 5, 7, 6, 6, 9, 7, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 2, 3, 5, 8, 5, 7, 6, 6, 9, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 3, 5, 5, 8, 7, 7, 6, 6, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 5, 6, 5, 8, 7, 7, 9, 6 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 5, 6, 6, 8, 7, 7, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 6, 7, 6, 8, 9, 7 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 6, 7, 7, 8, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 7, 9, 7, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 7, 9, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 8, 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		expected = { 9 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		heap.pop();
		EXPECT_TRUE(heap.isEmpty());
	}

	TEST_F(TernaryHeapTest, ChangePriortyOfElementMaintainsStructure) {
		TernaryHeap<int> heap
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };

		// expected heap order start 
		// 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 
		auto it = std::next(heap.begin(), 5);
		EXPECT_EQ(*it, 3);

		// iterators are not invalidated on change but the element they point 
		// to might. This change should not effect order therefore the iterator
		// will not change.
		heap.changePriority(it, 1);
		EXPECT_EQ(*it, 1);
		auto expected = { 0, 1, 1, 0, 2, 1, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		// This change will push this node down, and the iterator should point 
		// to its replacement 8
		heap.changePriority(it, 12);
		EXPECT_EQ(*it, 8);
		expected = { 0, 1, 1, 0, 2, 8, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 12 };
		this->expectSequence(heap.begin(), heap.end(), expected);

		// go to last element in heap
		it = std::prev(heap.end());
		EXPECT_EQ(*it, 12);

		// change it so that it becomes the root, 
		// its original pos is replaced by 8
		heap.changePriority(it, -1);
		EXPECT_EQ(*it, 8);
		EXPECT_EQ(heap.top(), -1);
		expected = { -1, 0, 1, 0, 2, 1, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 };
		this->expectSequence(heap.begin(), heap.end(), expected);
	}


	TEST_F(
		TernaryHeapTest, 
		ChangePriorityReturnsIteratorToPositionOfUpdatedElement
	) {
		TernaryHeap<int> heap
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };

		// expected heap order start 
		// 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 
		auto it = std::next(heap.begin(), 5);
		EXPECT_EQ(*it, 3);

		// should not affect heap order
		auto result = heap.changePriority(it, 1);
		EXPECT_EQ(*result, 1);

		// will push node down and replace with its only child 8
		result = heap.changePriority(it, 12);
		EXPECT_EQ(*result, 12);

		result = heap.changePriority(it, -1);
		EXPECT_EQ(*result, -1);
	}

	TEST_F(TernaryHeapTest, IteratorsCoverFullRange) {
		TernaryHeap<int> heap 
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };
		auto order 
			= { 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 };

		auto expected = order.begin();
		for (auto& current : heap) 
			EXPECT_EQ(current, *expected++);
		
		EXPECT_EQ(expected, order.end());
	}

	TEST_F(TernaryHeapTest, ConstIteratorsCoverFullRange) {
		TernaryHeap<int> heap 
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };
		auto order 
			= { 0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8 };

		auto expected = order.begin();
		for (const auto& current : heap) 
			EXPECT_EQ(current, *expected++);

		EXPECT_EQ(expected, order.end());
	}

	TEST_F(TernaryHeapTest, ReverseIteratorsCoverFullRange) {
		TernaryHeap<int> heap 
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };

		// forward order at start:
		//  0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 2, 5, 7, 5, 7, 9, 8

		auto reverse = { 8, 9, 7, 5, 7, 5, 2, 1, 2, 6, 6, 3, 2, 0, 1, 1, 0 };

		auto expected = reverse.begin();
		for (auto& current : std::ranges::reverse_view(heap))
			EXPECT_EQ(current, *expected++);

		EXPECT_EQ(expected, reverse.end());
	}

	TEST_F(TernaryHeapTest, ConstReverseIteratorsCoverFullRange) {
		TernaryHeap<int> heap 
			= { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0, 2, 5, 0, 1, 7, 9, 3 };

		// forward order at start:
		//  0, 1, 1, 0, 2, 3, 6, 6, 2, 1, 7, 5, 2, 5, 7, 9, 8 

		auto reverse = { 8, 9, 7, 5, 7, 5, 2, 1, 2, 6, 6, 3, 2, 0, 1, 1, 0 };

		auto expected = reverse.begin();
		for (const auto& current : std::ranges::reverse_view(heap))
			EXPECT_EQ(current, *expected++);

		EXPECT_EQ(expected, reverse.end());
	}
}