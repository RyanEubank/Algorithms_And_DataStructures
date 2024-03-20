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

#include "../interfaces/ITree.h"
#include "../interfaces/IBinaryTree.h"

namespace collections {

	template<class T, BinaryTreeIteratorTag traversalOrder>
	class _tree_adapter {
	private:
		T& m_iterable;

	public:
		explicit _tree_adapter(T& iterable) : m_iterable(iterable) {}

		auto begin() {
			return m_iterable.begin<traversalOrder>();
		}

		auto end() {
			return m_iterable.end();
		}
	};

	template<class T, BinaryTreeIteratorTag traversalOrder>
	class _tree_adapter_move {
	private:
		T m_iterable;

	public:

		explicit _tree_adapter_move(T&& iterable) 
			: m_iterable(std::move(iterable)) {}

		auto begin() {
			return m_iterable.begin<traversalOrder>();
		}

		auto end() {
			return m_iterable.end();
		}
	};

	template <class T>
	_tree_adapter<T, PRE_ORDER> preOrder(T& iterable) {
		return _tree_adapter<T, PRE_ORDER>(iterable);
	}

	template <class T>
	_tree_adapter_move<T, PRE_ORDER> preOrder(T&& iterable) {
		return _tree_adapter_move<T, PRE_ORDER>(std::move(iterable));
	}

	template <class T>
	_tree_adapter<T, POST_ORDER> postOrder(T& iterable) {
		return _tree_adapter<T, POST_ORDER>(iterable);
	}

	template <class T>
	_tree_adapter_move<T, POST_ORDER> postOrder(T&& iterable) {
		return _tree_adapter_move<T, POST_ORDER>(std::move(iterable));
	}

	template <class T>
	_tree_adapter<T, IN_ORDER> inOrder(T& iterable) {
		return _tree_adapter<T, IN_ORDER>(iterable);
	}

	template <class T>
	_tree_adapter_move<T, IN_ORDER> inOrder(T&& iterable) {
		return _tree_adapter_move<T, IN_ORDER>(std::move(iterable));
	}

	template <class T>
	_tree_adapter<T, LEVEL_ORDER> levelOrder(T& iterable) {
		return _tree_adapter<T, LEVEL_ORDER>(iterable);
	}

	template <class T>
	_tree_adapter_move<T, LEVEL_ORDER> levelOrder(T&& iterable) {
		return _tree_adapter_move<T, LEVEL_ORDER>(std::move(iterable));
	}
}