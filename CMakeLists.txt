cmake_minimum_required(VERSION 3.23)

set(UGRID_VERSION 0.0.0)

project(
  UGrid
  VERSION ${UGRID_VERSION}
  DESCRIPTION "Library for reading/writing UGrid files."
  LANGUAGES CXX C
)

# get options
include(cmake/options.cmake)

# configure the compiler
include(cmake/compiler_config.cmake)

# fetch dependencies, must appear after options.cmake
include(cmake/fetch_contents.cmake)

# find required packages
include(cmake/find_packages.cmake)

# organize targets into folders, can be remove in version > 3.26 (ON by default)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Docs only available if this is the main app
add_subdirectory(docs)

# Tools
# Note: tools/Version sets the semantic version
#add_subdirectory(tools)

# Run packaging scripts: requires semantic version
add_subdirectory(package)

# Libraries
add_subdirectory(libs)

# Add unit tests
if(ENABLE_UNIT_TESTING)
  add_subdirectory(tests)
endif()
