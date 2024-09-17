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

#include <string>
#include <utility>
#include <gtest/gtest.h>

#include "adapters/TreeTraversalAdapters.h"
#include "containers/AVLTree.h"

#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using namespace collections;

	class AVLTreeStructureTest : public CollectionTest<SimpleAVL<int>> {
	protected:
		void expectInOrderNodeHeights(const auto& tree, const auto& expected) {
			ASSERT_EQ(tree.size(), expected.size());

			auto heights = expected.begin();
			auto pos = tree.begin();

			while (pos != tree.end()) 
				EXPECT_EQ(tree.heightOf(pos++), *heights++);

			EXPECT_EQ(heights, expected.end());
		}
	};

	TEST_F(AVLTreeStructureTest, LeftRotationAfterInsertionRebalancesTree) {
		
		SimpleAVL<int> tree{ 0, 1 };

		// Before insert
		//
		//	(0)
		//	  \
		//     (1)
		//       \
		//        _ <---- Insert 2 Here

		ASSERT_EQ(*tree.root(), 0);
		ASSERT_EQ(*tree.minimum(), 0);
		ASSERT_EQ(*tree.maximum(), 1);

		auto inOrderBefore = { 0, 1 };
		auto preOrderBefore = { 0, 1 };
		auto postOrderBefore = { 1, 0 };
		auto levelOrderBefore = { 0, 1 };
		auto inOrderHeightsBefore = { 1, 0 };
		
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(2);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(AVLTreeStructureTest, RightRotationAfterInsertionRebalancesTree) {

		SimpleAVL<int> tree{ 2, 1 };

		// Before insert
		//
		//		(2)
		//	    /
		//    (1)
		//    /
		//   _ <---- Insert 0 Here

		EXPECT_EQ(*tree.root(), 2);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderBefore = { 1, 2 };
		auto preOrderBefore = { 2, 1 };
		auto postOrderBefore = { 1, 2 };
		auto levelOrderBefore = { 2, 1 };
		auto inOrderHeightsBefore = { 0, 1 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(0);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleLeftRightRotationAfterInsertionRebalancesTree
	) {
		SimpleAVL<int> tree{ 2, 0 };

		// Before insert
		//
		//		(2)
		//	    /
		//    (0)
		//      \
		//       _ <---- Insert 1 Here

		EXPECT_EQ(*tree.root(), 2);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderBefore = { 0, 2 };
		auto preOrderBefore = { 2, 0 };
		auto postOrderBefore = { 0, 2 };
		auto levelOrderBefore = { 2, 0 };
		auto inOrderHeightsBefore = { 0, 1 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(1);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleRightLeftRotationAfterInsertionRebalancesTree
	) {
		SimpleAVL<int> tree{ 0, 2 };

		// Before insert
		//
		//		(0)
		//	      \
		//        (2)
		//        /
		//       _ <---- Insert 1 Here

		EXPECT_EQ(*tree.root(), 0);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderBefore = { 0, 2 };
		auto preOrderBefore = { 0, 2 };
		auto postOrderBefore = { 2, 0 };
		auto levelOrderBefore = { 0, 2 };
		auto inOrderHeightsBefore = { 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(1);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);
		
		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		LeftRotationAfterDeletionRebalancesTree
	) {
		SimpleAVL<int> tree{ 0, -1, 1, 2 };

		// Before delete
		//
		//		(0)
		//	    / \
		//   (-1) (1)
		//    /\    \
		//	   \	(2)
		//	    \
		//       \____ Delete -1 Here

		EXPECT_EQ(*tree.root(), 0);
		EXPECT_EQ(*tree.minimum(), -1);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderBefore = { -1, 0, 1, 2 };
		auto preOrderBefore = { 0, -1, 1, 2 };
		auto postOrderBefore = { -1, 2, 1, 0 };
		auto levelOrderBefore = { 0, -1, 1, 2 };
		auto inOrderHeightsBefore = { 0, 2, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = tree.begin();
		ASSERT_EQ(*it, -1);
		tree.remove(it);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		LeftRotationRebalancesAfterRemovingNonLeafNodes
	) {
		SimpleAVL<int> tree{ 0, -1, 1, 2 };

		// Before delete
		//
		//		(0) <--- Delete Here
		//	    / \
		//   (-1) (1)
		//          \
		//	   	    (2)

		auto it = tree.begin<traversal_order::PRE_ORDER>();
		ASSERT_EQ(*it, 0);
		tree.remove(it);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (-1)   (2)

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), -1);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { -1, 1, 2 };
		auto preOrderAfter = { 1, -1, 2 };
		auto postOrderAfter = { -1, 2, 1 };
		auto levelOrderAfter = { 1, -1, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		RightRotationAfterDeletionRebalancesTree
	) {
		SimpleAVL<int> tree{ 2, 1, 3, 0 };

		// Before delete
		//
		//		 (2)
		//	     / \
		//     (1) (3) <--- Delete 3 Here
		//     /    
		//   (0)     

		EXPECT_EQ(*tree.root(), 2);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 3);

		auto inOrderBefore = { 0, 1, 2, 3 };
		auto preOrderBefore = { 2, 1, 0, 3 };
		auto postOrderBefore = { 0, 1, 3, 2 };
		auto levelOrderBefore = { 2, 1, 3, 0 };
		auto inOrderHeightsBefore = { 0, 1, 2, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = --tree.end();
		ASSERT_EQ(*it, 3);
		tree.remove(it);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest,
		RightRotationRebalancesAfterRemovingNonLeafNodes
	) {
		SimpleAVL<int> tree{ 2, 1, 3, 0 };

		// Before delete
		//
		//		 (2) <--- Delete Here
		//	     / \
		//     (1) (3) 
		//     /    
		//   (0)     

		auto it = tree.begin<traversal_order::PRE_ORDER>();
		ASSERT_EQ(*it, 2);
		tree.remove(it);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (3)

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 3);

		auto inOrderAfter = { 0, 1, 3 };
		auto preOrderAfter = { 1, 0, 3 };
		auto postOrderAfter = { 0, 3, 1 };
		auto levelOrderAfter = { 1, 0, 3 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleRightLeftRotationAfterDeletionRebalancesTree
	) {
		SimpleAVL<int> tree{ 0, -1, 2, 1 };

		// Before delete
		//
		//		 (0)
		//	    /   \
		//   (-1)   (2)
		//    /\    / 
		//	   \  (1)	
		//	    \
		//       \____ Delete -1 Here

		EXPECT_EQ(*tree.root(), 0);
		EXPECT_EQ(*tree.minimum(), -1);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderBefore = { -1, 0, 1, 2 };
		auto preOrderBefore = { 0, -1, 2, 1 };
		auto postOrderBefore = { -1, 1, 2, 0 };
		auto levelOrderBefore = { 0, -1, 2, 1 };
		auto inOrderHeightsBefore = { 0, 2, 0, 1 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = tree.begin();
		ASSERT_EQ(*it, -1);
		tree.remove(it);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleRightLeftRebalancesAfterRemovingNonLeafNodes
	) {
		SimpleAVL<int> tree{ 0, -1, 2, 1 };

		// Before delete
		//
		//		 (0) <--- Delete Here
		//	    /   \
		//   (-1)   (2)
		//          / 
		//	      (1)	

		auto it = tree.begin<traversal_order::PRE_ORDER>();
		ASSERT_EQ(*it, 0);
		tree.remove(it);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (-1)   (2)


		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), -1);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { -1, 1, 2 };
		auto preOrderAfter = { 1, -1, 2 };
		auto postOrderAfter = { -1, 2, 1 };
		auto levelOrderAfter = { 1, -1, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleLeftRightRotationAfterDeletionRebalancesTree
	) {
		SimpleAVL<int> tree{ 2, 0, 3, 1 };

		// Before delete
		//
		//		 (2)
		//	     / \
		//     (0) (3) <--- Delete 3 Here
		//       \   
		//       (1)   

		EXPECT_EQ(*tree.root(), 2);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 3);

		auto inOrderBefore = { 0, 1, 2, 3 };
		auto preOrderBefore = { 2, 0, 1, 3 };
		auto postOrderBefore = { 1, 0, 3, 2 };
		auto levelOrderBefore = { 2, 0, 3, 1 };
		auto inOrderHeightsBefore = { 1, 0, 2, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = --tree.end();
		ASSERT_EQ(*it, 3);
		tree.remove(it);

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto levelOrderAfter = { 1, 0, 2 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleLeftRightRebalancesAfterRemovingNonLeafNodes
	) {
		SimpleAVL<int> tree{ 2, 0, 3, 1 };

		// Before delete
		//
		//		 (2) <--- Delete Here
		//	     / \
		//     (0) (3) 
		//       \   
		//       (1)   


		auto it = tree.begin<traversal_order::PRE_ORDER>();
		ASSERT_EQ(*it, 2);
		tree.remove(it);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (3)

		EXPECT_EQ(*tree.root(), 1);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 3);

		auto inOrderAfter = { 0, 1, 3 };
		auto preOrderAfter = { 1, 0, 3 };
		auto postOrderAfter = { 0, 3, 1 };
		auto levelOrderAfter = { 1, 0, 3 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(AVLTreeStructureTest, MutlipleRotationsOnRemoveRebalancesTree) {
		SimpleAVL<int> tree{ 8, 5, 11, 3, 7, 10, 12, 2, 4, 6, 9, 1 };

		// Before delete
		//				    (8)	 
		//	              /     \
		//             (5)       (11)
		//           /    \      /   \
		//         (3)    (7)  (10)  (12) <-- delete here
		//        /  \    /    /
		//       (2) (4)(6)   (9) 
		//       /
		//      (1)   

		EXPECT_EQ(*tree.root(), 8);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 12);

		auto inOrderBefore = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		auto preOrderBefore = { 8, 5, 3, 2, 1, 4, 7, 6, 11, 10, 9, 12 };
		auto postOrderBefore = { 1, 2, 4, 3, 6, 7, 5, 9, 10, 12, 11, 8 };
		auto levelOrderBefore = { 8, 5, 11, 3, 7, 10, 12, 2, 4, 6, 9, 1 };
		auto inOrderHeightsBefore = { 0, 1, 2, 0, 3, 0, 1, 4, 0, 1, 2, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);
		this->expectInOrderNodeHeights(tree, inOrderHeightsBefore);

		auto it = std::prev(tree.end());
		ASSERT_EQ(*it, 12);
		tree.remove(it);

		// After 1st rotation
		//				  (8)	 
		//	            /     \
		//            (5)      (10)
		//          /    \     /   \ 
		//         (3)    (7) (9)  (11)
		//        /  \    /   
		//       (2) (4)(6)    
		//       /
		//      (1)  
		
		// After 2nd Rotation
		//			  (5)
		//          /     \   
		//       (3)       (8)
		//      /   \      /  \
		//    (2)   (4)  (7)  (10)
		//    /          /    /  \
		//   (1)        (6) (9)  (11)

		EXPECT_EQ(*tree.root(), 5);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 11);

		auto inOrderAfter = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		auto preOrderAfter = { 5, 3, 2, 1, 4, 8, 7, 6, 10, 9, 11 };
		auto postOrderAfter = { 1, 2, 4, 3, 6, 7, 9, 11, 10, 8, 5 };
		auto levelOrderAfter = { 5, 3, 8, 2, 4, 7, 10, 1, 6, 9, 11 };
		auto inOrderHeightsAfter = { 0, 1, 2, 0, 3, 0, 1, 2, 0, 1, 0 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);
		this->expectInOrderNodeHeights(tree, inOrderHeightsAfter);
	}

	TEST_F(AVLTreeStructureTest, TreeRotatesDuplicateElements) {

		/* Expected Structure (all elements have the same key 1):
		*       
		*        (1a)                                                   (1b)
		*        /        Rotate after unbalanced insertion of 1c       /  \
		*      (1b)      ---------------------------------------->    (1c) (1a)
		*      /
		*    (1c)
		*/

		using tree_type = MultiMapAVL<int, std::string>;
		tree_type tree{ {1, "1a"}, {1, "1b"}, {1, "1c"} };

		std::initializer_list<tree_type::value_type> preOrder 
			= { {1, "1c"}, {1, "1b"}, {1, "1a"} };

		std::initializer_list<tree_type::value_type> inOrder 
			= { {1, "1b"}, {1, "1a"}, {1, "1c"} };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrder);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrder);
	}

}