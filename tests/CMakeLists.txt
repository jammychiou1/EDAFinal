add_executable(linear_test linear_test.cpp)
add_executable(solver_test solver_test.cpp)

target_include_directories(linear_test PRIVATE ../src)
target_link_libraries(linear_test objs flint gmp)

target_include_directories(solver_test PRIVATE ../src)
target_link_libraries(solver_test objs flint gmp)
