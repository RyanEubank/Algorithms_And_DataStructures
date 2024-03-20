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

#include "BinarySearchTree.h"
#include "../adapters/Stack.h"

namespace collections {

	template <class element_t>
	class AVLTree final : public BinarySearchTree<element_t> {
	private:

		using E = element_t;
		using BaseNode = BinarySearchTree<E>::Node;

		struct Node : public BaseNode {
			int8_t m_balance;

			Node(const E& element) : m_balance(0), BaseNode(element) {}

			Node* right() {
				return asAVLNode(this->m_right);
			}

			Node* left() {
				return asAVLNode(this->m_left);
			}
		};

		enum Rotation : uint8_t {
			RIGHTRIGHT = 0b00,
			RIGHTLEFT = 0b01,
			LEFTRIGHT = 0b10,
			LEFTLEFT = 0b11
		};

		static Node* asAVLNode(BaseNode* node) {
			return static_cast<Node*>(node);
		}

		static Node* optionalPeekOrNull(const Stack<Node*>& stack) {
			return stack.isEmpty() ? nullptr : stack.peek();
		}

		static void peekNextAndParent(
			Node** next, 
			Node** parent, 
			Stack<Node*>& stack
		) {
			*next = stack.peek();
			stack.pop();
			*parent = optionalPeekOrNull(stack);
		}

		Node* getRoot() {
			return asAVLNode(this->m_root);
		}

		Node* nextChildToward(const E& key, Node* node) {
			return asAVLNode(this->nextChildInDirectionOf(key, node));
		}

		Stack<Node*> traverseTo(const E& key, Node* from) {
			Stack<Node*> stack;
			while (from && from->m_element != key) {
				stack.push(from);
				from = nextChildToward(key, from);
			}
			return stack;
		}

		Stack<Node*> insert(const E& key, Node* node) {
			Stack<Node*> path = traverseTo(key, node);
			insertAt(path, key);
			return path;
		}

		void insertAt(const Stack<Node*>& path, const E& key) {
			Node* parent = path.isEmpty() ? getRoot() : path.peek();
			this->insertAtParent(parent, new Node(key));
		}

		void rebalanceOnInsert(Stack<Node*>& path, const E& key) {
			Node *next, *parent;

			while (!path.isEmpty()) {
				peekNextAndParent(&next, &parent, path);
				if (tryRebalanceOnInsert(next, parent, key))
					break;
				next->m_balance += (key > next->m_element) ? 1 : -1;
			}
		}

		bool tryRebalanceOnInsert(Node* next, Node* parent, const E& key) {
			if (next->m_balance != 0) {
				if (!isBenificialInsert(next, key))
					rotateOnInsert(parent, next, key);
				else
					next->m_balance += (key > next->m_element) ? 1 : -1;
				return true;
			}
			return false;
		}

		bool isBenificialInsert(const Node* node, const E& key) {
			return ((key < node->m_element) && (node->m_balance > 0)) ||
				((key > node->m_element) && (node->m_balance < 0));
		}

		void rotateOnInsert(Node* parent, Node* pivot, const E& key) {
			Rotation direction = rotationDirectionForInsert(pivot, key);

			if (parent && key < parent->m_element)
				parent->m_left = rotate(pivot, direction);
			else if (parent && key > parent->m_element)
				parent->m_right = rotate(pivot, direction);
			else
				this->m_root = rotate(pivot, direction);
		}

		Rotation rotationDirectionForInsert(Node* pivot, const E& key) {
			if (key < pivot->m_element) {
				return (key < pivot->m_left->m_element)
					? LEFTLEFT : LEFTRIGHT;
			}
			else {
				return (key < pivot->m_right->m_element)
					? RIGHTLEFT : RIGHTRIGHT;
			}
		}

		Stack<Node*> remove(E& key, Node* node) {
			Stack<Node*> path = traverseTo(key, node);
			removeAt(path, key);
			return path;
		}

		void removeAt(Stack<Node*>& path, E& key) {
			if (path.isEmpty()) {
				this->m_root = replaceAndDelete(getRoot(), path, key);
				return;
			}
			Node* parent = path.peek();
			removeAtParent(parent, path, key);
		}

