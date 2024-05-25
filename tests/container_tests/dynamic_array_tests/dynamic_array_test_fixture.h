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

#include "../../test_suites/sequential_collection_tests/sequential_collection_test_fixture.h"
#include "../../mocks/mock_allocator.h"
#include "../../test_data/test_types.h"

#include "containers/DynamicArray.h"

namespace collection_tests {

	using namespace collections;

	template <class T>
	struct DynamicArrayTestTypes {
		using element_t = T;
		using collection_t = DynamicArray<T>;
	};

	template <class params>
	class DynamicArrayTest :
		public SequentialCollectionTests<params>,
		public MockAllocatorTest<typename params::element_t>
	{
	protected:
		using element_t = params::element_t;
		using collection_t = params::collection_t;
		using mock_t = DynamicArray<element_t, MockAllocatorWrapper<element_t>>;
	};
}