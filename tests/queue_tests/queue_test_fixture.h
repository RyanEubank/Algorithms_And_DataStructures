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

#include "../test_suites/collection_tests.h"
#include "../mocks/mock_allocator.h"
#include "../test_data/test_types.h"

#include "adapters/Queue.h"

namespace collection_tests {

	using testing::_;
	using namespace collections;

	template <class T>
	struct QueueTestTypes {
		using element_t = T;
		using collection_t = Queue<T>;
	};

	template <class params>
	class QueueTest :
		public CollectionTests<params>,
		public MockAllocatorTest<typename params::element_t>
	{
	protected:
		using element_t = params::element_t;
		using collection_t = params::collection_t;
	};
}