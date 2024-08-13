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

#include "adapters/BinaryHeap.h"

#include "binary_heap_test_fixture.h"

namespace collection_tests {

	class BinaryHeapTest : public CollectionTests<BinaryHeapTestTypes<int>> {};

	TEST_F(BinaryHeapTest, BuildHeapOrdersElementsCorrectly) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		auto expected = { 0, 1, 1, 2, 2, 8, 6, 6, 7, 5 };
		this->testCollectionEqualsExpectedSequence(heap, expected);
	}

	TEST_F(BinaryHeapTest, TopReturnsRootOfTheHeap) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		EXPECT_EQ(heap.top(), 0);
	}

	TEST_F(BinaryHeapTest, PushOntoHeapMaintainsStructure) {
		BinaryHeap<int> heap{};
		
		heap.push(2);
		auto expected = { 2 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(5);
		expected = { 2, 5 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(1);
		expected = { 1, 5, 2 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(7);
		expected = { 1, 5, 2, 7 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(2);
		expected = { 1, 2, 2, 7, 5 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(8);
		expected = { 1, 2, 2, 7, 5, 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);
		
		heap.push(6);
		expected = { 1, 2, 2, 7, 5, 8, 6 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(6);
		expected = { 1, 2, 2, 6, 5, 8, 6, 7 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(1);
		expected = { 1, 1, 2, 2, 5, 8, 6, 7, 6 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.push(0);
		expected = { 0, 1, 2, 2, 1, 8, 6, 7, 6, 5 };
		this->testCollectionEqualsExpectedSequence(heap, expected);
	}

	TEST_F(BinaryHeapTest, PopFromHeapMaintainsStructure) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		// expected at start 0, 1, 1, 2, 2, 8, 6, 6, 7, 5

		heap.pop();
		auto expected = { 1, 2, 1, 5, 2, 8, 6, 6, 7 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 1, 2, 6, 5, 2, 8, 7, 6 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 2, 2, 6, 5, 6, 8, 7 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 2, 5, 6, 7, 6, 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 5, 6, 6, 7, 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 6, 7, 6, 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 6, 7, 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 7, 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		expected = { 8 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		heap.pop();
		EXPECT_TRUE(heap.isEmpty());
	}

	TEST_F(BinaryHeapTest, ChangePriortyOfElementMaintainsStructure) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		// expected order at start 0, 1, 1, 2, 2, 8, 6, 6, 7, 5

		auto it = std::next(heap.begin(), 4);
		EXPECT_EQ(*it, 2);

		// iterators are not invalidated on change but the element they point 
		// to might. This change should not effect order therefore the iterator
		// will not change.
		heap.changePriority(it, 3);
		EXPECT_EQ(*it, 3);
		auto expected = { 0, 1, 1, 2, 3, 8, 6, 6, 7, 5 };
		this->testCollectionEqualsExpectedSequence(heap, expected);


		// This change will push this node down, and the iterator should point 
		// to its replacement 5
		heap.changePriority(it, 12);
		EXPECT_EQ(*it, 5);
		expected = { 0, 1, 1, 2, 5, 8, 6, 6, 7, 12 };
		this->testCollectionEqualsExpectedSequence(heap, expected);

		// go to last element in heap
		it = std::prev(heap.end());
		EXPECT_EQ(*it, 12);

		// change it so that it becomes the root
		heap.changePriority(it, -1);
		EXPECT_EQ(*it, 5);
		EXPECT_EQ(heap.top(), -1);
		expected = { -1, 0, 1, 2, 1, 8, 6, 6, 7, 5 };
		this->testCollectionEqualsExpectedSequence(heap, expected);
	}


	TEST_F(
		BinaryHeapTest, 
		ChangePriorityReturnsIteratorToPositionOfUpdatedElement) 
	{
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		// expected order at start 0, 1, 1, 2, 2, 8, 6, 6, 7, 5

		auto it = std::next(heap.begin(), 4);
		EXPECT_EQ(*it, 2);

		auto result = heap.changePriority(it, 3);
		EXPECT_EQ(*result, 3);

		result = heap.changePriority(it, 12);
		EXPECT_EQ(*result, 12);

		result = heap.changePriority(it, -1);
		EXPECT_EQ(*result, -1);
	}

	TEST_F(BinaryHeapTest, IteratorsCoverFullRange) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		auto order = { 0, 1, 1, 2, 2, 8, 6, 6, 7, 5 };

		auto expected = order.begin();
		for (auto& current : heap) 
			EXPECT_EQ(current, *expected++);
		
		EXPECT_EQ(expected, order.end());
	}

	TEST_F(BinaryHeapTest, ConstIteratorsCoverFullRange) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };
		auto order = { 0, 1, 1, 2, 2, 8, 6, 6, 7, 5 };

		auto expected = order.begin();
		for (const auto& current : heap) 
			EXPECT_EQ(current, *expected++);

		EXPECT_EQ(expected, order.end());
	}

	TEST_F(BinaryHeapTest, ReverseIteratorsCoverFullRange) {
		BinaryHeap<int> heap = { 2, 5, 1, 7, 2, 8, 6, 6, 1, 0 };

		// forward order at start -> 0, 1, 1, 2, 2, 8, 6, 6, 7, 5
		auto reverse = { 5, 7, 6, 6, 8, 2, 2, 1, 1, 0 };

		auto expected = reverse.begin();
		for (const auto& current : std::ranges::reverse_view(heap))
			EXPECT_EQ(current, *expected++);

		EXPECT_EQ(expected, reverse.end());
	}
}