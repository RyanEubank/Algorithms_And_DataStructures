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

#include "../Algorithms_DataStructures/Source/concepts/collection.h"

namespace collection_tests {
	
	template <class T>
	struct test_case_data;

	template <collections::collection T>
	struct test_case_data<T> {
	private:

		T v1 = { 1 };
		T v2 = { 2 };
		T v3 = { 3 };
		T v4 = { 4 };
		T v5 = { 5 };
		T v6 = { 6 };

	public:
		auto control() {
			return std::array<T, 3>{ v1, v2, v3 };
		}

		auto different_elements() {
			return std::array<T, 3>{ v4, v5, v6 };
		}

		auto different_order() {
			return std::array<T, 3>{ v3, v2, v1 };
		}

		auto different_size() {
			return std::array<T, 2>{ v1, v2 };
		}
	};

	template <std::integral T>
	struct test_case_data<T> {
	private:
		T v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6;

	public:

		auto control() {
			return std::array<T, 3>{ v1, v2, v3 };
		}

		auto different_elements() {
			return std::array<T, 3>{ v4, v5, v6 };
		}

		auto different_order() {
			return std::array<T, 3>{ v3, v2, v1 };
		}

		auto different_size() {
			return std::array<T, 2>{ v1, v2 };
		}
	};

	template <std::floating_point T>
	struct test_case_data<T> {
	private:
		T v1 = 1.f, v2 = 2.f, v3 = 3.f, v4 = 4.f, v5 = 5.f, v6 = 6.f;

	public:

		auto control() {
			return std::array<T, 3>{ v1, v2, v3 };
		}

		auto different_elements() {
			return std::array<T, 3>{ v4, v5, v6 };
		}

		auto different_order() {
			return std::array<T, 3>{ v3, v2, v1 };
		}

		auto different_size() {
			return std::array<T, 2>{ v1, v2 };
		}
	};

	template <>
	struct test_case_data<void*> {
	private:
		int i1 = 1, i2 = 2, i3 = 3, i4 = 4, i5 = 5, i6 = 6;
		void *v1 = &i1, *v2 = &i2, *v3 = &i3, *v4 = &i4, *v5 = &i5, *v6 = &i6;

	public:

		auto control() {
			return std::array<void*, 3>{ v1, v2, v3 };
		}

		auto different_elements() {
			return std::array<void*, 3>{ v4, v5, v6 };
		}

		auto different_order() {
			return std::array<void*, 3>{ v3, v2, v1 };
		}

		auto different_size() {
			return std::array<void*, 2>{ v1, v2 };
		}
	};

	template <>
	struct test_case_data<std::string> {
	private:
		std::string v1 = "a", v2 = "b", v3 = "c", v4 = "d", v5 = "e", v6 = "f";

	public:

		auto control() {
			return std::array<std::string, 3>{ v1, v2, v3 };
		}

		auto different_elements() {
			return std::array<std::string, 3>{ v4, v5, v6 };
		}

		auto different_order() {
			return std::array<std::string, 3>{ v3, v2, v1 };
		}

		auto different_size() {
			return std::array<std::string, 2>{ v1, v2 };
		}
	};
}