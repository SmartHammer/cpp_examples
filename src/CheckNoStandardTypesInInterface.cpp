#include <cinttypes>

#include "StrongType.hpp"
#include "LimitedNumericStrongType.hpp"
#include "Quantity.hpp"

enum struct Switch : bool
{
    Off,
    On
};

using MyStrongType = StrongType<int, struct Tag1>;
using MyLimitedNumericStrongType = LimitedNumericStrongType<int, struct Tag2, 0, 100>;
using MyDifference = Difference<float, struct Tag3>;
using MyQuantity = Quantity<MyDifference>;

struct IGoodInterface
{
    virtual void f(MyStrongType x) = 0;
    virtual void g(MyLimitedNumericStrongType x) = 0;
    virtual void h(MyQuantity x) = 0;
    virtual Switch i() = 0;
};

struct BadInterface
{
    virtual void f(uint32_t x) = 0; // this should be an error
    virtual void g(int32_t x) = 0; // this should be an error
    virtual void h(int32_t x) = 0; // this should be an error
    virtual bool i() = 0; // this should be an error
};

int main()
{
    return 0;
}