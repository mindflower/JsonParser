cmake_minimum_required(VERSION 3.8)

enable_testing()

project(json)
set(BUILD_GMOCK OFF CACHE BOOL "Build only googletest")
set(gtest_force_shared_crt ON CACHE BOOL "Use Multi-threaded DLL Runtime Library")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(deps/gtest )

add_subdirectory(json)
add_subdirectory(json_tests)

add_dependencies(json_tests json gtest gtest_main)