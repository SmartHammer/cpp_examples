cmake_minimum_required(VERSION 3.28)

project(decorator
	VERSION 0.0.1
	DESCRIPTION "C++ Examples"
	HOMEPAGE_URL "https://github.com/SmartHammer/cpp_examples"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(decorator STATIC)
add_library(decorator::decorator ALIAS decorator)

target_include_directories(decorator
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(decorator
  PRIVATE
    src/widget.cpp
)