cmake_minimum_required(VERSION 3.28)

project(quantity
	VERSION 0.0.1
	DESCRIPTION "C++ Examples"
	HOMEPAGE_URL "https://github.com/SmartHammer/cpp_examples"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(quantity INTERFACE)
add_library(quantity::quantity ALIAS quantity)

target_include_directories(quantity
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
