#include <cassert>
#include <ratio>

#include "Quantity.hpp"

// ── distance types (ratio-scaled) ───────────────────────────────
using Kilometers  = Difference<double, struct DistTag, std::ratio<1000>>;
using MetersR     = Difference<double, struct DistTag>;
using Centimeters = Difference<double, struct DistTag, std::ratio<1, 100>>;
using Millimeters = Difference<double, struct DistTag, std::ratio<1, 1000>>;

using KmPoint = Quantity<Kilometers>;
using MPoint  = Quantity<MetersR>;
using CmPoint = Quantity<Centimeters>;

// ── time types (ratio-scaled) ───────────────────────────────────
using Hours   = Difference<double, struct TimeTag, std::ratio<3600>>;
using Minutes = Difference<double, struct TimeTag, std::ratio<60>>;
using Secs    = Difference<double, struct TimeTag>;
using Millis  = Difference<double, struct TimeTag, std::ratio<1, 1000>>;

// ── user-defined literals (like std::chrono_literals) ───────────
namespace distance_literals {
    QUANTITY_DIFFERENCE_LITERAL(km, Kilometers)
    QUANTITY_DIFFERENCE_LITERAL(m,  MetersR)
    QUANTITY_DIFFERENCE_LITERAL(cm, Centimeters)
    QUANTITY_DIFFERENCE_LITERAL(mm, Millimeters)

    QUANTITY_UNIT_LITERAL(km_pt, KmPoint)
    QUANTITY_UNIT_LITERAL(m_pt,  MPoint)
    QUANTITY_UNIT_LITERAL(cm_pt, CmPoint)
}

namespace time_literals {
    QUANTITY_DIFFERENCE_LITERAL(h,   Hours)
    QUANTITY_DIFFERENCE_LITERAL(min, Minutes)
    QUANTITY_DIFFERENCE_LITERAL(s,   Secs)
    QUANTITY_DIFFERENCE_LITERAL(ms,  Millis)
}

