add_library(objs OBJECT converter.cpp solver.cpp parser.cpp string_util.cpp pcr.cpp circuit.cpp math_util.cpp)

target_link_libraries(objs flint::flint)
