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
#include "containers/BinarySearchTree.h"

namespace collection_tests {

	using namespace collections;

	class BinarySearchTreeStructureTest : public BinaryTreeStructureTest {};

	TEST_F(BinarySearchTreeStructureTest, InsertPlacesSmallestElement) {
		/*
						     (5)
						    /   \
						  (3)	(8)
						 /   \    
						(1)  (4) 
					   /
					  __	
					  /\
		insert here __/
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4 };

		auto preOrderBefore = { 5, 3, 1, 4, 8 };
		auto preOrderAfter = { 5, 3, 1, 0, 4, 8 };

		auto inOrderBefore = { 1, 3, 4, 5, 8 };
		auto inOrderAfter = { 0, 1, 3, 4, 5, 8 };

		auto postOrderBefore = { 1, 4, 3, 8, 5 };
		auto postOrderAfter = { 0, 1, 4, 3, 8, 5 };

		this->testPreOrderSequence(tree, preOrderBefore);
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);

		tree.insert(0);

		this->testPreOrderSequence(tree, preOrderAfter);
		this->testInOrderSequence(tree, inOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);

		EXPECT_EQ(tree.root(), 5);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 8);
	}

	TEST_F(BinarySearchTreeStructureTest, InsertPlacesElementInMiddleOfTree) {
		/*
							(5)
						   /   \
						(1)		(8)
					   /  \     / \
					 (0)  (3) (7) (11)
						  /   /  
						(2)  __
							 /\	
			insert here _____/
		*/

		BinarySearchTree<int> tree = { 5, 1, 8, 3, 2, 0, 7, 11 };

		auto preOrderBefore = { 5, 1, 0, 3, 2, 8, 7, 11 };
		auto preOrderAfter = { 5, 1, 0, 3, 2, 8, 7, 6, 11 };

		auto inOrderBefore = { 0, 1, 2, 3, 5, 7, 8, 11 };
		auto inOrderAfter = { 0, 1, 2, 3, 5, 6, 7, 8, 11 };

		auto postOrderBefore = { 0, 2, 3, 1, 7, 11, 8, 5 };
		auto postOrderAfter = { 0, 2, 3, 1, 6, 7, 11, 8, 5 };

		this->testPreOrderSequence(tree, preOrderBefore);
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);

		tree.insert(6);

		this->testPreOrderSequence(tree, preOrderAfter);
		this->testInOrderSequence(tree, inOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);

