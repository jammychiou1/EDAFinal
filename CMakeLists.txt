cmake_minimum_required(VERSION 3.24)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
# TODO: Pack as an AppImage
project(edafinal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # for clangd

add_compile_options(-Wall -ggdb)

find_package(flint REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(apps)
