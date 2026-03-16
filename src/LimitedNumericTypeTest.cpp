#include <cstdint>
#include "LimitedNumericStrongType.hpp"

int main()
{
    using LimitedType1 = LimitedNumericStrongType<uint32_t, struct LimitType1Tag, -20, 200>;
    using LimitedType2 = LimitedNumericStrongType<double, struct LimitType2Tag, -20.0, 200.0>;
    using LimitedType3 = LimitedNumericStrongType<int32_t, struct LimitType3Tag, 10, 100>;
    using LimitedType4 = LimitedNumericStrongType<int32_t, struct LimitType4Tag, 10, 100>;
    LimitedType1 a{100};
    // LimitedType1 b{-50}; // shall result in compile error
    // LimitedType1 c{250}; // shall result in compile error
    LimitedType2 c{100.0};
    // LimitedType2 d{-50.0}; // shall result in compile error
    // LimitedType2 e{250.0}; // shall result in compile error
    LimitedType3 e{50};
    // LimitedType3 f{5}; // shall result in compile error
    // LimitedType3 g{150}; // shall result in compile error
    LimitedType4 h{50};
    static_assert(not std::is_same_v<LimitedType3, LimitedType4>, "LimitedType3 and LimitedType4 should be the same type");
    return 0;
}