#pragma once

#include <type_traits>

template <typename T, typename tag>
class NamedType {
public:
	explicit NamedType(T const& value) : value_(value) {}

	explicit NamedType(T&& value) requires !std::is_reference_v<T> : 
		value_(std::move(value)) {}

	T& get() { return value_; }
	T const& get() const { return value_; }

private:
	T value_;
};