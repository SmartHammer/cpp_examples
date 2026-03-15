#include <cstdint>
#include <cassert>

#include "StrongType.hpp"

int main() {
    using MyStrongType = StrongType<uint32_t, struct MyStrongTypeTag>;
    //constexpr MyStrongType a{42}; // this should not compile because it is an int not an uint32_t
    constexpr MyStrongType b{42u}; // this should compile because it is an uint32_t
    //int c = b; // this should not compile because it is an uint32_t not an int
    uint32_t d = b; // this should compile because it is an uint32_t
    constexpr MyStrongType e{b}; // this should compile because it is a MyStrongType

    MyStrongType f{b}; // this should compile because it is a MyStrongType
    f = b; // this should compile because it is a MyStrongType

    assert( b == 42u ); // this should compile because it is an uint32_t
    assert( b == f   ); // this should compile because it is a MyStrongType
    
    //f = 42; // this should not compile because it is an int not an uint32_t
    f = 42u; // this should compile because it is an uint32_t

    static_assert( b == 42u, "" ); // this should compile because it is an uint32_t
    assert( b == f              ); // f is not constexpr, so use runtime assert


    return 0;
}