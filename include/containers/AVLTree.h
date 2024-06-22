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

namespace collections {

	template <
		class element_t,
		class compare_t = std::less<element_t>,
		class allocator_t = std::allocator<element_t>
	> 
	class AVLTree : public BinarySearchTree<element_t, compare_t, allocator_t> {
	private:

		using base = BinarySearchTree<element_t, compare_t, allocator_t>;

		using base_node = base::_node_base;
		using bst_node = base::node;
		using avl_node = struct avl_node;

		using alloc_traits = std::allocator_traits<allocator_t>::template rebind_traits<avl_node>;
		using node_allocator_type = alloc_traits::allocator_type;

	public:

		using allocator_type = allocator_t;
		using value_type = allocator_type::value_type;
		using size_type = size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = std::allocator_traits<allocator_t>::pointer;
		using const_pointer = std::allocator_traits<allocator_t>::const_pointer;
		using iterator = base::iterator;
		using const_iterator = base::const_iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using base::base;

	private:

		struct _avl_node : bst_node {
			size_type _height = 0;
		};

		[[nodiscard]] avl_node* allocate() {
			return alloc_traits::allocate(this->_node_allocator, 1);
		}

		void deallocate(avl_node* n) {
			alloc_traits::deallocate(this->_node_allocator, n, 1);
		}

		template <class... Args>
		void constructNode(avl_node* n, Args&&... args) {
			alloc_traits::construct(
				this->_node_allocator, 
				n, 
				std::forward<Args>(args)...
			);
		}

		void destroyNode(avl_node* n) {
			alloc_traits::destroy(this->_node_allocator, n);
		}

		template <class... Args>
		[[nodiscard]] avl_node* createNode(Args&&... args) {
			avl_node* n = allocate();
			constructNode(n, std::forward<Args>(args)...);
			return n;
		}

		template <class... Args>
		iterator insertAt(base_node* hint, Args&&... args) {
			avl_node* child = createNode(std::forward<Args>(args)...);
			base_node* parent = this->getInsertLocation(hint, child->_element);

			size_type before = this->size();
			base_node* result = this->tryInsert(parent, child);

			if (this->size() > before)
				rotateOnInsert(static_cast<avl_node*>(result));

			return iterator(this, result);
		}

		void rotateOnInsert(avl_node* n) {
			throw std::exception("Not yet implemented.");
		}
	};

	static_assert(
		associative_collection<AVLTree<int>>,
		"AVLTree does not implement the associative collection interface."
	);

	static_assert(
		bidirectional_collection<AVLTree<int>>,
		"AVLTree does not meet the requirements for a bidirectional collection."
	);

	static_assert(
		ordered_collection<AVLTree<int>>,
		"AVLTree does not meet the requirements for an ordered collection."
	);
}