		EXPECT_EQ(tree.root(), 5);
		EXPECT_EQ(tree.minimum(), 0);
		EXPECT_EQ(tree.maximum(), 11);
	}

	TEST_F(BinarySearchTreeStructureTest, InsertPlacesLargestElement) {
		/*
				   (5)
				  /   \
				(3)	  (8)
			   /   \    \ 
			  (1)  (4)   __
						 /\
		insert here _____/
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4 };

		auto preOrderBefore = { 5, 3, 1, 4, 8 };
		auto preOrderAfter = { 5, 3, 1, 4, 8, 10 };

		auto inOrderBefore = { 1, 3, 4, 5, 8 };
		auto inOrderAfter = { 1, 3, 4, 5, 8, 10 };

		auto postOrderBefore = { 1, 4, 3, 8, 5 };
		auto postOrderAfter = { 1, 4, 3, 10, 8, 5 };

		this->testPreOrderSequence(tree, preOrderBefore);
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);

		tree.insert(10);

		this->testPreOrderSequence(tree, preOrderAfter);
		this->testInOrderSequence(tree, inOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);

		EXPECT_EQ(tree.root(), 5);
		EXPECT_EQ(tree.minimum(), 1);
		EXPECT_EQ(tree.maximum(), 10);
	}

	TEST_F(BinarySearchTreeStructureTest, RemoveDeletesSmallestElement) {
		/*
						 (5) 
						/   \
					  (3)  (8) 
					 /   \    \	   
				   (1)   (4)  (10)	   
				   /\		  /
				    \       (9)
					 \
					  \____ Delete Here
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto node = tree.begin();
		ASSERT_EQ(tree.minimum(), 1);
		ASSERT_EQ(*node, 1);

		tree.remove(node);

		EXPECT_EQ(tree.minimum(), 3);
	}

	TEST_F(BinarySearchTreeStructureTest, RemoveDeletesLargestElement) {
		/*
				(5) 
			   /   \
			  (3)  (8) 
			 /   \    \	   
		   (1)   (4)  (10)	<------ Delete Here   
					  /
					(9) 
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto node = --tree.end();
		ASSERT_EQ(tree.maximum(), 10);
		ASSERT_EQ(*node, 10);

		tree.remove(node);

		EXPECT_EQ(tree.maximum(), 9);
	}

	TEST_F(BinarySearchTreeStructureTest, RemoveDeletesLeafNodesCorrectly) {
		/*
					 (5)
					/   \
				   (3)  (8)
				  /   \    
				(1)   (4)  
					  /\
		delete here __/
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4 };

		auto preOrderBefore = { 5, 3, 1, 4, 8 };
		auto preOrderAfter = { 5, 3, 1, 8 };

		auto inOrderBefore = { 1, 3, 4, 5, 8 };
		auto inOrderAfter = { 1, 3, 5, 8 };

		auto postOrderBefore = { 1, 4, 3, 8, 5 };
		auto postOrderAfter = { 1, 3, 8, 5 };

		this->testPreOrderSequence(tree, preOrderBefore);
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);

		auto node = tree.find(4);
		ASSERT_EQ(*node, 4);
		tree.remove(node);

		this->testPreOrderSequence(tree, preOrderAfter);
		this->testInOrderSequence(tree, inOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);

		EXPECT_EQ(tree.root(), 5);
		EXPECT_EQ(tree.minimum(), 1);
		EXPECT_EQ(tree.maximum(), 8);
	}

	TEST_F(
		BinarySearchTreeStructureTest,
		RemoveDeletesNodesWithOneChildCorrectly
	) {
		/*
						 (5)
						/   \
					   (3)  (8) <----- delete here
					  /   \    \	   
					(1)   (4)  (10)	   
							   /
							 (9)
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto preOrderBefore = { 5, 3, 1, 4, 8, 10, 9 };
		auto preOrderAfter = { 5, 3, 1, 4, 10, 9 };

		auto inOrderBefore = { 1, 3, 4, 5, 8, 9, 10 };
		auto inOrderAfter = { 1, 3, 4, 5, 9, 10 };

		auto postOrderBefore = { 1, 4, 3, 9, 10, 8, 5 };
		auto postOrderAfter = { 1, 4, 3, 9, 10, 5 };

		this->testPreOrderSequence(tree, preOrderBefore);
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);

		auto node = tree.find(8);
		ASSERT_EQ(*node, 8);
		tree.remove(node);

		this->testPreOrderSequence(tree, preOrderAfter);
		this->testInOrderSequence(tree, inOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);

		EXPECT_EQ(tree.root(), 5);
		EXPECT_EQ(tree.minimum(), 1);
		EXPECT_EQ(tree.maximum(), 10);
	}

	TEST_F(BinarySearchTreeStructureTest, RemoveDeletesFullNodesCorrectly) {
		/*
						 (5) <---- delete here
						/   \
					   (3)  (8) 
					  /   \    \	   
					(1)   (4)  (10)	   
								/
							  (9)
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto preOrderBefore = { 5, 3, 1, 4, 8, 10, 9 };
		auto preOrderAfter = { 4, 3, 1, 8, 10, 9 };

		auto inOrderBefore = { 1, 3, 4, 5, 8, 9, 10 };
		auto inOrderAfter = { 1, 3, 4, 8, 9, 10 };

		auto postOrderBefore = { 1, 4, 3, 9, 10, 8, 5 };
		auto postOrderAfter = { 1, 3, 9, 10, 8, 4 };

		this->testPreOrderSequence(tree, preOrderBefore);
		this->testInOrderSequence(tree, inOrderBefore);
		this->testPostOrderSequence(tree, postOrderBefore);

		auto node = tree.find(5);
		ASSERT_EQ(*node, 5);
		tree.remove(node);

		this->testPreOrderSequence(tree, preOrderAfter);
		this->testInOrderSequence(tree, inOrderAfter);
		this->testPostOrderSequence(tree, postOrderAfter);

		EXPECT_EQ(tree.root(), 4);
		EXPECT_EQ(tree.minimum(), 1);
		EXPECT_EQ(tree.maximum(), 10);
	}

	TEST_F(BinarySearchTreeStructureTest, NodeHeightIsReportedCorrectly) { 
		/*
				  (5)
				 /   \
			    (3)  (8) 
			   /   \    \	   
		     (1)   (4)  (10)	   
					    /
					  (9)
		*/

		BinarySearchTree<int> tree = { 5, 3, 8, 1, 4, 10, 9 };
		auto it = tree.begin();

		// in order traversal -> expected height of each
		EXPECT_EQ(tree.heightOf(it++), 0);	// 1
		EXPECT_EQ(tree.heightOf(it++), 1);	// 3
		EXPECT_EQ(tree.heightOf(it++), 0);	// 4
		EXPECT_EQ(tree.heightOf(it++), 3);;	// 5
		EXPECT_EQ(tree.heightOf(it++), 2);	// 8
		EXPECT_EQ(tree.heightOf(it++), 0);	// 9
		EXPECT_EQ(tree.heightOf(it++), 1);	// 10
		EXPECT_EQ(it, tree.end());
	}
}