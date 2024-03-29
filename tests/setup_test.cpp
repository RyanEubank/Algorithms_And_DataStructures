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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace setup_tests {

	class MockTest {
	public:
		MOCK_METHOD(void, SomeMethod, ());
	};

	TEST(TestProjectSetup, Test_GTestAndGMock_ConfiguredCorrectly) {
		MockTest mock;
		EXPECT_CALL(mock, SomeMethod);
		mock.SomeMethod();
		EXPECT_EQ(1, 1);
		EXPECT_TRUE(true);
	}
}