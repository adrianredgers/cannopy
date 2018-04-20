
set(UNIT_TESTS numerUnitTests)

set(TEST_SOURCES
        FactorialTest.cpp)

# Create new runUnitTests target from test sources and link with ${GTEST}_LIB and ${MY_LIB}
add_executable(${UNIT_TESTS} ${TEST_SOURCES})
target_link_libraries(${UNIT_TESTS} ${GTEST_LIB} ${MY_LIB})





