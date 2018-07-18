
# !!! NOTE: compile.sh expects this label to be suffixed with "UnitTests"  !!!
set(UNIT_TESTS perfUnitTests)

set(TEST_SOURCES
        VectorVsArrayTest.cpp)

# Create new runUnitTests target from test sources and link with ${GTEST}_LIB and ${MY_LIB}
add_executable(${UNIT_TESTS} ${TEST_SOURCES})
target_link_libraries(${UNIT_TESTS} ${GTEST_LIB} ${MY_LIB})



