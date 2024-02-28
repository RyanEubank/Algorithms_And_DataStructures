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

#include "BaseBinaryTree.h"

namespace collections {

	template<class element_t>
	class BinarySearchTree :
		public BaseBinaryTree<BinarySearchTree<element_t>, element_t>
	{
	protected:

		using E = element_t;
		using base = BaseBinaryTree<BinarySearchTree<E>, E>;
		using Node = base::Node;

		Node* nextChildInDirectionOf(const E& target, const Node* node) const {
			if (target < node->m_element)
				return node->m_left;
			if (target > node->m_element)
				return node->m_right;
			return nullptr;
		}

		void insertAtParent(Node* parent, Node* child) {
			if (child->m_element < parent->m_element)
				parent->m_left = child;
			else if (child->m_element > parent->m_element)
				parent->m_right = child;
			else
				throw std::exception(DUPLICATE_ERROR);
		}

		bool removeAtParent(Node* parent, const E& element) {
			if (element < parent->m_element && parent->m_left)
				parent->m_left = replaceNode(parent->m_left);
			else if (element > parent->m_element && parent->m_right)
				parent->m_right = replaceNode(parent->m_right);
			else if (element == parent->m_element)
				this->m_root = replaceNode(this->m_root);
			else
				return false;
			return true;
		}

		void swapLinks(const Node* root, Node* predecessor, Node* parent) {
			if (predecessor != root->m_left) {
				parent->m_right = predecessor->m_left;
				predecessor->m_left = root->m_left;
			}
			predecessor->m_right = root->m_right;
		}

	private:

		static constexpr auto DUPLICATE_ERROR
			= "Operation failed: duplicate element.\n";

		const Node* parentOf(const E& element, const Node* root) const {
			Node* child = nextChildInDirectionOf(element, root);

			while (child && child->m_element != element) {
				root = child;
				child = nextChildInDirectionOf(element, root);
			}

			return root;
		}

		Node* parentOf(const E& element, const Node* root) {
			const Node* node = (std::as_const(*this).parentOf(element, root));
			return const_cast<Node*>(node);
		}

		Node* replaceNode(Node* node) {
			Node* replacement = replacementFor(node);
			delete node;
			return replacement;
		}

		Node* replacementFor(const Node* node) {
			Node* replacement = nullptr;
			size_t degree = node->degree();

			if (degree == 1)
				replacement = (node->m_left) ? node->m_left : node->m_right;
			if (degree == 2) 
				replacement = inOrderPredecessor(node);

			return replacement;
		}

		Node* inOrderPredecessor(const Node* root) {
			Node* parent = root->m_left;
			Node* predecessor = parent;

			while (predecessor->m_right) {
				parent = predecessor;
				predecessor = predecessor->m_right;
			}

			swapLinks(root, predecessor, parent);

			return predecessor;
		}

	public:

		using iterator = base::iterator;
		using const_iterator = base::const_iterator;
		using base::base;

		// --------------------------------------------------------------------
		/// <summary>
		/// Inserts the specified element into the tree. Throws an exception
		/// if the element is a duplicate.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be inserted into the tree.
		/// </param> ----------------------------------------------------------
		void insert(const E& element) {
			if (this->isEmpty())
				this->m_root = new Node(element);
			else {
				Node* parent = parentOf(element, this->m_root);
				insertAtParent(parent, new Node(element));
			}

			this->m_size++;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes the specified element from the tree if it exists.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be removed from the tree.
		/// </param> ----------------------------------------------------------
		void remove(const E& element) {
			if (this->isEmpty())
				return;

			Node* parent = parentOf(element, this->m_root);
			if (removeAtParent(parent, element))
				this->m_size--;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns whether the tree contains the specified element.
		/// </summary>
		/// 
		/// <param name="element">
		/// The element to be searched for.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the element exists in the tree, false otherwise.
		/// </returns> --------------------------------------------------------
		bool contains(const E& element) const {
			return !parentOf(element, this->m_root)->isLeaf();
		}
	};
}