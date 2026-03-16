cmake_minimum_required(VERSION 3.28)

project(examples
	VERSION 0.0.1
	DESCRIPTION "C++ Examples"
	HOMEPAGE_URL "https://github.com/SmartHammer/cpp_examples"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/decorator)
add_executable(c++-decorator src/DecoratorTest.cpp)
target_link_libraries(c++-decorator PRIVATE decorator::decorator)

add_subdirectory(libs/strong_type)
add_executable(c++-strong-type-test src/StrongTypeTest.cpp)
target_link_libraries(c++-strong-type-test PRIVATE strong_type::strong_type)

add_executable(c++-limited-numeric-strong-type-test src/LimitedNumericTypeTest.cpp)
target_link_libraries(c++-limited-numeric-strong-type-test PRIVATE strong_type::strong_type)

add_subdirectory(libs/quantity)
add_executable(c++-physical-unit-test src/QuantityTest.cpp)
target_link_libraries(c++-physical-unit-test PRIVATE quantity::quantity)