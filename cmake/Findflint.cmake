find_path(flint_INCLUDE_DIR flint.h
    PATH_SUFFIXES include/flint
)
find_library(flint_LIBRARY flint
    PATH_SUFFIXES lib64
)

message("${flint_INCLUDE_DIR}, ${flint_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(flint
    REQUIRED_VARS flint_LIBRARY flint_INCLUDE_DIR
)

find_library(GMP_LIBRARY gmp REQUIRED)

if(flint_FOUND)
  set(flint_LIBRARIES ${flint_LIBRARY})
  set(flint_INCLUDE_DIRS ${flint_INCLUDE_DIR})
  if(NOT TARGET flint::flint)
    add_library(flint::flint UNKNOWN IMPORTED)
    set_target_properties(flint::flint PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${flint_INCLUDE_DIR}"
      IMPORTED_LOCATION "${flint_LIBRARY}"
      INTERFACE_LINK_LIBRARIES "${GMP_LIBRARY}")
  endif()
endif()
