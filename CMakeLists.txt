cmake_minimum_required(VERSION 3.24)

# TODO: Pack as an AppImage
project(cmake_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # for clangd

add_compile_options(-Wall -ggdb)

# Libraries

find_library(gmp_path NAMES gmp)
add_library(gmp IMPORTED SHARED)
set_target_properties(gmp PROPERTIES
  IMPORTED_LOCATION "${gmp_path}"
)

find_library(flint_path NAMES flint)
add_library(flint IMPORTED SHARED)
set_target_properties(flint PROPERTIES
  IMPORTED_LOCATION "${flint_path}"
)

# Project stucture

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(apps)
