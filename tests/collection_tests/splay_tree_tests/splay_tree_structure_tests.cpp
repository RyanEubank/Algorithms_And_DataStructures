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
#include "containers/SplayTree.h"

#include "../../collection_test_suites/collection_test_fixture.h"

namespace collection_tests {

	using namespace collections;

	class SplayTreeStructureTest : public CollectionTest<SimpleSplayTree<int>> {
	protected:
		std::array<int, 10> elements = { 2, 5, 1, 0, 6, 3, 9, 8, 7, 4 };     
	};

	TEST_F(SplayTreeStructureTest, InsertSplaysNewElementsToTheRoot) {
		SimpleSplayTree<int> tree{};

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

		SimpleSplayTree<int> tree{};

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
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder[i]);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder[i]);
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

		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

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
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder[i]);
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

		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

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
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder[i]);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder[i]);
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

		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

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
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder[i]);
			tree.find(e);
			++i;
		}
	}

	TEST_F(SplayTreeStructureTest, ConstTreeDoesNotSplayOnSearch) {
		const SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto searchElements = { 5, 9, 2, 3, 15, 12, 8 };
		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto expectedPreOrder = { 4, 3, 1, 0, 2, 6, 5, 7, 8, 9 };

		int i = 0; 
		for (auto& e : searchElements) {
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder);
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

		SimpleSplayTree<int> tree = { 4, 10, 2, 0, 12, 6, 18, 16, 14, 8 };

		auto expectedInOrder = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };

		auto expectedPreOrderBefore = { 8, 6, 2, 0, 4, 12, 10, 14, 16, 18 };
		auto expectedPreOrderAfter = { 0, 8, 2, 6, 4, 12, 10, 14, 16, 18 };

		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrderBefore);

		tree.find(1);
		
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrderAfter);
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

		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

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
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder[i]);
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
	  
		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto expectedPreOrder = { 9, 6, 4, 3, 1, 0, 2, 5, 8, 7 };

		tree.lowerBound(10);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder);
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

		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

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
			this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
			this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder[i]);
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

		SimpleSplayTree<int> tree(this->elements.begin(), this->elements.end());

		auto expectedInOrder = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		auto expectedPreOrder = { 9, 6, 4, 3, 1, 0, 2, 5, 8, 7 };

		tree.upperBound(10);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), expectedInOrder);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), expectedPreOrder);
	}

	TEST_F(
		SplayTreeStructureTest,
		TreePlacesDuplicateElementsInCorrectPosition
	) {
		/* Expected structure (7a, 7b, and 7c have equal keys):
		* 
		*              (7c)
		*            /      \
		*          (4)       (8)
		*         /   \     /   \
		*       (0)  (5)  (7b)  (12)
		*                   \
		*                   (7a)
		*/

		MultiMapSplayTree<int, std::string> tree = { 
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
			{ 7, "7c"},
			{ 4, "4"},
			{ 0, "0" },
			{ 5, "5" },
			{ 8, "8" },
			{ 7, "7b" },
			{ 7, "7a"},
			{ 12, "12" }
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
			{ 5, "5" },
			{ 4, "4"}, 
			{ 7, "7a"},
			{ 7, "7b" },
			{ 12, "12" },
			{ 8, "8" },
			{ 7, "7c" }
		};

		std::initializer_list<key_value_pair<int, std::string>> levelOrder = {
			{ 7, "7c" },
			{ 4, "4"},
			{ 8, "8" },
			{ 0, "0" },
			{ 5, "5" },
			{ 7, "7b" },
			{ 12, "12" },
			{ 7, "7a"}
		};

		EXPECT_EQ(tree.size(), 8);
		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrder);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrder);
		this->expectSequence(tree.begin<traversal_order::POST_ORDER>(), tree.end(), postOrder);
		this->expectSequence(tree.begin<traversal_order::LEVEL_ORDER>(), tree.end(), levelOrder);
	}

	TEST_F(
		SplayTreeStructureTest,
		TreeSplaysDuplicateElementsOnSearch
	) {
		/* Expected structure (2a and 2b have equal keys):
		* 
		*     (1)       search(2)      (2b)
		*        \      --------->     /   \
		*        (2b)                (1)   (2a)
		*          \
		*          (2a)
		*/

		MultiMapSplayTree<int, std::string> tree 
			= { {2, "2a"}, {2, "2b"}, {1, "1"} };

		std::initializer_list<key_value_pair<int, std::string>> preOrder = {
			{2, "2b"}, {1, "1"}, {2, "2a"}
		};

		std::initializer_list<key_value_pair<int, std::string>> inOrder = { 
			{1, "1"}, {2, "2b"}, {2, "2a"} 
		};

		tree.find(2);

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrder);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrder);
	}

	TEST_F(
		SplayTreeStructureTest,
		TreeSplaysDuplicateElementsOnSearchLowerBound
	) {
		/* Expected structure (2a and 2b have equal keys):
		* 
		*     (1)       lowerBound(2)      (2b)
		*        \      --------->        /   \
		*        (2b)                   (1)   (2a)
		*          \
		*          (2a)
		*/

		MultiMapSplayTree<int, std::string> tree 
			= { {2, "2a"}, {2, "2b"}, {1, "1"} };

		std::initializer_list<key_value_pair<int, std::string>> preOrder = {
			{2, "2b"}, {1, "1"}, {2, "2a"}
		};

		std::initializer_list<key_value_pair<int, std::string>> inOrder = { 
			{1, "1"}, {2, "2b"}, {2, "2a"} 
		};

		tree.lowerBound(2); //splays 2b

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrder);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrder);
	}


	TEST_F(
		SplayTreeStructureTest,
		TreeSplaysDuplicateElementsOnSearchUpperBound
	) {
		/* Expected structure (2a and 2b have equal keys):
		* 
		*     (1)       upperBound(2)      (2a)
		*        \      --------->        / 
		*        (2b)                   (2b)
		*          \                    /
		*          (2a)               (1)
		*/

		MultiMapSplayTree<int, std::string> tree 
			= { {2, "2a"}, {2, "2b"}, {1, "1"} };

		std::initializer_list<key_value_pair<int, std::string>> preOrder = {
			{2, "2a"}, {2, "2b"}, {1, "1"}
		};

		std::initializer_list<key_value_pair<int, std::string>> inOrder = { 
			{1, "1"}, {2, "2b"}, {2, "2a"} 
		};

		tree.upperBound(2); /// splays 2a

		this->expectSequence(tree.begin<traversal_order::PRE_ORDER>(), tree.end(), preOrder);
		this->expectSequence(tree.begin<traversal_order::IN_ORDER>(), tree.end(), inOrder);
	}
}