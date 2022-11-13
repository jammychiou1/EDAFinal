add_library(objs OBJECT converter.cpp linear.cpp parser.cpp simulator.cpp)

#add_dependencies(objs flint gmp)
target_link_libraries(objs flint gmp)
