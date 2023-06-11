include(GoogleTest)

add_executable(test-leaf test_leaf.cpp)
target_link_libraries(test-leaf PRIVATE DotRaiser gtest pthread)

add_test(test-parser test-leaf)