int main() {
    // ── type aliases (like std::chrono typedefs) ────────────────
    using Distance = Difference<double, struct MeterTag>;
    using Meters   = Quantity<Distance>;
    // A second, incompatible physical unit
    using Duration = Difference<double, struct SecondTag>;
    using Seconds  = Quantity<Duration>;

    // ── construction ────────────────────────────────────────────
    constexpr Meters   origin{0.0};
    constexpr Meters   pos{10.0};
    constexpr Distance dist{5.0};

    // ── unit - unit => difference ───────────────────────────────
    constexpr auto d1 = pos - origin;
    static_assert(d1.count() == 10.0, "unit - unit must yield a difference");

    // ── unit + difference => unit ───────────────────────────────
    constexpr auto p1 = origin + dist;
    static_assert(p1.count() == 5.0, "unit + difference must yield a unit");

    // ── unit - difference => unit ───────────────────────────────
    constexpr auto p2 = pos - dist;
    static_assert(p2.count() == 5.0, "unit - difference must yield a unit");

    // ── difference + difference => difference ───────────────────
    constexpr auto d2 = dist + Distance{3.0};
    static_assert(d2.count() == 8.0, "diff + diff must yield a diff");

    // ── difference - difference => difference ───────────────────
    constexpr auto d3 = dist - Distance{3.0};
    static_assert(d3.count() == 2.0, "diff - diff must yield a diff");

    // ── difference * scalar => difference ───────────────────────
    constexpr auto d4 = dist * 2.0;
    static_assert(d4.count() == 10.0, "diff * scalar must yield a diff");

    // ── scalar * difference => difference ───────────────────────
    constexpr auto d5 = 3.0 * dist;
    static_assert(d5.count() == 15.0, "scalar * diff must yield a diff");

    // ── difference / scalar => difference ───────────────────────
    constexpr auto d6 = dist / 2.0;
    static_assert(d6.count() == 2.5, "diff / scalar must yield a diff");

    // ── unary minus on difference ───────────────────────────────
    constexpr auto d7 = -dist;
    static_assert(d7.count() == -5.0, "unary minus must negate a diff");

    // ── compound assignment: unit += / -= difference ────────────
    Meters mp{10.0};
    mp += dist;
    assert(mp.count() == 15.0);
    mp -= dist;
    assert(mp.count() == 10.0);

    // ── compound assignment: difference += / -= difference ──────
    Distance dp{10.0};
    dp += dist;
    assert(dp.count() == 15.0);
    dp -= dist;
    assert(dp.count() == 10.0);

    // ── compound assignment: difference *= / /= scalar ──────────
    dp *= 2.0;
    assert(dp.count() == 20.0);
    dp /= 4.0;
    assert(dp.count() == 5.0);

    // ── ordering ────────────────────────────────────────────────
    static_assert(Meters{1.0} < Meters{2.0},   "units must be ordered");
    static_assert(Distance{1.0} < Distance{2.0}, "diffs must be ordered");

    // ── equality ────────────────────────────────────────────────
    static_assert(Meters{1.0} == Meters{1.0},     "equal units");
    static_assert(Distance{1.0} == Distance{1.0}, "equal diffs");

    // ── default-constructed difference is zero ──────────────────
    constexpr Distance zero;
    static_assert(zero.count() == 0.0, "default diff must be zero");

    // ═══════════════════════════════════════════════════════════
    // The lines below must NOT compile.
    // Uncomment any single line to verify it produces a
    // compile-time error.
    // ═══════════════════════════════════════════════════════════

    // --- wrong underlying type (int instead of double) ----------
    // Meters{1};          // int is not double
    // Distance{1};        // int is not double

    // --- unit + unit (meaningless) ------------------------------
    // auto bad1 = Meters{1.0} + Meters{2.0};

    // --- unit * scalar / unit / scalar (meaningless) ------------
    // auto bad2 = Meters{1.0} * 2.0;
    // auto bad3 = Meters{1.0} / 2.0;

    // --- difference * difference (would create a new dimension) -
    // auto bad4 = dist * dist;

    // --- implicit conversion to underlying type -----------------
    // double raw = pos;

    // --- cross-tag operations (meters vs seconds) ---------------
    // auto bad5 = Meters{1.0} - Seconds{2.0};
    // auto bad6 = Meters{1.0} + Duration{1.0};
    // auto bad7 = dist + Duration{1.0};

    // --- comparing different tags --------------------------------
    // bool bad8 = (Meters{1.0} == Seconds{1.0});

    // ═══════════════════════════════════════════════════════════
    // Ratio-based conversions (like std::chrono minutes/seconds)
    // ═══════════════════════════════════════════════════════════

    // ── distance units with different ratios ────────────────────
    using Kilometers  = Difference<double, struct DistTag, std::ratio<1000>>;
    using MetersR     = Difference<double, struct DistTag>;                    // ratio<1>
    using Centimeters = Difference<double, struct DistTag, std::ratio<1, 100>>;
    using Millimeters = Difference<double, struct DistTag, std::ratio<1, 1000>>;

    using KmPoint = Quantity<Kilometers>;
    using MPoint  = Quantity<MetersR>;
    using CmPoint = Quantity<Centimeters>;

    // ── implicit conversion between ratios ──────────────────────
    constexpr Kilometers one_km{1.0};
    constexpr MetersR    as_meters = one_km;           // km → m
    static_assert(as_meters.count() == 1000.0, "1 km = 1000 m");

    constexpr MetersR     five_m{5.0};
    constexpr Centimeters as_cm = five_m;              // m → cm
    static_assert(as_cm.count() == 500.0, "5 m = 500 cm");

    constexpr Millimeters mm{1500.0};
    constexpr MetersR     as_m = mm;                   // mm → m
    static_assert(as_m.count() == 1.5, "1500 mm = 1.5 m");

    constexpr MetersR     thousand_m{1000.0};
    constexpr Kilometers  as_km = thousand_m;          // m → km
    static_assert(as_km.count() == 1.0, "1000 m = 1 km");

    // ── cross-ratio addition (result in common/finest ratio) ────
    constexpr auto sum1 = one_km + five_m;             // km + m => m
    static_assert(sum1.count() == 1005.0, "1km + 5m = 1005m");

    constexpr auto sum2 = five_m + Centimeters{50.0};  // m + cm => cm
    static_assert(sum2.count() == 550.0, "5m + 50cm = 550cm");

    // ── cross-ratio subtraction ─────────────────────────────────
    constexpr auto diff_cr = one_km - five_m;          // km - m => m
    static_assert(diff_cr.count() == 995.0, "1km - 5m = 995m");

    // ── cross-ratio comparison ──────────────────────────────────
    static_assert(Kilometers{1.0} == MetersR{1000.0},  "1km == 1000m");
    static_assert(MetersR{500.0}  <  Kilometers{1.0},  "500m < 1km");
    static_assert(Kilometers{2.0} >  MetersR{1000.0},  "2km > 1000m");

    // ── Quantity cross-ratio conversion ─────────────────────────────
    constexpr KmPoint km_pt{5.0};
    constexpr MPoint  m_pt = km_pt;                    // implicit conversion
    static_assert(m_pt.count() == 5000.0, "5 km point = 5000 m point");

    // ── Quantity cross-ratio subtraction ────────────────────────────
    constexpr auto pt_diff = KmPoint{2.0} - MPoint{500.0};
    static_assert(pt_diff.count() == 1500.0, "2km_pt - 500m_pt = 1500m");

    // ── Quantity cross-ratio addition with difference ───────────────
    constexpr auto shifted = KmPoint{1.0} + MetersR{500.0};
    static_assert(shifted.count() == 1500.0, "1km_pt + 500m = 1500m_pt");

    constexpr auto shifted2 = MPoint{100.0} - Kilometers{0.05};
    static_assert(shifted2.count() == 50.0, "100m_pt - 0.05km = 50m_pt");

    // ── Quantity cross-ratio comparison ─────────────────────────────
    static_assert(KmPoint{1.0} == MPoint{1000.0}, "1km_pt == 1000m_pt");
    static_assert(MPoint{500.0} < KmPoint{1.0},   "500m_pt < 1km_pt");

    // ── time units (like std::chrono) ───────────────────────────
    using Hours   = Difference<double, struct TimeTag, std::ratio<3600>>;
    using Minutes = Difference<double, struct TimeTag, std::ratio<60>>;
    using Secs    = Difference<double, struct TimeTag>;                   // ratio<1>
    using Millis  = Difference<double, struct TimeTag, std::ratio<1, 1000>>;

    constexpr Hours   one_hour{1.0};
    constexpr Minutes as_min = one_hour;
    static_assert(as_min.count() == 60.0, "1 hour = 60 minutes");

    constexpr Secs as_sec = one_hour;
    static_assert(as_sec.count() == 3600.0, "1 hour = 3600 seconds");

    constexpr Millis as_ms = Secs{2.5};
    static_assert(as_ms.count() == 2500.0, "2.5 s = 2500 ms");

    // hours + minutes => common type (minutes)
    constexpr auto total_time = Hours{1.0} + Minutes{30.0};
    static_assert(total_time.count() == 90.0, "1h + 30min = 90 (minutes)");

    // minutes + seconds => common type (seconds)
    constexpr auto total2 = Minutes{90.0} + Secs{30.0};
    static_assert(total2.count() == 5430.0, "90min + 30s = 5430s");

    // ── default-constructed difference is still zero ────────────
    constexpr Kilometers zero_km;
    static_assert(zero_km.count() == 0.0, "default km diff must be zero");

    // ═══════════════════════════════════════════════════════════
    // The lines below must NOT compile (ratio-related).
    // Uncomment any single line to verify a compile error.
    // ═══════════════════════════════════════════════════════════

    // --- cross-tag operations still forbidden --------------------
    // auto bad_cross1 = MetersR{1.0} + Secs{1.0};
    // auto bad_cross2 = KmPoint{1.0} - Quantity<Secs>{1.0};
    // bool bad_cross3 = (Kilometers{1.0} == Secs{1.0});

    // ═══════════════════════════════════════════════════════════
    // User-defined literals (like std::chrono_literals)
    // ═══════════════════════════════════════════════════════════
    {
        using namespace distance_literals;
        using namespace time_literals;

        // ── Difference literals (floating-point) ────────────────
        constexpr auto track = 1.0_km + 500.0_m;
        static_assert(track.count() == 1500.0, "1km + 500m = 1500m");

        constexpr auto tape = 30.0_cm + 5.0_mm;
        static_assert(tape.count() == 305.0, "30cm + 5mm = 305mm");

        constexpr auto sprint = 100.0_m * 2.0;
        static_assert(sprint.count() == 200.0, "100m * 2 = 200m");

        // ── Difference literals (integer) ───────────────────────
        constexpr auto marathon = 42_km + 195_m;
        static_assert(marathon.count() == 42195.0, "42km + 195m = 42195m");

        // ── Time literals ───────────────────────────────────────
        constexpr auto flight = 2.0_h + 30.0_min;
        static_assert(flight.count() == 150.0, "2h + 30min = 150min");

        constexpr auto blink = 1.0_s + 500.0_ms;
        static_assert(blink.count() == 1500.0, "1s + 500ms = 1500ms");

        // ── Quantity literals ───────────────────────────────────────
        constexpr auto start  = 0.0_m_pt;
        constexpr auto finish = 100.0_m_pt;
        constexpr auto gap    = finish - start;
        static_assert(gap.count() == 100.0, "100m_pt - 0m_pt = 100m");

        // ── mixing literals with cross-ratio Quantity operations ────
        constexpr auto shifted = 1.0_km_pt + 500.0_m;
        static_assert(shifted.count() == 1500.0, "1km_pt + 500m = 1500m_pt");

        constexpr auto halfway = 10.0_km_pt - 5.0_km;
        static_assert(halfway.count() == 5.0, "10km_pt - 5km = 5km_pt");
    }

    return 0;
}
