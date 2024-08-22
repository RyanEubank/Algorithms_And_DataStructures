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

#include <array>
#include <vector>

#include "adapters/TreeTraversalAdapters.h"
#include "containers/SplayTree.h"

namespace collection_tests {

	using namespace collections;

	class SplayTreeStructureTest : public BinaryTreeStructureTest {
	protected:
		std::array<int, 10> elements = { 2, 5, 1, 0, 6, 3, 9, 8, 7, 4 };     
	};

	TEST_F(SplayTreeStructureTest, InsertSplaysNewElementsToTheRoot) {
		SplayTree<int> tree{};

		EXPECT_EQ(tree.root(), tree.end());

		for (const auto& e : this->elements) {
			tree.insert(e);
			EXPECT_EQ(*tree.root(), e);
		}
	}

	TEST_F(SplayTreeStructureTest, TreeMaintainsExpectedStructureOnInsert) {
		/* Insertion structure:
		* 
		*		 insert 5	  (5)  insert 1   (1)    insert 0    (0)     CONT.  
		*	(2)  --------->    /   --------->    \   --------->     \    -----> 
		*                    (2)                 (2)                (1)         
		*                                          \                  \         
		*                                          (5)                (2)       
		*                                                               \       
		*                                                               (5)     
		* 
		*   (0)                      (6)                   (3)
		*     \       insert 6      /     insert 3       /     \      CONT.
		*     (1)     --------->  (1)     --------->   (1)     (6)    ------>
		*       \                /  \                 /  \     / 
		*       (2)             (0) (5)              (0) (2) (5)
		*         \                 /  
		*         (5)             (2)
		* 
		* 
		*      (3)                   (9)                    (8)
		*    /     \   insert 9      /     insert 8        /  \      CONT.
		*   (1)    (6) --------->   (6)    --------->     (6) (9)    ------>
		*  /  \    /                /                    /
		* (0) (2) (5)             (3)                   (3)
		*                        /   \                 /  \
		*                      (1)   (5)             (1)  (5)
		*                     /  \                   / \
		*                    (0) (2)               (0) (2)
		* 
		* 
		*          (8)                  (7)                       (4)
		*          / \    insert 7      / \      insert 4        /  \
		*        (6) (9)  --------->  (6) (8)    --------->    (3)  (6)
		*        /                    /     \                 /    /  \
		*      (3)                  (3)     (9)              (1)  (5) (7)
		*      / \                  / \                     /  \        \
		*    (1) (5)              (1) (5)                 (0)  (2)      (8)
		*    / \                  / \                                      \
		*  (0) (2)              (0) (2)                                    (9)
		*/   

		SplayTree<int> tree{};

		std::vector<std::vector<int>> expectedInOrder = {
			{ 2 },
			{ 2, 5 },
			{ 1, 2, 5 },
			{ 0, 1, 2, 5 },
			{ 0, 1, 2, 5, 6 },
			{ 0, 1, 2, 3, 5, 6 },
			{ 0, 1, 2, 3, 5, 6, 9 },
			{ 0, 1, 2, 3, 5, 6, 8, 9 },
			{ 0, 1, 2, 3, 5, 6, 7, 8, 9 },
			{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
		};

		std::vector<std::vector<int>> expectedPreOrder = {
			{ 2 },
			{ 5, 2 },
			{ 1, 2, 5 },
			{ 0, 1, 2, 5 },
			{ 6, 1, 0, 5, 2 },
			{ 3, 1, 0, 2, 6, 5 },
			{ 9, 6, 3, 1, 0, 2, 5 },
			{ 8, 6, 3, 1, 0, 2, 5, 9 },
			{ 7, 6, 3, 1, 0, 2, 5, 8, 9 },
			{ 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 }
		};


		for (int i = 0; i < this->elements.size(); ++i) {
			tree.insert(this->elements[i]);
			this->testInOrderSequence(tree, expectedInOrder[i]);
			this->testPreOrderSequence(tree, expectedPreOrder[i]);
		}
	}

	TEST_F(SplayTreeStructureTest, TreeSplaysWhenInsertingExistingElement) {
		/* Tree Structure
		*
		*       (4)                      (5)                         (9)
		*      /   \     insert 5       /  \      insert 9           /    CONT.
		*     (3)  (6)   --------->    (4) (6)    --------->       (6)    ----->
		*     /    /  \                /     \                    /   \
		*    (1)  (5) (7)             (3)    (7)                (5)    (8)
		*   /  \        \            /         \                /      /
		*  (0)  (2)      (8)        (1)        (8)            (4)    (7)
		*                  \       /  \         \             /
		*                  (9)	  (0) (2)       (9)         (3)
		*                                                   / 
		*                                                 (1) 
		*                                                 / \
		*                                               (0) (2)                        
		* 
		*               (9)                    (2)                       
		*               /     insert 2        /    \         CONT.
		*             (6)     --------->    (1)    (6)       ----->      
		*            /   \                  /     /    \          
		*          (5)    (8)              (0)  (4)     (9)             
		*          /      /                     / \     /
		*        (4)    (7)                   (3) (5)  (8)
		*        /                                     /
		*      (3)                                   (7)
		*      /                                 
		*    (1)                                
		*    / \
		*   (0) (2)
		* 
		* 
		*           (2)                         (3)
		*          /    \        insert 3       / \
		*         (1)    (6)     --------->   (2) (4)   
		*        /     /    \                 /     \
		*       (0)  (4)     (9)            (1)     (6)
		*            / \     /              /       / \
		*          (3) (5)  (8)            (0)    (5) (9)
		*                   /                         /
		*                 (7)                        (8)
		*                                            /
		*                                           (7)
		*/  

		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto existingElements = { 5, 9, 2, 3 };

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		std::vector<std::vector<int>> expectedPreOrder = {
			{ 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 },
			{ 5, 4, 3, 1, 0, 2, 6, 7, 8, 9 },
			{ 9, 6, 5, 4, 3, 1, 0, 2, 8, 7 },
			{ 2, 1, 0, 6, 4, 3, 5, 9, 8, 7 },
			{ 3, 2, 1, 0, 4, 6, 5, 9, 8, 7 }
		};

		int i = 0; 
		for (auto& e : existingElements) {
			this->testInOrderSequence(tree, expectedInOrder);
			this->testPreOrderSequence(tree, expectedPreOrder[i]);
			tree.insert(e);
			++i;
		}

	}

	TEST_F(SplayTreeStructureTest, TreeMaintainsExpectedStructureOnRemove) {
		/* Removal structure:
		* 
		*       (4)                      (4)                       (4)
		*      /   \     remove 0       /    \     remove 1       /  \     CONT.
		*     (3)  (6)   --------->   (1)    (6)   --------->   (3)  (6)   ---->
		*     /    /  \                 \    /  \               /    /  \
		*    (1)  (5) (7)               (3) (5) (7)            (2)  (5) (7)
		*   /  \        \               /         \                       \
		*  (0)  (2)      (8)           (2)        (8)                     (8)
		*                  \                        \                       \
		*                  (9)	                    (9)                    (9)
		*	 
		*		 
		*    (4)                    (7)                   (5)
		*    /  \     remove 8     /   \    remove 6     /  \      CONT.
		*  (3)  (6)   --------->  (4)  (9)  ---------> (4)  (7)    ----->
		*  /    / \              /  \                  /      \
		* (2)  (5) (7)          (3)  (6)              (3)     (9)
		*            \          /   /                 /
		*            (8)      (2)  (5)               (2)
		*             \
		*             (9)
		* 
		*       (5)                    (4)                     (7)
		*       /  \    remove 5       / \      remove 9       /
		*     (4)  (7)  --------->   (3) (7)    --------->   (4)
		*     /      \               /     \                 /
		*   (3)      (9)            (2)    (9)             (3)
		*   /                                              /
		*  (2)                                            (2)
		*/             

		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		// removal order: 0, 1, 8, 6, 5, 9
		auto positions = {
			tree.begin(),
			std::next(tree.begin()),
			std::next(tree.begin(), 8),
			std::next(tree.begin(), 6),
			std::next(tree.begin(), 5),
			std::next(tree.begin(), 9),
		};

		std::vector<std::vector<int>> expectedInOrder = {
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
			{ 2, 3, 4, 5, 6, 7, 8, 9 },
			{ 2, 3, 4, 5, 6, 7, 9 },
			{ 2, 3, 4, 5, 7, 9 },
			{ 2, 3, 4, 7, 9 },
			{ 2, 3, 4, 7 }
		};

		std::vector<std::vector<int>> expectedPreOrder = {
			{ 4, 1, 3, 2, 6, 5, 7, 8, 9 },
			{ 4, 3, 2, 6, 5, 7, 8, 9 },
			{ 7, 4, 3, 2, 6, 5, 9 },
			{ 5, 4, 3, 2, 7, 9 },
			{ 4, 3, 2, 7, 9 },
			{ 7, 4, 3, 2 }
		};

		int i = 0; 
		for (auto& pos : positions) {
			tree.remove(pos);
			this->testInOrderSequence(tree, expectedInOrder[i]);
			this->testPreOrderSequence(tree, expectedPreOrder[i]);
			++i;
		}
	}

	TEST_F(SplayTreeStructureTest, TreeMaintainsExpectedStructureOnSearch) {

		/* Search structure:
		*
		*       (4)                      (5)                         (9)
		*      /   \     search 5       /  \      search 9           /    CONT.
		*     (3)  (6)   --------->    (4) (6)    --------->       (6)    ----->
		*     /    /  \                /     \                    /   \
		*    (1)  (5) (7)             (3)    (7)                (5)    (8)
		*   /  \        \            /         \                /      /
		*  (0)  (2)      (8)        (1)        (8)            (4)    (7)
		*                  \       /  \         \             /
		*                  (9)	  (0) (2)       (9)         (3)
		*                                                   / 
		*                                                 (1) 
		*                                                 / \
		*                                               (0) (2)                        
		* 
		*               (9)                    (2)                       
		*               /     search 2        /    \         CONT.
		*             (6)     --------->    (1)    (6)       ----->      
		*            /   \                  /     /    \          
		*          (5)    (8)              (0)  (4)     (9)             
		*          /      /                     / \     /
		*        (4)    (7)                   (3) (5)  (8)
		*        /                                     /
		*      (3)                                   (7)
		*      /                                 
		*    (1)                                
		*    / \
		*   (0) (2)
		* 
		* 
		*           (2)                         (3)
		*          /    \        search 3       / \
		*         (1)    (6)     --------->   (2) (4)   
		*        /     /    \                 /     \
		*       (0)  (4)     (9)            (1)     (6)
		*            / \     /              /       / \
		*          (3) (5)  (8)            (0)    (5) (9)
		*                   /                         /
		*                 (7)                        (8)
		*                                            /
		*                                           (7)
		*/  

		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto searchElements = { 5, 9, 2, 3 };

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		std::vector<std::vector<int>> expectedPreOrder = {
			{ 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 },
			{ 5, 4, 3, 1, 0, 2, 6, 7, 8, 9 },
			{ 9, 6, 5, 4, 3, 1, 0, 2, 8, 7 },
			{ 2, 1, 0, 6, 4, 3, 5, 9, 8, 7 },
			{ 3, 2, 1, 0, 4, 6, 5, 9, 8, 7 }
		};

		int i = 0; 
		for (auto& e : searchElements) {
			this->testInOrderSequence(tree, expectedInOrder);
			this->testPreOrderSequence(tree, expectedPreOrder[i]);
			tree.find(e);
			++i;
		}
	}

	TEST_F(SplayTreeStructureTest, ConstTreeDoesNotSplayOnSearch) {
		const SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto searchElements = { 5, 9, 2, 3, 15, 12, 8 };
		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto expectedPreOrder = { 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 };

		int i = 0; 
		for (auto& e : searchElements) {
			this->testInOrderSequence(tree, expectedInOrder);
			this->testPreOrderSequence(tree, expectedPreOrder);
			tree.find(e);
			++i;
		}
	}

	TEST_F(SplayTreeStructureTest, TreeSplaysParentOfNonExistingSearchItems) {

		/* Search structure:
		*
		*       (8)                       (0)
		*      /   \       search 1         \ 
		*     (6)  (12)    --------->        (8)
		*     /    /  \                     /   \
		*    (2)  (10) (14)               (2)   (12)
		*   /  \         \                 \    /   \
		*  (0)  (4)      (16)              (6) (10) (14)
		*                  \               /          \
		*                  (18)          (4)	     (16)
		*                                               \
		*                                              (18)
		*/  

		SplayTree<int> tree = { 4, 10, 2, 0, 12, 6, 18, 16, 14, 8 };

		auto expectedInOrder = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };

		auto expectedPreOrderBefore = { 8, 6, 2, 0, 4, 12, 10, 14, 16, 18 };
		auto expectedPreOrderAfter = { 0, 8, 2, 6, 4, 12, 10, 14, 16, 18 };

		this->testInOrderSequence(tree, expectedInOrder);
		this->testPreOrderSequence(tree, expectedPreOrderBefore);

		tree.find(1);
			
		this->testInOrderSequence(tree, expectedInOrder);
		this->testPreOrderSequence(tree, expectedPreOrderAfter);
	}