		void removeAtParent(Node* parent, Stack<Node*>& path, E& key) {
			if (parent->m_left && (key < parent->m_element))
				parent->m_left = replaceAndDelete(parent->left(), path, key);
			else if (parent->m_right && (key > parent->m_element))
				parent->m_right = replaceAndDelete(parent->right(), path, key);
			else
				path.clear();
		}
		
		Node* replaceAndDelete(Node* node, Stack<Node*>& path, E& key) {
			Node* replacement = nullptr;
			uint8_t degree = node->degree();
		
			if (degree == 1) 
				replacement = (node->m_left) ? node->left() : node->right();
			if (degree == 2) {
				replacement = inOrderPredecessor(node, path);
				key = replacement->m_element;
			}
		
			delete node;
			return replacement;
		}

		Node* inOrderPredecessor(Node* node, Stack<Node*>& path) {
			Node *predecessor, *parent;
			Stack<Node*> pathToPredecessor = traverseTo(
				node->m_element, node->left()
			);
		
			peekNextAndParent(&predecessor, &parent, pathToPredecessor);
			this->swapLinks(node, predecessor, parent);
			predecessor->m_balance = node->m_balance;
			path.push(predecessor);
			pathToPredecessor.placeOnTopOf(path);
		
			return predecessor;
		}

		void rebalanceOnRemove(Stack<Node*>& path, const E& key) {
			Node *next, *parent;
			E keyLastUpdated = key;

			while (!path.isEmpty()) {
				peekNextAndParent(&next, &parent, path);
				if (tryRebalanceOnRemove(next, parent, keyLastUpdated)) 
					break;
			}
		}

		bool tryRebalanceOnRemove(Node* pivot, Node* parent, E& key) {
			pivot->m_balance += (key <= pivot->m_element) ? 1 : -1;
			
			if (std::abs(pivot->m_balance) > 1) {
				Node* newPivot = rotateOnRemove(pivot, parent, key);
				key = newPivot->m_element;
			}

			if (!parent)
				return true;

			return (key <= parent->m_element) 
				? parent->left()->m_balance != 0 
				: parent->right()->m_balance != 0;
		}

		Node* rotateOnRemove(Node* pivot, Node* parent, const E& key) {
			Rotation direction = rotationDirectionForRemove(pivot, key);
			Node* newPivot = rotate(pivot, direction);

			if (parent && key <= parent->m_element) 
				parent->m_left = newPivot;
			else if (parent && key > parent->m_element) 
				parent->m_right = newPivot;
			else
				this->m_root = newPivot;

			return newPivot;
		}

		Rotation rotationDirectionForRemove(Node* pivot, const E& key) {
			if (key <= pivot->m_element) {
				return (pivot->right()->m_balance >= 0) 
					? RIGHTRIGHT : RIGHTLEFT;
			}
			else {
				return (pivot->left()->m_balance <= 0) 
					? LEFTLEFT : LEFTRIGHT;
			}
		}

		Node* rotate(Node* pivot, Rotation direction) {
			bool isLeftChildRotation = (direction & (1 << 1));
			bool isLeftGrandchildRotation = (direction & (1 << 0));

			Node* child = isLeftChildRotation 
				? pivot->left() : pivot->right();
			Node* grandchild = isLeftGrandchildRotation 
				? child->left() : child->right();
	
			recalculateBalanceOnRotation(pivot, child, grandchild, direction);
			return rotatePivot(pivot, direction);
		}

		void recalculateBalanceOnRotation(
			Node* pivot,
			Node* child,
			Node* grandchild,
			Rotation direction
		) {
			switch (direction) {
			case (LEFTLEFT):
				pivot->m_balance = (child->m_right && !pivot->m_right) ? -1 : 0;
				child->m_balance = (pivot->m_balance == 0) ? 0 : 1;
				break;
			case (LEFTRIGHT):
				pivot->m_balance = (grandchild->m_balance < 0) ? 1 : 0;
				child->m_balance = (grandchild->m_balance > 0) ? -1 : 0;
				grandchild->m_balance = 0;
				break;
			case (RIGHTLEFT):
				pivot->m_balance = (grandchild->m_balance > 0) ? -1 : 0;
				child->m_balance = (grandchild->m_balance < 0) ? 1 : 0;
				grandchild->m_balance = 0;
				break;
			case (RIGHTRIGHT):
				pivot->m_balance = (child->m_left && !pivot->m_left) ? 1 : 0;
				child->m_balance = (pivot->m_balance == 0) ? 0 : -1;
			};
		}
	
