
# !!! NOTE: compile.sh expects this label to be suffixed with "UnitTests"  !!!
set(UNIT_TESTS wisardlUnitTests)

set(TEST_SOURCES
        WisardTest.cpp)

# Create new *UnitTests target from test sources and link with ${GTEST}_LIB and ${MY_LIB}
add_executable(${UNIT_TESTS} ${TEST_SOURCES})
target_link_libraries(${UNIT_TESTS} ${GTEST_LIB} ${MY_LIB})





