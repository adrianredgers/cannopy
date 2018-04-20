
include_directories(${CMAKE_SOURCE_DIR}/src/main ${CMAKE_SOURCE_DIR}/src/include)

# Initialize global ${MY_SOURCES}
set(MY_SOURCES Adrian.cpp CACHE INTERNAL "dummy comment")

# Add new source directories here, each one should have a CMakeLists.txt
add_subdirectory(numer)
add_subdirectory(ann)

# Create our C++ lib (not the Python module), which is used by the Python module and the tests
add_library(${MY_LIB} SHARED ${MY_SOURCES})


# Include pybind11 source you (should) have previously cloned into src/main/lib/
set(PYTHON_INCLUDE_DIR /usr/include/python2.7)
set(PYBIND11 lib/pybind11)
include_directories(${PYTHON_INCLUDE_DIR} ${PYBIND11}/include)
# Ghastly hack to prevent this pybind11 build from using python 3.5 :-(
set(PYBIND11_PYTHON_VERSION 2.7 CACHE STRING "")
add_subdirectory(${PYBIND11})

# Create our Python module using pybind11 sauce-ry
pybind11_add_module(${MY_PY_MODULE} PyModule.cpp)
# Link ${MY_LIB} into new module - note the PRIVATE keyword
target_link_libraries(${MY_PY_MODULE} PRIVATE ${MY_LIB})


# Copy our Python module to $(MY_PYTHON_DIST_DIR}, which should be in your Unix env $PYTHONPATH
add_custom_command(TARGET ${MY_PY_MODULE}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MY_PY_MODULE}> ${MY_PYTHON_DIST_DIR})

# Copy our shared lib to $(MY_PYTHON_DIST_DIR}, which should be in your Unix env $LD_LIBRARY_PATH
add_custom_command(TARGET ${MY_LIB}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MY_LIB}>  ${MY_PYTHON_DIST_DIR})

