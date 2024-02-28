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
	/// QueueConcept defines a concept for enforcing a queue-like interface on
	/// the given type. Queues must meet the follwing requirements:
	/// 
	/// <para>[Other Concept Requirements]</para>
	///	<list type="bullet">
	///		<para><item><term>
	///		Must be a collection (collections::CollectionConcept)
	///		</term></item></para>
	/// </list>
	/// 
	/// <para>[Required Methods]</para>
	/// <list type="bullet">
	///		<para><item><term>enqueue(element)</term></item></para>
	///		<para><item><term>dequeue()</term></item></para>
	///		<para><item><term>element&amp; peek()</term></item></para>
	///		<para><item><term>const element&amp; peek()</term></item></para>
	/// </list>
	/// 
	/// </summary> ------------------------------------------------------------
	template <class container, class element_t>
	concept QueueConcept = CollectionConcept<container, element_t> && 
	requires(container& t1, const container& t2, const element_t& u) {
		{ t1.enqueue(u) };
		{ t1.dequeue() };
		{ t1.peek() } -> std::same_as<element_t&>;
		{ t2.peek() } -> std::same_as<const element_t&>;
	};

	// ------------------------------------------------------------------------
	/// <summary>
	/// IQueue defines an interface for any collection that supports queue
	/// operations for FIFO access. Inheriting types must compile against
	/// the QueueConcept requirement.
	/// </summary>
	/// 
	/// <typeparam name="container">
	/// The underlying container accessed by the queue.
	/// </typeparam>
	/// <typeparam name="element_t">
	/// The element type contained by the queue.
	/// </typeparam> ----------------------------------------------------------
	template <class container, class element_t>
	class IQueue {
	public:
		IQueue() requires(QueueConcept<container, element_t>) = default;
		~IQueue() = default;
	};
}