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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace collection_tests {

	template <class T>
	class MockAllocatorBase {
	public:
		MOCK_METHOD(T*, allocate, (const size_t));
		MOCK_METHOD(void, deallocate, (T* const, const size_t));
	};


	template <class T>
	class MockAllocator {
	private:
		static inline MockAllocatorBase<T>* _allocator;

	public:

		using value_type = T;
		
		static void init(MockAllocatorBase<T>* allocator) {
			_allocator = allocator;
		}

		T* allocate(const size_t count) {
			_allocator->allocate(count);
			return std::allocator<T>().allocate(count);
		}

		void deallocate(T* const ptr, const size_t count) {
			_allocator->deallocate(ptr, count);
			std::allocator<T>().deallocate(ptr, count);
		}
	};

	template <class T>
	class MockAllocatorTest {
	protected:
		MockAllocatorBase<T> _allocator;

		MockAllocatorTest() {
			MockAllocator<T>::init(&this->_allocator);
		}

		MockAllocatorBase<T>& allocator() {
			return this->_allocator;
		}
	};
}