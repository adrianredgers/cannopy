
# For <xxx.h> used by tests
include_directories(${CMAKE_SOURCE_DIR}/src/include ${CMAKE_SOURCE_DIR}/src/test)


# Build the googletest library from source you have previously cloned into src/test/lib/
set(GTEST_ROOT lib/googletest)
add_subdirectory(${GTEST_ROOT})
# For <gtest/gtest.h> used by tests
include_directories(${GTEST_ROOT}/googletest/include)
# Name of the Googletest lib we need
set(GTEST_LIB gtest_main)


#
# We haz a top-level unit-test suite
# NOTE: Call all unit-test suites: <xxx>UnitTests so compile.sh can pick them up
set(UNIT_TESTS topLevelUnitTests)

set(TEST_SOURCES
        AdrianTest.cpp)

# Create new UNIT_TESTS target from test sources and link with ${GTEST}_LIB and ${MY_LIB}
add_executable(${UNIT_TESTS} ${TEST_SOURCES})
target_link_libraries(${UNIT_TESTS} ${GTEST_LIB} ${MY_LIB})


# Subdirs containing moar tests
add_subdirectory(numer)
add_subdirectory(ann)


