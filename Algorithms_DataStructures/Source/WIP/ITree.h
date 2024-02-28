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

#include "ICollection.h"

namespace collections {

	struct PRE_ORDER {};
	struct POST_ORDER {};
	struct LEVEL_ORDER {};

	// ------------------------------------------------------------------------
	/// <summary>
	/// TreeIteratorTag defines a concept to require the given type be
	/// only one of the follwing tags:
	/// 
	/// <list type="bullet">
	///		<para><item><term>collections::PRE_ORDER</term></item></para>
	///		<para><item><term></term>collections::POST_ORDER</item></para>
	///		<para><item><term></term>collections::LEVEL_ORDER</item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class T>
	concept TreeIteratorTag = 
		std::same_as<T, PRE_ORDER> ||
		std::same_as<T, POST_ORDER> || 
		std::same_as<T, LEVEL_ORDER>;

	// ------------------------------------------------------------------------
	/// <summary>
	/// TreeConcept defines a concept for enforcing a tree-like interface on
	/// the given type. Trees must meet the following requirements:
	/// 
	/// <para>[Other Concept Requirements]</para>
	///	<list type="bullet">
	///		<para><item><term>
	///		Must be a collection (collections::CollectionConcept)
	///		</term></item></para>
	/// 
	///		<para><item><term>
	///		Must be a forward range at minimum (std::forward_range)
	///		</term></item></para>
	/// </list>
	/// 
	/// <para>[Required alias or typedefs]</para>
	/// <list type="bullet">
	///		<item><term>tree::iterator</term></item>
	///		<item><term>tree::const_iterator</term></item>
	/// </list>
	/// 
	/// <para>[Required Methods]</para>
	/// <list type="bullet">
	///		<para><item><term>
	///		iterator begin&lt;PRE_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	/// 	<para><item><term>
	///		iterator begin&lt;POST_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	/// 	<para><item><term>
	///		iterator begin&lt;LEVEL_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	/// 	<para><item><term>
	///		const_iterator begin&lt;PRE_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	/// 	<para><item><term>
	///		const_iterator begin&lt;POST_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	/// 	<para><item><term>
	///		const_iterator begin&lt;LEVEL_ORDER&gt;(element, index)
	///		</term></item></para>
	/// 
	/// 	<para><item><term>size_t height()</term></item></para>
	///		<para><item><term>insert(element)</term></item></para>
	///		<para><item><term>remove(element)</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class collection, class element_t>
	concept TreeConcept = CollectionConcept<collection, element_t> &&
	std::ranges::forward_range<collection> &&
	requires(collection& t1, const collection& t2, const element_t& e) {
		typename collection::iterator;
		typename collection::const_iterator;

		{ t1.template begin<PRE_ORDER>() }
			-> std::same_as<typename collection::iterator>;
		{ t2.template begin<PRE_ORDER>() }
			-> std::same_as<typename collection::const_iterator>;

		{ t1.template begin<POST_ORDER>() }
			-> std::same_as<typename collection::iterator>;
		{ t2.template begin<POST_ORDER>() }
			-> std::same_as<typename collection::const_iterator>;

		{ t1.template begin<LEVEL_ORDER>() }
			-> std::same_as<typename collection::iterator>;
		{ t2.template begin<LEVEL_ORDER>() }
			-> std::same_as<typename collection::const_iterator>;

		{ t2.height() } -> std::same_as<size_t>;
		{ t1.insert(e) };
		{ t1.remove(e) };
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// ITree defines an interface for any collection that supports tree
	/// operations for efficient iteration, search, and insert/remove.
	/// Inheriting types must compile against the TreeConcept requirement.
	/// </summary>
	/// 
	/// <typeparam name="derived">
	/// The inheriting dervied type.
	/// </typeparam>
	/// <typeparam name="element_t">
	/// The element type contained by the tree.
	/// </typeparam> ----------------------------------------------------------
	template <class derived, class element_t>
	class ITree {
	public:
		ITree() requires(TreeConcept<derived, element_t>) = default;
		~ITree() = default;
	};
}