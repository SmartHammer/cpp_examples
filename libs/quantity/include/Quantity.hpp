#pragma once

#include <concepts>
#include <compare>
#include <ratio>
#include <numeric>

// ── forward declarations ────────────────────────────────────────
inline namespace v1 {
template <typename T, typename Tag, typename Ratio = std::ratio<1>>
class Difference;

template <typename Diff>
class Quantity;

// ── helper: common Difference type (finest shared granularity) ──

namespace quantity_detail {

template <typename D1, typename D2>
struct common_diff;

template <typename T, typename Tag, typename R1, typename R2>
struct common_diff<Difference<T, Tag, R1>, Difference<T, Tag, R2>> {
    using type = Difference<T, Tag,
        std::ratio<
            std::gcd(R1::num, R2::num),
            std::lcm(R1::den, R2::den)
        >>;
};

template <typename D1, typename D2>
using common_diff_t = typename common_diff<D1, D2>::type;

/// Two Difference instantiations are compatible when they share T and Tag.
template <typename D1, typename D2>
concept compatible_diff =
    std::is_same_v<typename D1::value_type, typename D2::value_type> &&
    std::is_same_v<typename D1::tag_type,   typename D2::tag_type>;

} // namespace quantity_detail

// ═══════════════════════════════════════════════════════════════
// Difference – like std::chrono::duration
// ═══════════════════════════════════════════════════════════════

/// @brief A difference between two Quantity values (like std::chrono::duration).
///
/// Template parameters:
///   T     – underlying arithmetic type (e.g. double)
///   Tag   – phantom type that prevents mixing unrelated dimensions
///   Ratio – std::ratio that defines the unit's scale relative to the
///           base unit (default: std::ratio<1>).
///           Example: std::ratio<1000> for kilometres when metres is base.
///
/// Allowed operations:
///   difference +/- difference          => common difference
///   difference * scalar                => difference
///   scalar     * difference            => difference
///   difference / scalar                => difference
///   unary minus                        => difference
///   comparison (==, <=>)               between compatible differences
///   implicit conversion               between ratios (same tag)
///
/// Everything else is a compile error.
template <typename T, typename Tag, typename Ratio>
class Difference {
    static_assert(Ratio::num > 0, "Ratio must be positive");
    T value_;

public:
    using value_type = T;
    using tag_type   = Tag;
    using ratio_type = Ratio;

    // Only accept exact type T — no implicit conversions
    template <std::same_as<T> U>
    constexpr explicit Difference(U value) : value_(value) {}

    /// Zero-initialising default constructor
    constexpr Difference() : value_{} {}

    /// Converting constructor from a Difference with a different Ratio
    /// but the same value_type and tag_type (like chrono::duration casts).
    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr Difference(const Difference<T, Tag, R2>& other)
        : value_(other.count()
                 * static_cast<T>(std::ratio_divide<R2, Ratio>::num)
                 / static_cast<T>(std::ratio_divide<R2, Ratio>::den)) {}

    /// Named accessor
    constexpr T count() const { return value_; }

    // ── difference / difference arithmetic (same ratio) ─────────

    /// difference + difference => difference
    constexpr Difference operator+(const Difference& rhs) const {
        return Difference(static_cast<T>(value_ + rhs.value_));
    }

    /// difference - difference => difference
    constexpr Difference operator-(const Difference& rhs) const {
        return Difference(static_cast<T>(value_ - rhs.value_));
    }

    /// difference += difference
    constexpr Difference& operator+=(const Difference& rhs) {
        value_ += rhs.value_;
        return *this;
    }

    /// difference -= difference
    constexpr Difference& operator-=(const Difference& rhs) {
        value_ -= rhs.value_;
        return *this;
    }

    // ── cross-ratio arithmetic ──────────────────────────────────

    /// difference + difference (different ratio) => common difference
    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr auto operator+(const Difference<T, Tag, R2>& rhs) const
        -> quantity_detail::common_diff_t<Difference, Difference<T, Tag, R2>>
    {
        using Common = quantity_detail::common_diff_t<Difference, Difference<T, Tag, R2>>;
        return Common(Common(*this).count() + Common(rhs).count());
    }

    /// difference - difference (different ratio) => common difference
    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr auto operator-(const Difference<T, Tag, R2>& rhs) const
        -> quantity_detail::common_diff_t<Difference, Difference<T, Tag, R2>>
    {
        using Common = quantity_detail::common_diff_t<Difference, Difference<T, Tag, R2>>;
        return Common(Common(*this).count() - Common(rhs).count());
    }

    // ── scalar multiplication / division ────────────────────────

    /// difference * scalar => difference
    template <std::same_as<T> U>
    constexpr Difference operator*(U scalar) const {
        return Difference(static_cast<T>(value_ * scalar));
    }

    /// difference / scalar => difference
    template <std::same_as<T> U>
    constexpr Difference operator/(U scalar) const {
        return Difference(static_cast<T>(value_ / scalar));
    }

    /// difference *= scalar
    template <std::same_as<T> U>
    constexpr Difference& operator*=(U scalar) {
        value_ *= scalar;
        return *this;
    }

    /// difference /= scalar
    template <std::same_as<T> U>
    constexpr Difference& operator/=(U scalar) {
        value_ /= scalar;
        return *this;
    }

    /// unary minus
    constexpr Difference operator-() const {
        return Difference(static_cast<T>(-value_));
    }

    // ── comparison (same ratio) ─────────────────────────────────

    constexpr bool operator==(const Difference&) const = default;
    constexpr auto operator<=>(const Difference&) const = default;

    // ── comparison (cross-ratio) ────────────────────────────────

    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr bool operator==(const Difference<T, Tag, R2>& rhs) const {
        using Common = quantity_detail::common_diff_t<Difference, Difference<T, Tag, R2>>;
        return Common(*this) == Common(rhs);
    }

    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr auto operator<=>(const Difference<T, Tag, R2>& rhs) const {
        using Common = quantity_detail::common_diff_t<Difference, Difference<T, Tag, R2>>;
        return Common(*this) <=> Common(rhs);
    }

    // ── explicitly forbidden ────────────────────────────────────

    /// Prevent implicit conversion to the underlying type (use .count() instead)
    operator T() const = delete;
};

// ── free function: scalar * difference => difference ────────────

template <typename T, typename Tag, typename Ratio>
constexpr Difference<T, Tag, Ratio>
operator*(T scalar, const Difference<T, Tag, Ratio>& diff) {
    return diff * scalar;
}

// ═══════════════════════════════════════════════════════════════
// Quantity – like std::chrono::time_point
// ═══════════════════════════════════════════════════════════════

/// @brief An absolute physical quantity (like std::chrono::time_point).
///
/// Parameterised on its difference type (a Difference instantiation),
/// which fully determines the value type, tag, and ratio.
///
/// Allowed operations:
///   unit - unit              => common difference
///   unit +/- difference      => unit (in common ratio)
///   comparison (==, <=>)     between compatible units
///   implicit conversion      between units with different ratios (same tag)
///
/// Everything else is a compile error.
template <typename Diff>
class Quantity {
    using T     = typename Diff::value_type;
    using Tag   = typename Diff::tag_type;
    using Ratio = typename Diff::ratio_type;

    T value_;

public:
    using value_type      = T;
    using tag_type        = Tag;
    using ratio_type      = Ratio;
    using difference_type = Diff;

    // Only accept exact type T — no implicit conversions
    template <std::same_as<T> U>
    constexpr explicit Quantity(U value) : value_(value) {}

    /// Converting constructor from a Quantity with a different ratio
    /// but the same value_type and tag_type.
    template <typename D2>
        requires (quantity_detail::compatible_diff<Diff, D2>
                  && !std::is_same_v<Diff, D2>)
    constexpr Quantity(const Quantity<D2>& other)
        : value_(other.count()
                 * static_cast<T>(std::ratio_divide<typename D2::ratio_type, Ratio>::num)
                 / static_cast<T>(std::ratio_divide<typename D2::ratio_type, Ratio>::den)) {}

    /// Named accessor (like std::chrono::duration::count)
    constexpr T count() const { return value_; }

    // ── arithmetic (same ratio) ─────────────────────────────────

    /// unit - unit => difference
    constexpr Diff operator-(const Quantity& rhs) const {
        return Diff(static_cast<T>(value_ - rhs.value_));
    }

    /// unit + difference => unit
    constexpr Quantity operator+(const Diff& diff) const {
        return Quantity(static_cast<T>(value_ + diff.count()));
    }

    /// unit - difference => unit
    constexpr Quantity operator-(const Diff& diff) const {
        return Quantity(static_cast<T>(value_ - diff.count()));
    }

    /// unit += difference
    constexpr Quantity& operator+=(const Diff& diff) {
        value_ += diff.count();
        return *this;
    }

    /// unit -= difference
    constexpr Quantity& operator-=(const Diff& diff) {
        value_ -= diff.count();
        return *this;
    }

    // ── cross-ratio arithmetic ──────────────────────────────────

    /// unit - unit (different ratio) => common difference
    template <typename D2>
        requires (quantity_detail::compatible_diff<Diff, D2>
                  && !std::is_same_v<Diff, D2>)
    constexpr auto operator-(const Quantity<D2>& rhs) const
        -> quantity_detail::common_diff_t<Diff, D2>
    {
        using Common = quantity_detail::common_diff_t<Diff, D2>;
        using CUnit  = Quantity<Common>;
        return CUnit(*this) - CUnit(rhs);
    }

    /// unit + difference (different ratio) => unit in common ratio
    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr auto operator+(const Difference<T, Tag, R2>& diff) const
        -> Quantity<quantity_detail::common_diff_t<Diff, Difference<T, Tag, R2>>>
    {
        using Common = quantity_detail::common_diff_t<Diff, Difference<T, Tag, R2>>;
        using CUnit  = Quantity<Common>;
        return CUnit(*this) + Common(diff);
    }

    /// unit - difference (different ratio) => unit in common ratio
    template <typename R2>
        requires (!std::is_same_v<Ratio, R2>)
    constexpr auto operator-(const Difference<T, Tag, R2>& diff) const
        -> Quantity<quantity_detail::common_diff_t<Diff, Difference<T, Tag, R2>>>
    {
        using Common = quantity_detail::common_diff_t<Diff, Difference<T, Tag, R2>>;
        using CUnit  = Quantity<Common>;
        return CUnit(*this) - Common(diff);
    }

    // ── comparison (same ratio) ─────────────────────────────────

    constexpr bool operator==(const Quantity&) const = default;
    constexpr auto operator<=>(const Quantity&) const = default;

    // ── comparison (cross-ratio) ────────────────────────────────

    template <typename D2>
        requires (quantity_detail::compatible_diff<Diff, D2>
                  && !std::is_same_v<Diff, D2>)
    constexpr bool operator==(const Quantity<D2>& rhs) const {
        using Common = quantity_detail::common_diff_t<Diff, D2>;
        using CUnit  = Quantity<Common>;
        return CUnit(*this) == CUnit(rhs);
    }

    template <typename D2>
        requires (quantity_detail::compatible_diff<Diff, D2>
                  && !std::is_same_v<Diff, D2>)
    constexpr auto operator<=>(const Quantity<D2>& rhs) const {
        using Common = quantity_detail::common_diff_t<Diff, D2>;
        using CUnit  = Quantity<Common>;
        return CUnit(*this) <=> CUnit(rhs);
    }

    // ── explicitly forbidden ────────────────────────────────────

    /// Prevent implicit conversion to the underlying type (use .count() instead)
    operator T() const = delete;
};

} // namespace v1