	TEST_F(SplayTreeStructureTest, TreeMaintainsExpectedStructureOnSearchLowerBound) {

		/* Search structure:
		*
		*       (4)                      (5)                         (9)
		*      /   \     search 5       /  \      search 9           /    CONT.
		*     (3)  (6)   --------->    (4) (6)    --------->       (6)    ----->
		*     /    /  \                /     \                    /   \
		*    (1)  (5) (7)             (3)    (7)                (5)    (8)
		*   /  \        \            /         \                /      /
		*  (0)  (2)      (8)        (1)        (8)            (4)    (7)
		*                  \       /  \         \             /
		*                  (9)	  (0) (2)       (9)         (3)
		*                                                   / 
		*                                                 (1) 
		*                                                 / \
		*                                               (0) (2)                        
		* 
		*               (9)                    (2)                       
		*               /     search 2        /    \         CONT.
		*             (6)     --------->    (1)    (6)       ----->      
		*            /   \                  /     /    \          
		*          (5)    (8)              (0)  (4)     (9)             
		*          /      /                     / \     /
		*        (4)    (7)                   (3) (5)  (8)
		*        /                                     /
		*      (3)                                   (7)
		*      /                                 
		*    (1)                                
		*    / \
		*   (0) (2)
		* 
		* 
		*           (2)                         (3)
		*          /    \        search 3       / \
		*         (1)    (6)     --------->   (2) (4)   
		*        /     /    \                 /     \
		*       (0)  (4)     (9)            (1)     (6)
		*            / \     /              /       / \
		*          (3) (5)  (8)            (0)    (5) (9)
		*                   /                         /
		*                 (7)                        (8)
		*                                            /
		*                                           (7)
		*/  

		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto searchElements = { 5, 9, 2, 3 };

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		std::vector<std::vector<int>> expectedPreOrder = {
			{ 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 },
			{ 5, 4, 3, 1, 0, 2, 6, 7, 8, 9 },
			{ 9, 6, 5, 4, 3, 1, 0, 2, 8, 7 },
			{ 2, 1, 0, 6, 4, 3, 5, 9, 8, 7 },
			{ 3, 2, 1, 0, 4, 6, 5, 9, 8, 7 }
		};

		int i = 0; 
		for (auto& e : searchElements) {
			this->testInOrderSequence(tree, expectedInOrder);
			this->testPreOrderSequence(tree, expectedPreOrder[i]);
			tree.lowerBound(e);
			++i;
		}
	}

