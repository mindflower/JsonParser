cmake_minimum_required(VERSION 3.8)

set(SRC_PATH "${PROJECT_SOURCE_DIR}")
set(GTEST_PATH "${PROJECT_SOURCE_DIR}/deps/gtest/googletest/include")
list(APPEND EXTRA_LIBS json gtest gtest_main)
list(APPEND EXTRA_INCLUDES ${SRC_PATH} ${GTEST_PATH})

add_executable(json_tests json_lexer_tests.cpp json_object_tests.cpp)
target_link_libraries(json_tests ${EXTRA_LIBS})
target_include_directories(json_tests PUBLIC ${EXTRA_INCLUDES})

add_test(NAME json_tests COMMAND json_tests)