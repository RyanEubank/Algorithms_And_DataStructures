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

#include <functional>
#include <gtest/gtest.h>

#include "collection_test_fixture.h"
#include "containers/base/BaseHashTable.h"

namespace collection_tests {

	template <class T>
	class HashTableTests : public CollectionTest<T> {
	protected:

		using key_type = T::key_type;
		using size_type = T::size_type;

		struct testable_prehash_func {
			size_type offset = 0;

			size_type operator()(key_type key) {
				return typename T::prehash_type{}(key) + offset;
			}
		};

		struct testable_hash_func {
			size_type offset = 0;

			size_type operator()(size_type prehashed_key, size_type capacity) {
				return (typename T::hash_type{}(prehashed_key, capacity)
					+ offset) % capacity;
			}
		};

		using rehashable_test_type = ChainingHashTable<
			typename T::value_type,
			testable_prehash_func,
			testable_hash_func,
			typename T::equality_type,
			typename T::allocator_type,
			typename T::chain_type,
			table_traits<T>::allow_duplicates
		>;
	};

	TYPED_TEST_SUITE_P(HashTableTests);

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that a default constructed table is constructed with its default
	/// capacity.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(HashTableTests, EmptyTableHasDefaultCapacity) {
		FORWARD_TEST_TYPES();

		const collection_type obj{};

		size_t expected = table_traits<collection_type>::default_capacity;
		size_t actual = obj.capacity();

		EXPECT_EQ(expected, actual);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that a default constructed table is has a load factor of zero at
	/// initialization.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(HashTableTests, EmptyTableHasZeroLoadFactor) {
		FORWARD_TEST_TYPES();

		const collection_type obj{};

		float expected = obj.load_factor();
		float actual = 0.0f;

		EXPECT_EQ(expected, actual);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the table calculates its load factor correctly as the
	/// number of elements / table capacity.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(HashTableTests, TableCalculatesLoadFactorCorrectly) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		auto elements = { a, b, c, d, e, f, g, h, i, j };
		auto begin = elements.begin();
		
		for (int size = 1; size <= elements.size(); ++size) {
			obj.insert(*begin++);
			size_t capacity = obj.capacity();
			float expected = (float) size / (float) capacity;
			EXPECT_EQ(expected, obj.load_factor());
		}
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that setting the tables maximum load factor does not resize the
	/// table if its load factor is still below the new limit.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		SettingMaxLoadFactorDoesNotChangeTableCapacityIfStillBelowNewLimit
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		float current = obj.load_factor();
		float max = current + ((obj.max_load_factor() - current) / 2.0f);

		ASSERT_GT(max, current);

