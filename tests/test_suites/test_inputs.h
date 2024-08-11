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

#include "concepts/collection.h"

namespace collection_tests {
	
	template <class T>
	struct test_case_data;

	template <collections::collection T>
	struct test_case_data<T> {
		const T a = { 1 };
		const T b = { 2 };
		const T c = { 3 };
		const T d = { 4 };
		const T e = { 5 };
		const T f = { 6 };

		auto control() const {
			return std::array<T, 3>{ a, b, c };
		}

		auto different_elements() const {
			return std::array<T, 3>{ d, e, f };
		}

		auto reverse_order() const {
			return std::array<T, 3>{ c, b, a };
		}

		auto different_size() const {
			return std::array<T, 2>{ a, b };
		}
	};

	template <std::integral T>
	struct test_case_data<T> {
		const T a = 1;
		const T b = 2;
		const T c = 3;
		const T d = 4;
		const T e = 5;
		const T f = 6;

		auto control() const {
			return std::array<T, 3>{ a, b, c };
		}

		auto different_elements() const {
			return std::array<T, 3>{ d, e, f };
		}

		auto reverse_order() const {
			return std::array<T, 3>{ c, b, a };
		}

		auto different_size() const {
			return std::array<T, 2>{ a, b };
		}
	};

	template <std::floating_point T>
	struct test_case_data<T> {
		const T a = 1.0f;
		const T b = 2.0f;
		const T c = 3.0f;
		const T d = 4.0f;
		const T e = 5.0f;
		const T f = 6.0f;

		auto control() const {
			return std::array<T, 3>{ a, b, c };
		}

		auto different_elements() const {
			return std::array<T, 3>{ d, e, f };
		}

		auto reverse_order() const {
			return std::array<T, 3>{ c, b, a };
		}

		auto different_size() const {
			return std::array<T, 2>{ a, b };
		}
	};

	template <>
	struct test_case_data<void*> {
	private:
		int i1 = 1, i2 = 2, i3 = 3, i4 = 4, i5 = 5, i6 = 6;

	public:
		void* a = &i1;
		void* b = &i2;
		void* c = &i3; 
		void* d = &i4; 
		void* e = &i5; 
		void* f = &i6;

		auto control() const {
			return std::array<void*, 3>{ a, b, c };
		}

		auto different_elements() const {
			return std::array<void*, 3>{ d, e, f };
		}

		auto reverse_order() const {
			return std::array<void*, 3>{ c, b, a };
		}

		auto different_size() const {
			return std::array<void*, 2>{ a, b };
		}
	};

	template <>
	struct test_case_data<std::string> {
		const std::string a = "a";
		const std::string b = "b";
		const std::string c = "c";
		const std::string d = "d";
		const std::string e = "e";
		const std::string f = "f";

		auto control() const {
			return std::array<std::string, 3>{ a, b, c };
		}

		auto different_elements() const {
			return std::array<std::string, 3>{ d, e, f };
		}

		auto reverse_order() const {
			return std::array<std::string, 3>{ c, b, a };
		}

		auto different_size() const {
			return std::array<std::string, 2>{ a, b };
		}
	};

	template <class T>
	const test_case_data<T> test_data;
}