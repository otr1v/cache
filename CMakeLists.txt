cmake_minimum_required(VERSION 3.8)

project(my_cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_LIST main.cpp src/input.cpp)
set(TESTS tests/tests.cpp)
set(LFU LFU.cpp src/input.cpp)
set(PCA PCA.cpp src/input.cpp)


add_executable(my_LFU ${LFU})
add_executable(my_PCA ${PCA})
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(debug_flags OFF)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    set(debug_flags ON)
endif()

if (debug_flags)
    set(compile_flags -Og -Wpedantic -Wall -Wextra)
    target_compile_definitions(my_LFU PRIVATE DEBUG=1)
    target_compile_definitions(my_PCA PRIVATE DEBUG=1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
    set(compile_flags -O2)
endif()


target_compile_options(my_LFU PUBLIC ${compile_flags})
target_include_directories(my_LFU PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)



target_compile_options(my_PCA PUBLIC ${compile_flags})
target_include_directories(my_PCA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)



target_compile_options(${PROJECT_NAME} PUBLIC ${compile_flags})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)


add_executable(my_tests ${TESTS})
target_compile_options(my_tests PUBLIC ${compile_flags})
target_include_directories(my_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
