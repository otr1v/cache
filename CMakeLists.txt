cmake_minimum_required(VERSION 3.8)

project(my_cache)


set(SRC_LIST main.cpp src/input.cpp)
set(TESTS tests/tests.cpp)

set(debug_flags OFF)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    set(debug_flags ON)
endif()

if (debug_flags)
    set(compile_flags -Og -Wpedantic -Wall -Wextra)
else()
    set(compile_flags -O2)
endif()



add_executable(${PROJECT_NAME} ${SRC_LIST})
target_compile_options(${PROJECT_NAME} PUBLIC ${compile_flags})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)


add_executable(my_tests ${TESTS})
target_compile_options(my_tests PUBLIC ${compile_flags})
target_include_directories(my_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_compile_features(my_tests PUBLIC cxx_std_20)
