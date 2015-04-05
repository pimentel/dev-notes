cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
add_compile_options(-std=c++11)
add_compile_options(-g)
project(hdf5-example)

enable_language(CXX)

add_subdirectory(src)
