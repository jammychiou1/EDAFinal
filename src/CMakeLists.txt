add_library(objs OBJECT converter.cpp solver.cpp parser.cpp string_util.cpp pcr.cpp circuit.cpp math_util.cpp)

#add_dependencies(objs flint gmp)
target_link_libraries(objs flint gmp)
