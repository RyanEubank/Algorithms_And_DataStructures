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

namespace collections {

	struct IN_ORDER {};

	// ------------------------------------------------------------------------
	/// <summary>
	/// BinaryTreeIteratorTag defines an extension concept to require the 
	/// given type be any of the tags in TreeIteratorTag or the type
	/// collections::IN_ORDER.
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept BinaryTreeIteratorTag = 
		TreeIteratorTag<T> || std::same_as<T, IN_ORDER>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// BinaryTreeConcept defines an extension concept to TreeConcept for 
	/// enforcing a binary-tree interface the given type. Trees must meet the 
	/// following requirements:
	/// 
	/// <para>[Other Concept Requirements]</para>
	///	<list type="bullet">
	///		<para><item><term>
	///		Must be a tree (collections::TreeConcept)
	///		</term></item></para>
	/// </list>
	/// 
	/// <para>[Required Methods]</para>
	/// <list type="bullet">
	///		<para><item><term>
	///		iterator begin&lt;IN_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	///		<para><item><term>
	///		const_iterator begin&lt;IN_ORDER&gt;(element, index)
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class collection, class element_t>
	concept BinaryTreeConcept = TreeConcept<collection, element_t> &&
	requires(collection &t1, const collection &t2) {
		{ t1.template begin<IN_ORDER>() } ->
			std::same_as<typename collection::iterator>;
		{ t2.template begin<IN_ORDER>() } ->
			std::same_as<typename collection::const_iterator>;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// IBinaryTree defines an interface for any collection that supports 
	/// binary tree operations. Inheriting types must compile against the 
	/// BinaryTreeConcept requirement.
	/// </summary>
	/// 
	/// <typeparam name="derived">
	/// The inheriting dervied type.
	/// </typeparam>
	/// <typeparam name="element_t">
	/// The element type contained by the tree.
	/// </typeparam> ----------------------------------------------------------
	template <class derived, class element_t>
	class IBinaryTree : public ITree<derived, element_t> {
	public:

		IBinaryTree() requires(BinaryTreeConcept<derived, element_t>)
			= default;
		~IBinaryTree() = default;
	};
}
