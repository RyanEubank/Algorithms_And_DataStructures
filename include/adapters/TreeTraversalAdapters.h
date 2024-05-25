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

#include <concepts>

namespace collections {

	enum class traversal_order {
		IN_ORDER,
		PRE_ORDER,
		POST_ORDER,
		LEVEL_ORDER
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// 
	/// </summary>
	/// 
	/// <typeparam name="T">
	/// 
	/// </typeparam>
	/// 
	/// <typeparam name="traversalOrder">
	/// 
	/// </typeparam> ----------------------------------------------------------
	template<
		std::ranges::forward_range T, 
		traversal_order order
	>
	class _tree_adapter {
	private:
		T& _iterable;

	public:
		explicit _tree_adapter(T& iterable) : _iterable(iterable) {}

		auto begin() {
			return _iterable.begin<order>();
		}

		auto end() {
			return _iterable.end();
		}
	};

	template<
		std::ranges::forward_range T, 
		traversal_order order
	>
	class _tree_adapter_move {
	private:
		T _iterable;

	public:

		explicit _tree_adapter_move(T&& iterable) 
			: _iterable(std::move(iterable)) {}

		auto begin() {
			return _iterable.begin<order>();
		}

		auto end() {
			return _iterable.end();
		}
	};

	template <std::ranges::forward_range T>
	_tree_adapter<T, traversal_order::PRE_ORDER> preOrder(T& iterable) {
		return _tree_adapter<T, traversal_order::PRE_ORDER>(iterable);
	}

	template <std::ranges::forward_range T>
	_tree_adapter_move<T, traversal_order::PRE_ORDER> preOrder(T&& iterable) {
		return _tree_adapter_move<T, traversal_order::PRE_ORDER>(std::move(iterable));
	}

	template <std::ranges::forward_range T>
	_tree_adapter<T, traversal_order::POST_ORDER> postOrder(T& iterable) {
		return _tree_adapter<T, traversal_order::POST_ORDER>(iterable);
	}

	template <std::ranges::forward_range T>
	_tree_adapter_move<T, traversal_order::POST_ORDER> postOrder(T&& iterable) {
		return _tree_adapter_move<T, traversal_order::POST_ORDER>(std::move(iterable));
	}

	template <std::ranges::forward_range T>
	_tree_adapter<T, traversal_order::IN_ORDER> inOrder(T& iterable) {
		return _tree_adapter<T, traversal_order::IN_ORDER>(iterable);
	}

	template <std::ranges::forward_range T>
	_tree_adapter_move<T, traversal_order::IN_ORDER> inOrder(T&& iterable) {
		return _tree_adapter_move<T, traversal_order::IN_ORDER>(std::move(iterable));
	}

	template <std::ranges::forward_range T>
	_tree_adapter<T, traversal_order::LEVEL_ORDER> levelOrder(T& iterable) {
		return _tree_adapter<T, traversal_order::LEVEL_ORDER>(iterable);
	}

	template <std::ranges::forward_range T>
	_tree_adapter_move<T, traversal_order::LEVEL_ORDER> levelOrder(T&& iterable) {
		return _tree_adapter_move<T, traversal_order::LEVEL_ORDER>(std::move(iterable));
	}
}