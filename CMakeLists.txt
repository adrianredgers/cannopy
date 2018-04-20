cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
include(GNUInstallDirs)

project(cannopy)

# Our Python module name - NOTE: same as used in PyModule.cpp and in Python import statement.
set(MY_PY_MODULE cannopy)
# Our shared library used by our Python module and unit tests.
set(MY_LIB CannopylLib)
# Directory to copy our Python module and shared library so they are available for Python import.
set(MY_PYTHON_DIST_DIR ~)

# Set all target dirs globally
set(TARGET_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})

add_subdirectory(src/main)
add_subdirectory(src/test)