		Node* rotatePivot(Node* pivot, Rotation direction) {
			switch (direction) {
				case (LEFTLEFT): return singleRightRotation(pivot);
				case (RIGHTRIGHT): return singleLeftRotation(pivot);
				case (LEFTRIGHT): return leftRightRotation(pivot);
				case (RIGHTLEFT): return rightLeftRotation(pivot);
				default: throw std::exception("Executed unreachable code.\n");
			};
		}
	
		Node* singleRightRotation(BaseNode* node) {
			BaseNode* leftChild = node->m_left;
			node->m_left = leftChild->m_right;
			leftChild->m_right = node;
			return asAVLNode(leftChild);
		}
	
		Node* singleLeftRotation(BaseNode* node) {
			BaseNode* rightChild = node->m_right;
			node->m_right = rightChild->m_left;
			rightChild->m_left = node;
			return asAVLNode(rightChild);
		}
	
		Node* leftRightRotation(BaseNode* node) {
			node->m_left = singleLeftRotation(node->m_left);
			return singleRightRotation(node);
		}
	
		Node* rightLeftRotation(BaseNode* node) {
			node->m_right = singleRightRotation(node->m_right);
			return singleLeftRotation(node);
		}

	public:

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty AVL tree with zero size.
		/// </para></summary> -------------------------------------------------
		AVLTree() : BinarySearchTree<E>() {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Constructor ~~~
		/// 
		/// <para>
		/// Constructs a deep copy of the specified AVL tree.
		/// </para></summary>
		/// 
		/// <param name="copy">
		/// The AVL tree to be copied.
		/// </param> ----------------------------------------------------------
		AVLTree(const AVLTree& copy) : BinarySearchTree<E>() {
			this->m_root = this->copySubtreeAt(copy.m_root);
			this->m_size = copy.m_size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Constructor ~~~
		/// 
		/// <para>
		/// Constructs an AVL tree by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The AVL tree to be moved into this one.
		/// </param> ----------------------------------------------------------
		AVLTree(AVLTree&& other) noexcept : BinarySearchTree<E>() {
			swap(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs an AVL tree with the a copy of the elements in the 
		/// specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		AVLTree(std::initializer_list<E> init) :
			AVLTree(init.begin(), init.size()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs an AVL tree with the a copy of the elements in the 
		/// specified array.
		/// </summary>
		/// 
		/// <param name="array">
		/// The generic array to copy elements from.
		/// </param>
		/// 
		/// <param name="size">
		/// The size of the array being copied.
		/// </param> ----------------------------------------------------------
		AVLTree(const E* array, size_t size) : BinarySearchTree<E>() {
			for (const E* e = array; e != array + size; e++)
				this->insert(*e);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified AVL tree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The AVL tree to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this AVL tree with the copied data.
		/// </returns> --------------------------------------------------------
		AVLTree& operator=(AVLTree other) {
			swap(*this, other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element into the tree and automatically
		/// rebalances it. Throws an exception if the element is a duplicate.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted into the tree.
		/// </param> ----------------------------------------------------------
		void insert(const E& element) {
			if (this->isEmpty())
				this->m_root = new Node(element);
			else {
				Stack<Node*> path = insert(element, asAVLNode(this->m_root));
				rebalanceOnInsert(path, element);
			}
			
			this->m_size++;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the specified element from the tree if it exists and
		/// automatically rebalances it.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be removed from the tree.
		/// </param> ----------------------------------------------------------
		void remove(const E& element) {
			if (this->isEmpty())
				return;
			else {
				E key = element;
				Stack<Node*> path = remove(key, asAVLNode(this->m_root));
				if (!path.isEmpty()) {
					rebalanceOnRemove(path, key);
					this->m_size--;
				}
			}
		}
	};
}