	TEST_F(
		SplayTreeStructureTest, 
		TreeMaintainsExpectedStructureOnSearchNonExistingLowerBound
	) {
		/* Search structure:
		*
		*       (4)                               (9)     
		*      /   \     search 10                /  
		*     (3)  (6)   --------->             (6)
		*     /    /  \                       /     \
		*    (1)  (5) (7)                  (4)      (8)
		*   /  \        \                 /   \      /
		*  (0)  (2)      (8)             (3)  (5)  (7)
		*                  \            /
		*                  (9)	      (1)
		*                            /   \
		*                           (0)  (2)
		*/
	  
		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto expectedPreOrder = { 9, 6, 4, 3, 1, 0, 2, 5, 8, 7 };

		auto result = tree.lowerBound(10);
		this->testInOrderSequence(tree, expectedInOrder);
		this->testPreOrderSequence(tree, expectedPreOrder);
	}

	TEST_F(SplayTreeStructureTest, TreeMaintainsExpectedStructureOnSearchUpperBound) {

		/* Search structure:
		*
		*       (4)                      (5)                         (9)
		*      /   \     search 4       /  \      search 8           /    CONT.
		*     (3)  (6)   --------->    (4) (6)    --------->       (6)    ----->
		*     /    /  \                /     \                    /   \
		*    (1)  (5) (7)             (3)    (7)                (5)    (8)
		*   /  \        \            /         \                /      /
		*  (0)  (2)      (8)        (1)        (8)            (4)    (7)
		*                  \       /  \         \             /
		*                  (9)	  (0) (2)       (9)         (3)
		*                                                   / 
		*                                                 (1) 
		*                                                 / \
		*                                               (0) (2)                        
		* 
		*               (9)                    (2)                       
		*               /     search 1        /    \         CONT.
		*             (6)     --------->    (1)    (6)       ----->      
		*            /   \                  /     /    \          
		*          (5)    (8)              (0)  (4)     (9)             
		*          /      /                     / \     /
		*        (4)    (7)                   (3) (5)  (8)
		*        /                                     /
		*      (3)                                   (7)
		*      /                                 
		*    (1)                                
		*    / \
		*   (0) (2)
		* 
		* 
		*           (2)                         (3)
		*          /    \        search 2       / \
		*         (1)    (6)     --------->   (2) (4)   
		*        /     /    \                 /     \
		*       (0)  (4)     (9)            (1)     (6)
		*            / \     /              /       / \
		*          (3) (5)  (8)            (0)    (5) (9)
		*                   /                         /
		*                 (7)                        (8)
		*                                            /
		*                                           (7)
		*/  

		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto searchElements = { 4, 8, 1, 2 };

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		std::vector<std::vector<int>> expectedPreOrder = {
			{ 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 },
			{ 5, 4, 3, 1, 0, 2, 6, 7, 8, 9 },
			{ 9, 6, 5, 4, 3, 1, 0, 2, 8, 7 },
			{ 2, 1, 0, 6, 4, 3, 5, 9, 8, 7 },
			{ 3, 2, 1, 0, 4, 6, 5, 9, 8, 7 }
		};

		int i = 0; 
		for (auto& e : searchElements) {
			this->testInOrderSequence(tree, expectedInOrder);
			this->testPreOrderSequence(tree, expectedPreOrder[i]);
			tree.upperBound(e);
			++i;
		}
	}

	TEST_F(
		SplayTreeStructureTest, 
		TreeMaintainsExpectedStructureOnSearchNonExistingUpperBound
	) {
		/* Search structure:
		*
		*       (4)                               (9)     
		*      /   \     search 10                /  
		*     (3)  (6)   --------->             (6)
		*     /    /  \                       /     \
		*    (1)  (5) (7)                  (4)      (8)
		*   /  \        \                 /   \      /
		*  (0)  (2)      (8)             (3)  (5)  (7)
		*                  \            /
		*                  (9)	      (1)
		*                            /   \
		*                           (0)  (2)
		*/

		SplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto expectedPreOrder = { 9, 6, 4, 3, 1, 0, 2, 5, 8, 7 };

		auto result = tree.upperBound(10);
		this->testInOrderSequence(tree, expectedInOrder);
		this->testPreOrderSequence(tree, expectedPreOrder);
	}
}