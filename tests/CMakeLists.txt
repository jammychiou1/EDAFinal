add_executable(linear_test linear_test.cpp)

target_include_directories(linear_test PRIVATE ../src)
target_link_libraries(linear_test objs)
