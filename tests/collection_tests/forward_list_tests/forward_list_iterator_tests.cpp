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

#include <string>
#include <gtest/gtest.h>

#include "containers/ForwardList.h"

#include "../../collection_test_suites/iterator_tests/input_iterator_tests.h"
#include "../../collection_test_suites/iterator_tests/forward_iterator_tests.h"

namespace collection_tests {

	template <class T>
	struct stable_iterator_wrapper : public ForwardList<T> {

		using base = ForwardList<T>;
		using base::base;

		auto begin() {
			return this->stable_begin();
		}

		auto begin() const {
			return this->stable_begin();
		}

		auto end() {
			return this->stable_end();
		}

		auto end() const {
			return this->stable_end();
		}
	};

	using test_params = testing::Types<
		ForwardList<std::string>,
		stable_iterator_wrapper<std::string>
	>;

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ForwardListTest,
		InputIteratorTests,
		test_params
	);

	INSTANTIATE_TYPED_TEST_SUITE_P(
		ForwardListTest,
		ForwardIteratorTests,
		test_params
	);
}