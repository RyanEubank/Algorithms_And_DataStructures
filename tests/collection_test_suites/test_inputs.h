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

#include <array>
#include <concepts>
#include <string>
#include <type_traits>

#include "concepts/collection.h"
#include "util/key_value_pair.h"

namespace collection_tests {
	
	using kv_pair = collections::key_value_pair<const uint8_t, std::string>;

	template <class T>
	struct test_case_data;

	template <collections::collection T>
	struct test_case_data<T> {
		const T a = { 0 };
		const T b = { 1 };
		const T c = { 2 };
		const T d = { 3 };
		const T e = { 4 };
		const T f = { 5 };
		const T g = { 6 };
		const T h = { 7 };
		const T i = { 8 };
		const T j = { 9 };
	};

	template <>
	struct test_case_data<kv_pair> {
		const kv_pair a = { 0, "0" };
		const kv_pair b = { 1, "1" };
		const kv_pair c = { 2, "2" };
		const kv_pair d = { 3, "3" };
		const kv_pair e = { 4, "4" };
		const kv_pair f = { 5, "5" };
		const kv_pair g = { 6, "6" };
		const kv_pair h = { 7, "7" };
		const kv_pair i = { 8, "8" };
		const kv_pair j = { 9, "9" };
	};


	template <std::integral T>
	struct test_case_data<T> {
		const T a = 0;
		const T b = 1;
		const T c = 2;
		const T d = 3;
		const T e = 4;
		const T f = 5;
		const T g = 6;
		const T h = 7;
		const T i = 8;
		const T j = 9;
	};

	template <std::floating_point T>
	struct test_case_data<T> {
		const T a = 0.0f;
		const T b = 1.0f;
		const T c = 2.0f;
		const T d = 3.0f;
		const T e = 4.0f;
		const T f = 5.0f;
		const T g = 6.0f;
		const T h = 7.0f;
		const T i = 8.0f;
		const T j = 9.0f;
	};

	template <>
	struct test_case_data<void*> {
	private:
		int i0 = 0;
		int i1 = 1;
		int	i2 = 2;
		int	i3 = 3;
		int	i4 = 4;
		int	i5 = 5;
		int	i6 = 6;
		int	i7 = 7;
		int	i8 = 8;
		int	i9 = 9;

	public:
		void* a = &i0;
		void* b = &i1;
		void* c = &i2; 
		void* d = &i3; 
		void* e = &i4; 
		void* f = &i5;
		void* g = &i6;
		void* h = &i7;
		void* i = &i8;
		void* j = &i9;
	};

	template <>
	struct test_case_data<std::string> {
		const std::string a = "a";
		const std::string b = "b";
		const std::string c = "c";
		const std::string d = "d";
		const std::string e = "e";
		const std::string f = "f";
		const std::string g = "g";
		const std::string h = "h";
		const std::string i = "i";
		const std::string j = "j";
	};

	template <class T>
	const test_case_data<T> test_data;
}