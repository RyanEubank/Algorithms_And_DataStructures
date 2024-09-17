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
#include <type_traits>

#include "collection.h"

namespace collections {

	// --------------------------------------------------------------------
	/// <summary><para>
	/// Defines the miniimal set of methods and operators for sequential
	/// collection types.
	/// </para>
	/// 
	/// Required methods:
	/// 
	///	<list type="bullet">
	///		<para><item><term>
	///			reference front()
	///		</term></item></para>
	///		<para><item><term>
	///			const_reference front() const
	///		</term></item></para>
	///		<para><item><term>
	///			reference back()
	///		</term></item></para>
	///		<para><item><term>
	///			const_reference back() const
	///		</term></item></para>
	///		<para><item><term>
	///			void insertFront(const lvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			void insertFront(rvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			void insertBack(const lvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			void insertBack(rvalue reference)
	///		</term></item></para>
	///		<para><item><term>
	///			void removeFront()
	///		</term></item></para>
	///		<para><item><term>
	///			void removeBack()
	///		</term></item></para>
	///		<para><item><term>
	///			void emplaceFront(Args&lt;T&gt;...)
	///		</term></item></para>
	///		<para><item><term>
	///			void emplaceBack(Args&lt;T&gt;...)
	///		</term></item></para>
	/// </list>
	/// 
	/// </summary> --------------------------------------------------------
	template <class T,class ...Args>
	concept sequential = collection_type_traits<T> &&
	requires (
		T& obj1,
		const T& obj2,
		typename T::const_reference lval,
		typename T::value_type&& rval,
		Args&&... args
	) {
		{ obj1.front() }	-> std::convertible_to<typename T::reference>;
		{ obj2.front() }	-> std::convertible_to<typename T::const_reference>;
		{ obj1.back() }		-> std::convertible_to<typename T::reference>;
		{ obj2.back() }		-> std::convertible_to<typename T::const_reference>;
		{ obj1.insertFront(lval) };
		{ obj1.insertFront(rval) };
		{ obj1.insertBack(lval) };
		{ obj1.insertBack(rval) };
		{ obj1.removeFront() };
		{ obj1.removeBack() };
		{ obj1.emplaceFront(args...) };
		{ obj1.emplaceBack(args...) };
	};
}