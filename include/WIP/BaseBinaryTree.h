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

#include "../interfaces/IBinaryTree.h"
#include "../adapters/Queue.h"

namespace collections {

	template<class derived, class element_t>
	class BaseBinaryTree :
		public BaseCollection<derived, element_t>,
		public ITree<derived, element_t>
	{
	protected:

		struct _Node {
			_Node* m_left;
			_Node* m_right;
			element_t m_element;

			_Node(const element_t& element) :
				m_element(element),
				m_left(nullptr),
				m_right(nullptr)
			{}

			bool isLeaf() const {
				return !degree();
			}

			uint8_t degree() const {
				uint8_t degree = 0;
				if (m_left)
					degree++;
				if (m_right)
					degree++;
				return degree;
			}
		};

		using E = element_t;
		using Node = struct _Node;
		
		Node* m_root;

		void deleteSubtreeAt(Node* node) {
			Queue<Node*> queue = BinaryTreeIterator<false>
				(node, POST_ORDER{}).m_queue;

			for (size_t i = 0; i < this->m_size; i++) {
				Node* next = queue.peek();
				queue.dequeue();
				delete next;
			}
		}
		
		Node* copySubtreeAt(Node* node) {
			if (!node)
				throw std::exception("Subtree is null.\n");
			
			LinkedList<Node*> list;

			Node* new_root = new Node(node->m_element);
			Node* current_new = new_root;
			Node* current_old = node;

			list.insertLast(current_new);
			list.insertLast(current_old);

			while (!list.isEmpty()) {
				current_old = list.last();
				list.removeLast();
				current_new = list.last();
				list.removeLast();

				if (current_old->m_right) {
					Node* right = new Node(current_old->m_right->m_element);
					current_new->m_right = right;
					list.insertLast(current_new->m_right);
					list.insertLast(current_old->m_right);
				}
				if (current_old->m_left) {
					Node* left = new Node(current_old->m_left->m_element);
					current_new->m_left = left;
					list.insertLast(current_new->m_left);
					list.insertLast(current_old->m_left);
				}
			}

			return new_root;
		}

		int32_t heightOf(Node* node) const {
			if (!node)
				return -1;

			int32_t height = -1;
			size_t numNodesInLevel = 0;
			Node* current = nullptr;

			Queue<Node*> queue;
			queue.enqueue(node);

			while (!queue.isEmpty()) {
				height++;

				numNodesInLevel = queue.size();
				while (numNodesInLevel--) {
					current = queue.peek();
					queue.dequeue();
					if (current->m_left)
						queue.enqueue(current->m_left);
					if (current->m_right)
						queue.enqueue(current->m_right);
				}
			}
			return height;
		}

	public:

		template <bool isConst>
		class BinaryTreeIterator;

		using iterator = BinaryTreeIterator<false>;
		using const_iterator = BinaryTreeIterator<true>;

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Default Constructor ~~~
		/// 
		///	<para>
		/// Constructs an empty binary tree with zero size.
		/// </para></summary> -------------------------------------------------
		BaseBinaryTree() : m_root(nullptr) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Constructor ~~~
		/// 
		/// <para>
		/// Constructs a deep copy of the specified binary tree.
		/// </para></summary>
		/// 
		/// <param name="copy">
		/// The binary tree to be copied.
		/// </param> ----------------------------------------------------------
		BaseBinaryTree(const BaseBinaryTree& copy) : BaseBinaryTree() {
			this->m_root = copySubtreeAt(copy.m_root);
			this->m_size = copy.m_size;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Move Constructor ~~~
		/// 
		/// <para>
		/// Constructs a binary tree by moving the data from the provided
		/// object into the new one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The binary tree to be moved into this one.
		/// </param> ----------------------------------------------------------
		BaseBinaryTree(BaseBinaryTree&& other) noexcept : BaseBinaryTree() {
			swap(*this, other);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a binary tree with the a copy of the elements in the 
		/// specified initialization list.
		/// </summary>
		/// 
		/// <param name="init">
		/// The initialization list to copy elements from.
		/// </param> ----------------------------------------------------------
		BaseBinaryTree(std::initializer_list<E> init) :
			BaseBinaryTree(init.begin(), init.size()) {}

		// --------------------------------------------------------------------
		/// <summary>
		/// Constructs a binary with the a copy of the elements in the 
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
		BaseBinaryTree(const E* array, size_t size) : BaseBinaryTree() {
			for (const E* e = array; e != array + size; e++)
				THIS_DERIVED->insert(*e);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Destructor ~~~
		/// 
		/// <para>
		/// Destroys the tree safely releasing memory for each node.
		/// </para></summary> -------------------------------------------------
		~BaseBinaryTree() {
			clear();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Copy Assignment Operator ~~~
		/// 
		/// <para>
		/// Deep copies the data from the specified binary tree to this one.
		/// </para></summary>
		/// 
		/// <param name="other">
		/// The binary tree to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns this binary tree with the copied data.
		/// </returns> --------------------------------------------------------
		BaseBinaryTree& operator=(BaseBinaryTree other) {
			swap(*this, other);
			return *this;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Removes and deletes all nodes from the linked list emptying its 
		/// contents.
		/// </summary> --------------------------------------------------------
		void clear() {
			if (!this->isEmpty())
				deleteSubtreeAt(m_root);
			this->m_size = 0;
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns the height of the tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the height of the binary tree's root.
		/// </returns> --------------------------------------------------------
		size_t height() const {
			return heightOf(m_root);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the beginning of the binary tree.
		/// </summary>
		/// 
		/// <typeparam name="traversalOrder">
		/// The ordering in which the tree will be traversed by the returned
		/// iterator.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a forward iterator to the first element for the specified
		/// traversal in the binary tree.
		/// </returns> --------------------------------------------------------
		template <BinaryTreeIteratorTag traversalOrder = PRE_ORDER>
		iterator begin() {
			return BinaryTreeIterator<false>(m_root, traversalOrder{});
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns an iterator pointing to the end of the binary tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a forward iterator to the location after the last element 
		/// in the binary tree.
		/// </returns> --------------------------------------------------------
		iterator end() {
			return BinaryTreeIterator<false>();
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the beginning of the 
		/// binary tree.
		/// </summary>
		/// 
		/// <typeparam name="traversalOrder">
		/// The ordering in which the tree will be traversed by the returned
		/// iterator.
		/// </typeparam>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the first element for the 
		/// specified traversal in the binary tree.
		/// </returns> --------------------------------------------------------
		template <BinaryTreeIteratorTag traversalOrder = PRE_ORDER>
		const_iterator begin() const {
			return BinaryTreeIterator<true>(m_root, traversalOrder{});
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// Returns a constant iterator pointing to the end of the binary tree.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a constant forward iterator to the location after the last 
		/// element in the binary tree.
		/// </returns> --------------------------------------------------------
		const_iterator end() const {
			return BinaryTreeIterator<true>();
		}

		// --------------------------------------------------------------------
		/// <summary> 
		/// Swaps the contents of the given binary trees.
		/// </summary>
		/// 
		/// <param name="a">
		/// The first tree to be swapped.
		/// </param>
		/// 
		/// <param name="b">
		/// The second tree to be swapped.
		/// </param> ----------------------------------------------------------
		friend void swap(
			BaseBinaryTree<derived, E>& a,
			BaseBinaryTree<derived, E>& b
		) noexcept {
			using std::swap;
			swap(a.m_size, b.m_size);
			swap(a.m_root, b.m_root);
		}

		// --------------------------------------------------------------------
		/// <summary>
		/// ~~~ Output Stream Operator ~~~
		/// </summary>
		/// 
		/// <typeparam name="charT">
		/// The type of the character stream returned by the operator.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being written to.
		/// </param>
		/// <param name="arr">
		/// The DynamicArray being written out.
		/// </param>
		/// 
		/// <returns>
		/// Returns the output stream written with the given DynamicArray.
		/// </returns> --------------------------------------------------------
		template <typename charT>
		friend std::basic_ostream<charT>& operator<<(
			std::basic_ostream<charT>& os,
			const BaseBinaryTree<derived, E>& tree
		) {
			os << "BinaryTree<" << typeid(E).name() << ">"
				<< " (size = " << tree.size() << ") "
				<< std::endl;

			tree.writeToStream(os);
			return os;
		}

		// ----------------------------------------------------------------
		/// <summary>
		/// ~~~ Equality Operator ~~~
		/// </summary>
		///
		/// <param name="lhs">
		/// The binary tree appearing on the left-hand side of the 
		/// '=='
		/// operator.
		/// </param>
		/// <param name="rhs">
		/// The binary tree appearing on the right-hand side of the 
		/// '=='
		/// operator.
		/// </param>
		///
		/// <returns>
		/// Returns true if the binary trees have the same
		///	</returns> ----------------------------------------------------
		friend bool operator==(const derived& lhs,const derived& rhs) {
			return collections::isLexicographicallyEqual<derived>(rhs, lhs);
		}

		template <bool isConst>
		class BinaryTreeIterator {
		private:

			using E = element_t;

			Queue<Node*> m_queue;

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a BinaryTreeIterator pointing to the specified node 
			/// in a binary tree.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> 
			/// <param name="order">
			/// Tag indicating the iterator will traverse the tree via
			/// pre-order tree traversal.
			/// </param> ------------------------------------------------------
			BinaryTreeIterator(Node* node, PRE_ORDER order) : m_queue() {
				if (!node)
					return;

				LinkedList<Node*> list;
				list.insertLast(node);

				while (!list.isEmpty()) {
					Node* current = list.last();
					list.removeLast();

					if (current->m_right)
						list.insertLast(current->m_right);
					if (current->m_left)
						list.insertLast(current->m_left);

					m_queue.enqueue(current);
				}
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a BinaryTreeIterator pointing to the specified node 
			/// in a binary tree.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> 
			/// <param name="order">
			/// Tag indicating the iterator will traverse the tree via
			/// post-order tree traversal.
			/// </param> ------------------------------------------------------
			BinaryTreeIterator(Node* node, POST_ORDER order) : m_queue() {
				if (!node)
					return;

				LinkedList<Node*> list;
				list.insertFirst(node);
				Node* current = node;
				
				do {
					list.removeLast();
					list.insertFirst(current);
					
					if (current->m_left)
						list.insertLast(current->m_left);
					if (current->m_right)
						list.insertLast(current->m_right);

					current = list.last();
				} while (current != node);

				for (const auto& node : list)
					m_queue.enqueue(node);
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a BinaryTreeIterator pointing to the specified node 
			/// in a binary tree.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> 
			/// <param name="order">
			/// Tag indicating the iterator will traverse the tree via
			/// level-order tree traversal.
			/// </param> ------------------------------------------------------
			BinaryTreeIterator(Node* node, LEVEL_ORDER order) : m_queue() {
				if (!node)
					return;

				LinkedList<Node*> list;
				list.insertFirst(node);

				while (!list.isEmpty()) {
					Node* current = list.first();
					list.removeFirst();

					if (current->m_left)
						list.insertLast(current->m_left);
					if (current->m_right)
						list.insertLast(current->m_right);

					m_queue.enqueue(current);
				}
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// Constructs a BinaryTreeIterator pointing to the specified node 
			/// in a binary tree.
			///	</summary>
			///
			/// <param name="node">
			/// The node the iterator will point to.
			/// </param> 
			/// <param name="order">
			/// Tag indicating the iterator will traverse the tree via
			/// in-order tree traversal.
			/// </param> ------------------------------------------------------
			BinaryTreeIterator(Node* node, IN_ORDER order) : m_queue() {
				if (!node)
					return;

				LinkedList<Node*> list;
				traverseLeft(node, list);

				while (!list.isEmpty()) {
					Node* current = list.last();
					list.removeLast();
					traverseLeft(current->m_right, list);
					m_queue.enqueue(current);
				}
			}

			void traverseLeft(Node* node, LinkedList<Node*>& list) {
				while (node) {
					list.insertLast(node);
					node = node->m_left;
				}
			}

			friend class BaseBinaryTree<derived, E>;

		public:

			using value_type = std::conditional_t<isConst, const E, E>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Default Constructor ~~~
			///
			///	<para>
			/// Constructs an empty BinaryTreeIterator.
			/// </para></summary> ---------------------------------------------
			BinaryTreeIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Constructor ~~~
			///
			///	<para>
			/// Constructs a copy of the given BinaryTreeIterator.
			/// </para></summary> ---------------------------------------------
			BinaryTreeIterator(const BinaryTreeIterator& copy) = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// Destructs the BinaryTreeIterator.
			///	</summary> ----------------------------------------------------
			~BinaryTreeIterator() = default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Copy Assignment Operator ~~~
			///
			/// <para>
			///
			/// </para></summary>
			///
			/// <param name="other">
			/// The BinaryTreeIterator to copy from.
			/// </param>
			///
			/// <returns>
			/// Returns this BinaryTreeIterator with the copied data.
			/// </returns> ----------------------------------------------------
			BinaryTreeIterator& operator=(const BinaryTreeIterator& other) 
				= default;

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Implicit Conversion Constructor ~~~
			///
			/// <para>
			/// Constructs a copy of the given BinaryTreeIterator for implicit 
			/// conversion from non-const version to a const 
			/// BinaryTreeIterator.
			/// </para></summary>
			///
			/// <param name="other">
			/// The non-const BinaryTreeIterator to copy from.
			/// </param>
			///
			/// <typeparam name="wasConst">
			/// The 'const'-ness of the provided BinaryTreeIterator to copy.
			/// </typeparam> --------------------------------------------------
			template<
				bool wasConst,
				class = std::enable_if_t<isConst && !wasConst>
			>
			BinaryTreeIterator(BinaryTreeIterator<wasConst> copy) 
				: m_queue(copy.m_list) {}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Dereference Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Returns a reference to the element pointed to by the iterator
			/// in its current state.
			///	</returns> ----------------------------------------------------
			reference operator*() const {
				return m_queue.peek()->m_element;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Pre-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns the iterator
			/// after updating.
			///	</returns> ----------------------------------------------------
			BinaryTreeIterator& operator++() {
				m_queue.dequeue();
				return *this;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Post-Increment Operator ~~~
			/// </summary>
			///
			/// <returns>
			/// Moves the iterator to the next element and returns a copy of
			/// the iterator before updating.
			///	</returns> ----------------------------------------------------
			BinaryTreeIterator operator++(int) {
				auto copy = *this;
				m_queue.dequeue();
				return copy;
			}

			// ----------------------------------------------------------------
			/// <summary>
			/// ~~~ Equality Operator ~~~
			/// </summary>
			///
			/// <param name="lhs">
			/// The BinaryTreeIterator appearing on the left-hand side of the 
			/// '=='
			/// operator.
			/// </param>
			/// <param name="rhs">
			/// The BinaryTreeIterator appearing on the right-hand side of the 
			/// '=='
			/// operator.
			/// </param>
			///
			/// <returns>
			/// Returns true if the BinaryTreeIterators are pointing to the
			/// same location in the tree.
			///	</returns> ----------------------------------------------------
			friend bool operator==(
				const BinaryTreeIterator& lhs,
				const BinaryTreeIterator& rhs
			) {
				return lhs.m_queue == rhs.m_queue;
			}
		};
	};
}
