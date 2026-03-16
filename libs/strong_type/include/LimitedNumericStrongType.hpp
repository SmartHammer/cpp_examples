#pragma once

#include <concepts>
#include <limits>
#include <utility>  // std::cmp_less, std::cmp_greater

#include "StrongType.hpp"

inline namespace v1 {

template <typename T, typename Tag,
          auto Min = std::numeric_limits<T>::lowest(),
          auto Max = std::numeric_limits<T>::max()>
class LimitedNumericStrongType : public StrongType<T, Tag> {

    template <typename V, typename B>
    static consteval bool less_than(V val, B bound) {
        if constexpr (std::integral<V> && std::integral<B>) {
            return std::cmp_less(val, bound);
        } else {
            return val < bound;
        }
    }

    template <typename V, typename B>
    static consteval bool greater_than(V val, B bound) {
        if constexpr (std::integral<V> && std::integral<B>) {
            return std::cmp_greater(val, bound);
        } else {
            return val > bound;
        }
    }

public:
    template <typename U>
    consteval LimitedNumericStrongType(U value) : StrongType<T, Tag>(static_cast<T>(value)) {
        if (less_than(value, Min) || greater_than(value, Max)) {
            throw "Value out of range";
        }
    }
};

} // namespace v1
