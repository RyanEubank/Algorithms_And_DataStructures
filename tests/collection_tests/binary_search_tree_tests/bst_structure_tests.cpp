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
#include "containers/BinarySearchTree.h"

#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using BinarySearchTreeStructureTest = CollectionTest<SimpleBST<int>>;

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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4 };

		auto preOrderBefore = { 5, 3, 1, 4, 8 };
		auto preOrderAfter = { 5, 3, 1, 0, 4, 8 };

		auto inOrderBefore = { 1, 3, 4, 5, 8 };
		auto inOrderAfter = { 0, 1, 3, 4, 5, 8 };

		auto postOrderBefore = { 1, 4, 3, 8, 5 };
		auto postOrderAfter = { 0, 1, 4, 3, 8, 5 };

		auto levelOrderBefore = { 5, 3, 8, 1, 4 };
		auto levelOrderAfter = { 5, 3, 8, 1, 4, 0 };

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);

		tree.insert(0);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);

		EXPECT_EQ(*tree.root(), 5);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 8);
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

		SimpleBST<int> tree = { 5, 1, 8, 3, 2, 0, 7, 11 };

		auto preOrderBefore = { 5, 1, 0, 3, 2, 8, 7, 11 };
		auto preOrderAfter = { 5, 1, 0, 3, 2, 8, 7, 6, 11 };

		auto inOrderBefore = { 0, 1, 2, 3, 5, 7, 8, 11 };
		auto inOrderAfter = { 0, 1, 2, 3, 5, 6, 7, 8, 11 };

		auto postOrderBefore = { 0, 2, 3, 1, 7, 11, 8, 5 };
		auto postOrderAfter = { 0, 2, 3, 1, 6, 7, 11, 8, 5 };

		auto levelOrderBefore = { 5, 1, 8, 0, 3, 7, 11, 2 };
		auto levelOrderAfter = { 5, 1, 8, 0, 3, 7, 11, 2, 6 };

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);

		tree.insert(6);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);

		EXPECT_EQ(*tree.root(), 5);
		EXPECT_EQ(*tree.minimum(), 0);
		EXPECT_EQ(*tree.maximum(), 11);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4 };

		auto preOrderBefore = { 5, 3, 1, 4, 8 };
		auto preOrderAfter = { 5, 3, 1, 4, 8, 10 };

		auto inOrderBefore = { 1, 3, 4, 5, 8 };
		auto inOrderAfter = { 1, 3, 4, 5, 8, 10 };

		auto postOrderBefore = { 1, 4, 3, 8, 5 };
		auto postOrderAfter = { 1, 4, 3, 10, 8, 5 };

		auto levelOrderBefore = { 5, 3, 8, 1, 4 };
		auto levelOrderAfter = { 5, 3, 8, 1, 4, 10 };

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);

		tree.insert(10);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);

		EXPECT_EQ(*tree.root(), 5);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 10);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto node = tree.begin();
		ASSERT_EQ(*tree.minimum(), 1);
		ASSERT_EQ(*node, 1);

		tree.remove(node);

		EXPECT_EQ(*tree.minimum(), 3);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto node = --tree.end();
		ASSERT_EQ(*tree.maximum(), 10);
		ASSERT_EQ(*node, 10);

		tree.remove(node);

		EXPECT_EQ(*tree.maximum(), 9);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4 };

		auto preOrderBefore = { 5, 3, 1, 4, 8 };
		auto preOrderAfter = { 5, 3, 1, 8 };

		auto inOrderBefore = { 1, 3, 4, 5, 8 };
		auto inOrderAfter = { 1, 3, 5, 8 };

		auto postOrderBefore = { 1, 4, 3, 8, 5 };
		auto postOrderAfter = { 1, 3, 8, 5 };

		auto levelOrderBefore = { 5, 3, 8, 1, 4 };
		auto levelOrderAfter = { 5, 3, 8, 1 };

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);

		auto node = tree.find(4);
		ASSERT_EQ(*node, 4);
		tree.remove(node);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);

		EXPECT_EQ(*tree.root(), 5);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 8);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto preOrderBefore = { 5, 3, 1, 4, 8, 10, 9 };
		auto preOrderAfter = { 5, 3, 1, 4, 10, 9 };

		auto inOrderBefore = { 1, 3, 4, 5, 8, 9, 10 };
		auto inOrderAfter = { 1, 3, 4, 5, 9, 10 };

		auto postOrderBefore = { 1, 4, 3, 9, 10, 8, 5 };
		auto postOrderAfter = { 1, 4, 3, 9, 10, 5 };

		auto levelOrderBefore = { 5, 3, 8, 1, 4, 10, 9 };
		auto levelOrderAfter = { 5, 3, 10, 1, 4, 9 };

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);

		auto node = tree.find(8);
		ASSERT_EQ(*node, 8);
		tree.remove(node);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);

		EXPECT_EQ(*tree.root(), 5);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 10);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4, 10, 9 };

		auto preOrderBefore = { 5, 3, 1, 4, 8, 10, 9 };
		auto preOrderAfter = { 4, 3, 1, 8, 10, 9 };

		auto inOrderBefore = { 1, 3, 4, 5, 8, 9, 10 };
		auto inOrderAfter = { 1, 3, 4, 8, 9, 10 };

		auto postOrderBefore = { 1, 4, 3, 9, 10, 8, 5 };
		auto postOrderAfter = { 1, 3, 9, 10, 8, 4 };

		auto levelOrderBefore = { 5, 3, 8, 1, 4, 10, 9 };
		auto levelOrderAfter = { 4, 3, 8, 1, 10, 9 };

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderBefore);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderBefore);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderBefore);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderBefore);

		auto node = tree.find(5);
		ASSERT_EQ(*node, 5);
		tree.remove(node);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrderAfter);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrderAfter);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrderAfter);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrderAfter);

		EXPECT_EQ(*tree.root(), 4);
		EXPECT_EQ(*tree.minimum(), 1);
		EXPECT_EQ(*tree.maximum(), 10);
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

		SimpleBST<int> tree = { 5, 3, 8, 1, 4, 10, 9 };
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

	TEST_F(
		BinarySearchTreeStructureTest,
		BinarySearchTreeCorrectlyTracksRootMinAndMaxOnRemove
	) {
		/*
					remove tree2 ---> (1)
									 /   \
		        remove tree1 --->  (0)   (2) 
											\
											(3) <--- remove tree3
		*/

		SimpleBST<int> tree1 = { 1, 0, 2, 3 };
		SimpleBST<int> tree2 = { 1, 0, 2, 3 };
		SimpleBST<int> tree3 = { 1, 0, 2, 3 };

		auto it1 = tree1.begin();
		auto it2 = tree2.begin<traversal_order::PRE_ORDER>();
		auto it3 = std::prev(tree3.end());

		ASSERT_EQ(*it1, 0);
		ASSERT_EQ(*it2, 1);
		ASSERT_EQ(*it3, 3);

		tree1.remove(it1);
		tree2.remove(it2);
		tree3.remove(it3);

		EXPECT_EQ(*tree1.minimum(), 1);
		EXPECT_EQ(*tree1.maximum(), 3);
		EXPECT_EQ(*tree1.root(), 1);

		EXPECT_EQ(*tree2.minimum(), 0);
		EXPECT_EQ(*tree2.maximum(), 3);
		EXPECT_EQ(*tree2.root(), 0);

		EXPECT_EQ(*tree3.minimum(), 0);
		EXPECT_EQ(*tree3.maximum(), 2);
		EXPECT_EQ(*tree3.root(), 1);
	}

	TEST_F(
		BinarySearchTreeStructureTest,
		BinarySearchTreePlacesDuplicateElementsInCorrectPosition
	) {
		/* Expected structure (7a, 7b, and 7c have equal keys):
		* 
		*               (4)
		*              /   \
		*            (0)   (7a)
		*                  /  \
		*                (7b)  (12)
		*               /     /
		*             (5)   (8)
		*               \ 
		*               (7c)
		*/

		MultiMapBST<int, std::string> tree = { 
			{ 4, "4"},
			{ 7, "7a"},
			{ 12, "12" },
			{ 7, "7b" },
			{ 5, "5" },
			{ 0, "0" },
			{ 8, "8" },
			{ 7, "7c" }
		};

		std::initializer_list<key_value_pair<int, std::string>> preOrder = {
			{ 4, "4"},
			{ 0, "0" },
			{ 7, "7a"},
			{ 7, "7b"},
			{ 5, "5"},
			{ 7, "7c"},
			{ 12, "12" },
			{ 8, "8" }
		};

		std::initializer_list<key_value_pair<int, std::string>> inOrder = { 
			{ 0, "0" }, 
			{ 4, "4"}, 
			{ 5, "5" }, 
			{ 7, "7c" }, 
			{ 7, "7b" }, 
			{ 7, "7a" }, 
			{ 8, "8" }, 
			{ 12, "12" }
		};

		std::initializer_list<key_value_pair<int, std::string>> postOrder = {
			{ 0, "0" },
			{ 7, "7c" },
			{ 5, "5" },
			{ 7, "7b" },
			{ 8, "8" },
			{ 12, "12" },
			{ 7, "7a" },
			{ 4, "4" }
		};

		std::initializer_list<key_value_pair<int, std::string>> levelOrder = {
			{ 4, "4"},
			{ 0, "0" },
			{ 7, "7a"},
			{ 7, "7b"},
			{ 12, "12" },
			{ 5, "5" },
			{ 8, "8" },
			{ 7, "7c" },
		};

		EXPECT_EQ(tree.size(), 8);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrder);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrder);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrder);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrder);
	}
}