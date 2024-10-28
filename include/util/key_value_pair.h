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

#include <compare>
#include <type_traits>
#include <utility>

namespace collections { //TODO fix comments in class and move class to containers

	template <class key_t, class value_t>
	class key_value_pair { 
	public:

		using key_type		= key_t;
		using value_type	= value_t;

		template <class K, class V>
		using pair = key_value_pair<K, V>;

	private:
	
		std::pair<key_t, value_t> _pair; //TODO replace std pair member with just k and v

	public:

		key_value_pair() : _pair() {}

		key_value_pair(const std::pair<key_type, value_type>& p) : _pair(p) {}

		key_value_pair(std::pair<key_type, value_type>&& p) : _pair(std::move(p)) {}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Constructs a pair from the specified arguments.
		/// </summary>
		/// 
		/// <typeparam name="K">
		/// The key type or type keys are constructible from.
		/// </typeparam>
		/// 
		/// <typeparam name="V">
		/// The value type or type values are constructible from.
		/// </typeparam>
		/// 
		/// <param name="k">
		/// The object used to construct the key.
		/// </param>
		/// 
		/// <param name="v">
		/// The object used to construct the mapped value.
		/// </param> -----------------------------------------------------------
		template <class K, class V> requires 
			std::constructible_from<key_type, K> && 
			std::constructible_from<value_type, V>
		key_value_pair(K&& k, V&& v) 
			: _pair(std::forward<key_type>(k), std::forward<value_type>(v)) {}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Copy assigns the key and value type from the provided pair.
		/// </summary>
		/// 
		/// <typeparam name="V">
		/// The key type or a type keys are constructible from.
		/// </typeparam>
		/// 
		/// <typeparam name="K">
		/// The value type or a type values are constructible from.
		/// </typeparam>
		/// 
		/// <param name="other">
		/// The key/value pair to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to this pair with the copied data.
		/// </returns> ---------------------------------------------------------
		template <class K, class V> requires 
			std::assignable_from<key_type, K> && 
			std::assignable_from<value_type, V>
		key_value_pair& operator=(const pair<K, V>& other) 
		{
			_pair = other._pair;
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Move assigns the key and value type from the provided pair.
		/// </summary>
		/// 
		/// <typeparam name="V">
		/// The key type or a type keys are constructible from.
		/// </typeparam>
		/// 
		/// <typeparam name="K">
		/// The value type or a type values are constructible from.
		/// </typeparam>
		/// 
		/// <param name="other">
		/// The key/value pair to copy from.
		/// </param>
		/// 
		/// <returns>
		/// Returns a reference to this pair with the moved data.
		/// </returns> ---------------------------------------------------------
		template <class K, class V> requires 
			std::assignable_from<key_type, K> && 
			std::assignable_from<value_type, V>
		key_value_pair& operator=(pair<K, V>&& other) 
		{
			_pair = std::move(other._pair);
			return *this;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the key.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reference to the pair's key.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const key_type& key() const {
			return _pair.first;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the mapped value.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a reference to the pair's mapped value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] value_type& value() {
			return _pair.second;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the mapped value.
		/// </summary>
		/// 
		/// <returns>
		/// Returns a const reference to the pair's mapped value.
		/// </returns> ---------------------------------------------------------
		[[nodiscard]] const value_type& value() const {
			return _pair.second;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns the comparison of the keys in the specified pairs.
		/// </summary>
		/// 
		/// <typeparam name="K">
		/// The key type or a type keys are comparable to.
		/// </typeparam>
		/// 
		/// <typeparam name="V">
		/// The value type or a type values are comparable to.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The pair on the left of the operator.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The pair on the right of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns the comparison of the keys.
		/// </returns> ---------------------------------------------------------
		template <class K, class V> 
			requires std::three_way_comparable_with<key_type, K>
		[[nodiscard]] friend auto operator<=>(
			const key_value_pair& lhs, 
			const pair<K, V>& rhs
		) {
			return lhs.key() <=> rhs.key();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns if the left key is less than the right key.
		/// </summary>
		/// 
		/// <typeparam name="K">
		/// The key type or a type keys are less than comparable to.
		/// </typeparam>
		/// 
		/// <typeparam name="V">
		/// The value type or a type values are less than comparable to.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The pair on the left of the operator.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The pair on the right of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the key from the left pair is less than the key from
		/// the right pair.
		/// </returns> ---------------------------------------------------------
		template <class K, class V> 
			requires std::totally_ordered_with<key_type, K>
		[[nodiscard]] friend bool operator<(
			const key_value_pair& lhs, 
			const pair<K, V>& rhs
		) {
			return lhs.key() < rhs.key();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns if the left key is greater than the right key.
		/// </summary>
		/// 
		/// <typeparam name="K">
		/// The key type or a type keys are greater than comparable to.
		/// </typeparam>
		/// 
		/// <typeparam name="V">
		/// The value type or a type values are greater than comparable to.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The pair on the left of the operator.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The pair on the right of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the key from the left pair is greater than the key 
		/// from the right pair.
		/// </returns> ---------------------------------------------------------
		template <class K, class V> 
			requires std::totally_ordered_with<key_type, K>
		[[nodiscard]] friend bool operator>(
			const key_value_pair& lhs, 
			const pair<K, V>& rhs
		) {
			return lhs.key() > rhs.key();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Returns if the left key is equal to the right key.
		/// </summary>
		/// 
		/// <typeparam name="K">
		/// The key type or a type keys are equality comparable to.
		/// </typeparam>
		/// 
		/// <typeparam name="V">
		/// The value type or a type values are equality comparable to.
		/// </typeparam>
		/// 
		/// <param name="lhs">
		/// The pair on the left of the operator.
		/// </param>
		/// 
		/// <param name="rhs">
		/// The pair on the right of the operator.
		/// </param>
		/// 
		/// <returns>
		/// Returns true if the key from the left pair is greater than the key 
		/// from the right pair.
		/// </returns> ---------------------------------------------------------
		template <class K, class V>
			requires std::equality_comparable_with<key_type, K>
		[[nodiscard]] friend bool operator==(
			const key_value_pair& lhs, 
			const pair<K, V>& rhs
		) {
			return lhs.key() == rhs.key();
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Write the specified pair to the given output stream.
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The character type of the output stream.
		/// </typeparam>
		/// 
		/// <param name="os">
		/// The stream being written to.
		/// </param>
		/// 
		/// <param name="pair">
		/// The pair being written out.
		/// </param>
		/// 
		/// <returns>
		/// Return the stream object after writing.
		/// </returns> ---------------------------------------------------------
		template <typename char_t> //TODO - optimize/clean up stream operators?
		friend std::basic_ostream<char_t>& operator<<(
			std::basic_ostream<char_t>& os,
			const key_value_pair& pair
		) {
			os << "(key: " << pair.key() << ", value: " << pair.value() << ")";
			return os;
		}

		// ---------------------------------------------------------------------
		/// <summary>
		/// Reads a key/value from the specified input stream into the pair.
		/// </summary>
		/// 
		/// <typeparam name="char_t">
		/// The character type of the input stream.
		/// </typeparam>
		/// 
		/// <param name="is">
		/// The stream being read from.
		/// </param>
		/// 
		/// <param name="pair">
		/// The pair being read into.
		/// </param>
		/// 
		/// <returns>
		/// Return the stream object after reading.
		/// </returns> ---------------------------------------------------------
		template <typename char_t>
		friend std::basic_istream<char_t>& operator>>(
			std::basic_istream<char_t>& is,
			key_value_pair& pair
		) {
			std::string input;
			std::getline(is, input, '\0');

			size_t input_length = input.length();
			size_t first_start = 6;
			size_t first_length = input.find(", value: ") - first_start;
			size_t second_start = first_start + first_length + 9;
			size_t second_length = input_length - second_start - 2;

			std::basic_istringstream<char_t> first(
				input.substr(first_start, first_length));

			std::basic_istringstream<char_t> second(
				input.substr(second_start, second_length));

			if constexpr (std::convertible_to<key_type, std::string>)
				std::getline(first, pair._pair.first, '\0');
			else
				first >> pair._pair.first;

			if constexpr (std::convertible_to<value_type, std::string>)
				std::getline(second, pair._pair.second, '\0');
			else
				second >> pair._pair.second;

			return is;
		}
	};

	template <class T>
	concept pair_type = std::_Is_specialization_v<T, key_value_pair>;

	template <class T>
	struct key_traits {
		using key_type = T;
		using mapped_type = T;
	};

	template <pair_type T>
	struct key_traits<T> {
		using key_type = typename T::key_type;
		using mapped_type = typename T::value_type;
	};
}