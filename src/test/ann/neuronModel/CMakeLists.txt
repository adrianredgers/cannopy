
set(UNIT_TESTS neuronModelUnitTests)

set(TEST_SOURCES
        RAMNeuronTest.cpp)

# Create new runUnitTests target from test sources and link with ${GTEST}_LIB and ${MY_LIB}
add_executable(${UNIT_TESTS} ${TEST_SOURCES})
target_link_libraries(${UNIT_TESTS} ${GTEST_LIB} ${MY_LIB})

# Place the ${UNIT_TESTS} target executable in ${TARGET_DIR}
set_target_properties(${UNIT_TESTS}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})




