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

#include "../../test_suites/binary_tree_test_fixture.h"
#include "adapters/TreeTraversalAdapters.h"
#include "containers/AVLTree.h"

namespace collection_tests {

	using namespace collections;

	class AVLTreeStructureTest : public BinaryTreeStructureTest {
	protected:
		void testNodeHeightsInOrder(const auto& tree, const auto& expected) {
			ASSERT_EQ(tree.size(), expected.size());

			auto heights = expected.begin();
			auto pos = tree.begin();

			while (pos != tree.end()) 
				EXPECT_EQ(tree.heightOf(pos++), *heights++);

			EXPECT_EQ(heights, expected.end());
		}
	};

	TEST_F(AVLTreeStructureTest, LeftRotationAfterInsertionRebalancesTree) {
		
		AVLTree<int> tree{ 0, 1 };

		// Before insert
		//
		//	(0)
		//	  \
		//     (1)
		//       \
		//        _ <---- Insert 2 Here

		ASSERT_EQ(tree.root(), 0);
		ASSERT_EQ(tree.minimum(), 0);
		ASSERT_EQ(tree.maximum(), 1);

		auto inOrderBefore = { 0, 1 };
		auto preOrderBefore = { 0, 1 };
		auto postOrderBefore = { 1, 0 };
		auto inOrderHeightsBefore = { 1, 0 };
		
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(2);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(AVLTreeStructureTest, RightRotationAfterInsertionRebalancesTree) {

		AVLTree<int> tree{ 2, 1 };

		// Before insert
		//
		//		(2)
		//	    /
		//    (1)
		//    /
		//   _ <---- Insert 0 Here

		EXPECT_EQ(tree.root(), 2);
		EXPECT_EQ(tree.minimum(), 1);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderBefore = { 1, 2 };
		auto preOrderBefore = { 2, 1 };
		auto postOrderBefore = { 1, 2 };
		auto inOrderHeightsBefore = { 0, 1 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(0);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleLeftRightRotationAfterInsertionRebalancesTree
	) {
		AVLTree<int> tree{ 2, 0 };

		// Before insert
		//
		//		(2)
		//	    /
		//    (0)
		//      \
		//       _ <---- Insert 1 Here

		EXPECT_EQ(tree.root(), 2);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderBefore = { 0, 2 };
		auto preOrderBefore = { 2, 0 };
		auto postOrderBefore = { 0, 2 };
		auto inOrderHeightsBefore = { 0, 1 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(1);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleRightLeftRotationAfterInsertionRebalancesTree
	) {
		AVLTree<int> tree{ 0, 2 };

		// Before insert
		//
		//		(0)
		//	      \
		//        (2)
		//        /
		//       _ <---- Insert 1 Here

		EXPECT_EQ(tree.root(), 0);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderBefore = { 0, 2 };
		auto preOrderBefore = { 0, 2 };
		auto postOrderBefore = { 2, 0 };
		auto inOrderHeightsBefore = { 1, 0 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after insert and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		tree.insert(1);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);
		
		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		LeftRotationAfterDeletionRebalancesTree
	) {
		AVLTree<int> tree{ 0, -1, 1, 2 };

		// Before delete
		//
		//		(0)
		//	    / \
		//   (-1) (1)
		//    /\    \
		//	   \	(2)
		//	    \
		//       \____ Delete -1 Here

		EXPECT_EQ(tree.root(), 0);
		EXPECT_EQ(tree.minimum(), -1);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderBefore = { -1, 0, 1, 2 };
		auto preOrderBefore = { 0, -1, 1, 2 };
		auto postOrderBefore = { -1, 2, 1, 0 };
		auto inOrderHeightsBefore = { 0, 2, 1, 0 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = tree.begin();
		ASSERT_EQ(*it, -1);
		tree.remove(it);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		RightRotationAfterDeletionRebalancesTree
	) {
		AVLTree<int> tree{ 2, 1, 3, 0 };

		// Before delete
		//
		//		 (2)
		//	     / \
		//     (1) (3) <--- Delete 3 Here
		//     /    
		//   (0)     

		EXPECT_EQ(tree.root(), 2);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 3);

		auto inOrderBefore = { 0, 1, 2, 3 };
		auto preOrderBefore = { 2, 1, 0, 3 };
		auto postOrderBefore = { 0, 1, 3, 2 };
		auto inOrderHeightsBefore = { 0, 1, 2, 0 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = --tree.end();
		ASSERT_EQ(*it, 3);
		tree.remove(it);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleLeftRightRotationAfterDeletionRebalancesTree
	) {
		AVLTree<int> tree{ 0, -1, 2, 1 };

		// Before delete
		//
		//		 (0)
		//	    /   \
		//   (-1)   (2)
		//    /\    / 
		//	   \  (1)	
		//	    \
		//       \____ Delete -1 Here

		EXPECT_EQ(tree.root(), 0);
		EXPECT_EQ(tree.minimum(), -1);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderBefore = { -1, 0, 1, 2 };
		auto preOrderBefore = { 0, -1, 2, 1 };
		auto postOrderBefore = { -1, 1, 2, 0 };
		auto inOrderHeightsBefore = { 0, 2, 0, 1 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = tree.begin();
		ASSERT_EQ(*it, -1);
		tree.remove(it);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		DoubleRightLeftRotationAfterDeletionRebalancesTree
	) {

		AVLTree<int> tree{ 2, 0, 3, 1 };

		// Before delete
		//
		//		 (2)
		//	     / \
		//     (0) (3) <--- Delete 3 Here
		//       \   
		//       (1)   

		EXPECT_EQ(tree.root(), 2);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 3);

		auto inOrderBefore = { 0, 1, 2, 3 };
		auto preOrderBefore = { 2, 0, 1, 3 };
		auto postOrderBefore = { 1, 0, 3, 2 };
		auto inOrderHeightsBefore = { 1, 0, 2, 0 };

		this->testInOrderSequence(tree, inOrderBefore);
		this->testPreOrderSequence(tree, preOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);
		this->testNodeHeightsInOrder(tree, inOrderHeightsBefore);

		// Expected tree after delete and rotation
		//
		//		(1)
		//	   /   \
		//	 (0)   (2)

		auto it = --tree.end();
		ASSERT_EQ(*it, 3);
		tree.remove(it);

		EXPECT_EQ(tree.root(), 1);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 2);

		auto inOrderAfter = { 0, 1, 2 };
		auto preOrderAfter = { 1, 0, 2 };
		auto postOrderAfter = { 0, 2, 1 };
		auto inOrderHeightsAfter = { 0, 1, 0 };

		this->testInOrderSequence(tree, inOrderAfter);
		this->testPreOrderSequence(tree, preOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);
		this->testNodeHeightsInOrder(tree, inOrderHeightsAfter);
	}

	TEST_F(
		AVLTreeStructureTest, 
		TreeIsRebalancedWithMultipleRotationsAfterRemove
	) {
		AVLTree<int> tree{};
	}
}