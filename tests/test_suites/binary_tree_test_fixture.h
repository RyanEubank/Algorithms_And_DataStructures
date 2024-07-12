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
#include "adapters/TreeTraversalAdapters.h"

namespace collection_tests {

	class BinaryTreeStructureTest : public testing::Test {
	protected:

		void testPreOrderSequence(const auto& tree, const auto& expected) {
			ASSERT_EQ(tree.size(), expected.size());

			auto begin = expected.begin();
			for (const auto& e : preOrder(tree))
				EXPECT_EQ(e, *begin++);

			EXPECT_EQ(begin, expected.end());
		}

		void testPostOrderSequence(const auto& tree, const auto& expected) {
			ASSERT_EQ(tree.size(), expected.size());

			auto begin = expected.begin();
			for (const auto& e : postOrder(tree))
				EXPECT_EQ(e, *begin++);

			EXPECT_EQ(begin, expected.end());
		}

		void testInOrderSequence(const auto& tree, const auto& expected) {
			ASSERT_EQ(tree.size(), expected.size());

			auto begin = expected.begin();
			for (const auto& e : inOrder(tree))
				EXPECT_EQ(e, *begin++);

			EXPECT_EQ(begin, expected.end());
		}

		void testLevelOrderSequence(const auto& tree, const auto& expected) {
			ASSERT_EQ(tree.size(), expected.size());

			auto begin = expected.begin();
			for (const auto& e : levelOrder(tree))
				EXPECT_EQ(e, *begin++);

			EXPECT_EQ(begin, expected.end());
		}
	};
}