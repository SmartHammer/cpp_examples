# cpp_examples
Just some C++ examples that show enhanced stuff partly AI generated.

__NOTE__: Used concepts and space-ship operator.

## Strong-Type
C++ 20 strong-type template

__NOTE__: Any strong-type has a base-type, a name and a tag.

```cpp
using FirstStrongType = StrongType<uint8_t, struct FirstStrongTypeTag>;
using SecondStrongType = StrongType<int8_t, struct SecondStrongTypeTag>;
```
[see also](./src/StrongTypeTest.cpp)

## Quantity
C++ 20 strong-type pyhsical quantity type templates

__NOTE__: Each type has a difference type and a quantity type (like std::chrono::time_point and std::chrono::duration)
Any Unit has a difference and a quantity type. The Difference has a base-type, a name, a tag and a ratio (defaulted to 1).

```cpp
using DistanceMeter = Difference<double, struct DistanceTag>;
using Meter = Quantity<DistanceMeter>;
```
[see also](./src/QuantityTest.cpp)
