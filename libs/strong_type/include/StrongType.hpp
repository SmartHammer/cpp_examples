#pragma once

#include <concepts>

inline namespace v1 {

template <typename T, typename Tag>
class StrongType {
    T value_;

public:
    // Only accept exact type T — no implicit conversions (e.g. int → uint32_t)
    template <std::same_as<T> U>
    constexpr explicit StrongType(U value) : value_(value) {}

    // Implicit conversion to T
    constexpr operator T() const { return value_; }

    // Delete implicit conversion to any type other than T
    template <typename U>
        requires(not std::same_as<U, T>)
    operator U() const = delete;

    // Assignment from exact type T
    constexpr StrongType& operator=(T value) {
        value_ = value;
        return *this;
    }

    // Delete assignment from any type other than T or StrongType itself
    template <typename U>
        requires(not (std::same_as<U, T> or std::same_as<U, StrongType>))
    StrongType& operator=(U) = delete;

    // Equality with another StrongType (memberwise)
    constexpr bool operator==(const StrongType&) const = default;

    // Equality with underlying type T only
    constexpr bool operator==(T other) const { return value_ == other; }

    // Delete equality with any type other than T or StrongType
    template <typename U>
        requires(not (std::same_as<U, T> or std::same_as<U, StrongType>))
    bool operator==(const U&) const = delete;
};

} // namespace v1