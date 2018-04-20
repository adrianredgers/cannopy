
set(UNIT_TESTS neuronModelUnitTests)

set(TEST_SOURCES
        RAMNeuronTest.cpp)

# Create new runUnitTests target from test sources and link with ${GTEST}_LIB and ${MY_LIB}
add_executable(${UNIT_TESTS} ${TEST_SOURCES})
target_link_libraries(${UNIT_TESTS} ${GTEST_LIB} ${MY_LIB})