		size_t expected = obj.capacity();
		obj.set_max_load_factor(max);
		EXPECT_EQ(expected, obj.capacity());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that setting the tables maximum load factor does resize the table
	/// if its load factor surpasses the new limit.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		SettingMaxLoadFactorDoesChangeTableCapacityIfAboveNewLimit
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};
		float current = obj.load_factor();
		float max = current - 0.1f;

		ASSERT_LT(max, current);

		size_t oldCapacity = obj.capacity();
		obj.set_max_load_factor(max);
		EXPECT_GT(obj.capacity(), oldCapacity);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the bucket method returns the correct index for a given key.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(HashTableTests, BucketReturnsCorrectIndexForKey) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{a, b, c};

		auto prehash = obj.prehash_func();
		auto hash = obj.hash_func();
		auto capacity = obj.capacity();

		size_type expectedA = hash(prehash(a), capacity);
		size_type expectedB = hash(prehash(b), capacity);
		size_type expectedC = hash(prehash(c), capacity);

		size_type actualA = obj.bucket(a);
		size_type actualB = obj.bucket(b);
		size_type actualC = obj.bucket(c);

		EXPECT_EQ(expectedA, actualA);
		EXPECT_EQ(expectedB, actualB);
		EXPECT_EQ(expectedC, actualC);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the begin() and end() methods returns the correct local 
	/// iterator range for the given bucket index.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		IteratorsWithBucketIndexReturnsLocalIteratorToCorrectBucket
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		auto elements = {a, b, c, d, e, f, g, h, i, j};
		collection_type obj{collections::from_range, elements};

		auto expected = elements.begin();
		auto prehash = obj.prehash_func();
		auto hash = obj.hash_func();
		auto capacity = obj.capacity();

		while (expected != elements.end()) {
			size_type index = hash(prehash(*expected), capacity);
			auto bucket_begin = obj.begin(index);
			auto bucket_end = obj.end(index);
			auto search_result = std::find(bucket_begin, bucket_end, *expected++);
			EXPECT_NE(search_result, bucket_end);
		}
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the table will not change capacity when reserve() is called 
	/// if the adjusted load factor would be greater than the maximum load 
	/// factor.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		ReserveDoesNotChangeTableCapacityIfRequestWouldExceedMaxLoadFactor
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.set_max_load_factor(1.0f);
		size_t oldCapacity = obj.capacity();

		obj.insert(a);
		obj.insert(b);
		obj.insert(c);
		obj.insert(d);
		obj.insert(e);

		obj.reserve(4);
		EXPECT_EQ(oldCapacity, obj.capacity());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the table will change capacity when calling reserve() if the 
	/// adjusted load factor would still be below the maximum.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		ReserveDoesChangeTableCapacityIfRequestIsStillWithinLoadFactorLimit
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.set_max_load_factor(1.0f);

		obj.insert(a);
		obj.insert(b);
		obj.insert(c);

		// larger capacity will still grow, and smaller capacity still shrinks
		// within limit
		obj.reserve(4);
		EXPECT_EQ(4, obj.capacity());
		obj.reserve(3);
		EXPECT_EQ(3, obj.capacity());
	}

	// -------------------------------------------------------------------------
	/// Tests that the table will not change capacity when resize() is called 
	/// if the adjusted load factor would be greater than the maximum load 
	/// factor.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		ResizeDoesNotChangeTableCapacityIfRequestWouldExceedMaxLoadFactor
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.set_max_load_factor(1.0f);
		size_t oldCapacity = obj.capacity();

		obj.insert(a);
		obj.insert(b);
		obj.insert(c);
		obj.insert(d);
		obj.insert(e);

		obj.resize(4);
		EXPECT_EQ(oldCapacity, obj.capacity());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the table will change capacity when calling resize() if the 
	/// adjusted load factor would still be below the maximum.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		ResizeDoesChangeTableCapacityIfRequestIsStillWithinLoadFactorLimit
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};
		obj.set_max_load_factor(1.0f);

		obj.insert(a);
		obj.insert(b);
		obj.insert(c);

		// larger capacity will still grow, and smaller capacity still shrinks
		// within limit
		obj.resize(4);
		EXPECT_EQ(4, obj.capacity());
		obj.resize(3);
		EXPECT_EQ(3, obj.capacity());
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that rehashing the table with a new prehash function places
	/// elements in the correct buckets.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		RehashingWithNewPrehashFuncPlacesElementsInCorrectBuckets
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		using table_type = HashTableTests<collection_type>::rehashable_test_type;

		table_type obj{a, b, c};

		auto original_prehash = obj.prehash_func();
		auto hash_func = obj.hash_func();
		size_type capacity = obj.capacity();

		size_type original_bucket_a = hash_func(original_prehash(a), capacity);
		size_type original_bucket_b = hash_func(original_prehash(b), capacity);
		size_type original_bucket_c = hash_func(original_prehash(c), capacity);

		auto new_prehash = original_prehash;
		new_prehash.offset = 1;  // the new prehash function returns original + 1
		obj.rehash(new_prehash);

		size_type expected_bucket_a = hash_func(new_prehash(a), capacity);
		size_type expected_bucket_b = hash_func(new_prehash(b), capacity);
		size_type expected_bucket_c = hash_func(new_prehash(c), capacity);

		ASSERT_NE(original_bucket_a, expected_bucket_a);
		ASSERT_NE(original_bucket_b, expected_bucket_b);
		ASSERT_NE(original_bucket_c, expected_bucket_c);

		size_type actual_bucket_a = obj.bucket(a);
		size_type actual_bucket_b = obj.bucket(b);
		size_type actual_bucket_c = obj.bucket(c);

		EXPECT_EQ(actual_bucket_a, expected_bucket_a);
		EXPECT_EQ(actual_bucket_b, expected_bucket_b);
		EXPECT_EQ(actual_bucket_c, expected_bucket_c);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that rehashing the table with a new prehash function places
	/// elements in the correct buckets.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		RehashingWithNewHashFuncPlacesElementsInCorrectBuckets
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		using table_type = HashTableTests<collection_type>::rehashable_test_type;

		table_type obj{a, b, c};

		auto prehash_func = obj.prehash_func();
		auto original_hash = obj.hash_func();
		size_type capacity = obj.capacity();

		size_type original_bucket_a = original_hash(prehash_func(a), capacity);
		size_type original_bucket_b = original_hash(prehash_func(b), capacity);
		size_type original_bucket_c = original_hash(prehash_func(c), capacity);

		auto new_hash = original_hash;
		new_hash.offset = 1;  // the new hash function returns original + 1
		obj.rehash(new_hash);

		size_type expected_bucket_a = new_hash(prehash_func(a), capacity);
		size_type expected_bucket_b = new_hash(prehash_func(b), capacity);
		size_type expected_bucket_c = new_hash(prehash_func(c), capacity);

		ASSERT_NE(original_bucket_a, expected_bucket_a);
		ASSERT_NE(original_bucket_b, expected_bucket_b);
		ASSERT_NE(original_bucket_c, expected_bucket_c);

		size_type actual_bucket_a = obj.bucket(a);
		size_type actual_bucket_b = obj.bucket(b);
		size_type actual_bucket_c = obj.bucket(c);

		EXPECT_EQ(actual_bucket_a, expected_bucket_a);
		EXPECT_EQ(actual_bucket_b, expected_bucket_b);
		EXPECT_EQ(actual_bucket_c, expected_bucket_c);
	}

	// -------------------------------------------------------------------------
	/// <summary>
	/// Tests that the hash table will resize and expand its capacity when it
	/// reaches the set maximum load factor.
	/// </summary> -------------------------------------------------------------
	TYPED_TEST_P(
		HashTableTests, 
		TableCapacityExpandsWhenMaxLoadFactorIsReached
	) {
		FORWARD_TEST_TYPES();
		DECLARE_TEST_DATA();

		collection_type obj{};

		size_type original_capacity = 4;
		obj.resize(original_capacity);
		obj.set_max_load_factor(0.5);

		ASSERT_EQ(obj.capacity(), original_capacity);

		obj.insert(a);
		EXPECT_EQ(obj.capacity(), original_capacity);

		obj.insert(b);
		EXPECT_EQ(obj.capacity(), original_capacity);

		obj.insert(c);
		EXPECT_GT(obj.capacity(), original_capacity);
	}

	REGISTER_TYPED_TEST_SUITE_P(
		HashTableTests,
		EmptyTableHasDefaultCapacity,
		EmptyTableHasZeroLoadFactor,
		TableCalculatesLoadFactorCorrectly,
		SettingMaxLoadFactorDoesNotChangeTableCapacityIfStillBelowNewLimit,
		SettingMaxLoadFactorDoesChangeTableCapacityIfAboveNewLimit,
		BucketReturnsCorrectIndexForKey,
		IteratorsWithBucketIndexReturnsLocalIteratorToCorrectBucket,
		ReserveDoesNotChangeTableCapacityIfRequestWouldExceedMaxLoadFactor,
		ReserveDoesChangeTableCapacityIfRequestIsStillWithinLoadFactorLimit,
		ResizeDoesNotChangeTableCapacityIfRequestWouldExceedMaxLoadFactor,
		ResizeDoesChangeTableCapacityIfRequestIsStillWithinLoadFactorLimit,
		RehashingWithNewPrehashFuncPlacesElementsInCorrectBuckets,
		RehashingWithNewHashFuncPlacesElementsInCorrectBuckets,
		TableCapacityExpandsWhenMaxLoadFactorIsReached
	);
}