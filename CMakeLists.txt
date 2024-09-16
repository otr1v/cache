cmake_minimum_required(VERSION 3.8)

project("cache")



add_executable(cach src/cache.cpp src/input.cpp)

target_compile_features(cache PUBLIC cxx_std_20)