// ═══════════════════════════════════════════════════════════════
// User-defined literal macros (like std::chrono_literals)
// ═══════════════════════════════════════════════════════════════

/// Define a user-defined literal for a Difference type.
/// The literal suffix will be _<suffix>.
///
/// Usage:
///   using Meters = Difference<double, struct DistTag>;
///   namespace distance_literals {
///       QUANTITY_DIFFERENCE_LITERAL(m, Meters)   // enables  5.0_m, 42_m
///   }
///   using namespace distance_literals;
///   auto d = 5.0_m + 42_m;   // Meters{47.0}
#define QUANTITY_DIFFERENCE_LITERAL(suffix, DiffType)                          \
    constexpr DiffType operator""_##suffix(long double val) {                 \
        return DiffType(static_cast<typename DiffType::value_type>(val));      \
    }                                                                          \
    constexpr DiffType operator""_##suffix(unsigned long long val) {           \
        return DiffType(static_cast<typename DiffType::value_type>(val));      \
    }

/// Define a user-defined literal for a Quantity (absolute quantity) type.
/// The literal suffix will be _<suffix>.
///
/// Usage:
///   using MPoint = Quantity<Meters>;
///   namespace distance_literals {
///       QUANTITY_UNIT_LITERAL(m_pt, MPoint)   // enables  5.0_m_pt
///   }
///   using namespace distance_literals;
///   auto p = 100.0_m_pt;   // MPoint{100.0}
#define QUANTITY_UNIT_LITERAL(suffix, UnitType)                                \
    constexpr UnitType operator""_##suffix(long double val) {                 \
        return UnitType(static_cast<typename UnitType::value_type>(val));      \
    }                                                                          \
    constexpr UnitType operator""_##suffix(unsigned long long val) {           \
        return UnitType(static_cast<typename UnitType::value_type>(val));      \
    }
