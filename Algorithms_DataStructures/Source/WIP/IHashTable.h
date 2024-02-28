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

	// ------------------------------------------------------------------------
	/// <summary>
	/// Hashable is a concept to enforce that a type can be hashed
	/// via std::hash and returns a size_t as a result.
	/// </summary> ------------------------------------------------------------
	template<typename T>
	concept Hashable = requires(T a) {
		{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// HashTableConcept defines a concept for enforcing a hash table interface 
	/// on the given type. Hash tables must meet the following requirements:
	/// 
	/// <para>[Required Methods]</para>
	/// <list type="bullet">
	///		<para><item><term>void insert(element)</term></item></para>
	///		<para><item><term></term>void remove(element)</item></para>
	///		<para><item><term></term>float loadFactor()</item></para>
	///		<para><item><term></term>void resize(capacity)</item></para>
	///		<para><item><term></term>void rehash(hashFunction)</item></para>
	///		<para><item><term></term>
	///			void resizeAndRehash(capacity, hashFunction)
	///		</item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class collection, class key_t, class value_t>
	concept HashTableConcept =
		CollectionConcept<collection, value_t> &&
		std::ranges::forward_range<collection> &&
		Hashable<key_t> &&
	requires (
		collection& t1,
		const collection& t2,
		const key_t& k,
		const value_t& v,
		size_t capacity,
		std::function<size_t(value_t)> func
	) {
		typename collection::iterator;
		typename collection::const_iterator;
		{ t1.insert(k, v) };
		{ t1.remove(k) };
		{ t2.loadFactor() } -> std::convertible_to<float>;
		{ t1.resize(capacity) };
		{ t1.rehash(func) };
		{ t1.resizeAndRehash(capacity, func) };
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// IHashTable defines an interface for any collection that supports set
	/// operations for constant time search, insertion and removal.
	/// Inheriting types must compile against the HashTableConcept requirement.
	/// </summary>
	/// 
	/// <typeparam name="derived">
	/// The inheriting dervied type.
	/// </typeparam>
	/// <typeparam name="key_t">
	/// The key type mapped by the hashmap.
	/// </typeparam>
	/// <typeparam name="value_t">
	/// The element type contained by the hashmap.
	/// </typeparam> ---------------------------------------------------------- 
	template <class derived, class key_t, class value_t>
	class IHashTable {
	public:
		IHashTable() requires(HashTableConcept<derived, key_t, value_t>) 
			= default;
		~IHashTable() = default;
	};
}