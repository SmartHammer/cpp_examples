cmake_minimum_required(VERSION 3.28)

project(strong_type
	VERSION 0.0.1
	DESCRIPTION "C++ Examples"
	HOMEPAGE_URL "https://github.com/SmartHammer/cpp_examples"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(strong_type INTERFACE)
add_library(strong_type::strong_type ALIAS strong_type)

target_include_directories(strong_type
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)