enable_testing()
add_executable(she_test 
    she_test.cc
)

# target_include_directories(she_test
#     ${PROJECT_SOURCE_DIR}/src
# )

target_link_libraries(she_test
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(she_test)