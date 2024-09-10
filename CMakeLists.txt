cmake_minimum_required(VERSION 3.23)

# cached globals
include(cmake/cached_globals.cmake)

project(
  UGrid
  VERSION ${GLOB_UGRID_PROJECT_VERSION}
  DESCRIPTION "Library for reading and writing UGrid files."
  LANGUAGES CXX C
)

# configuration options
if (NOT WIN32)
  message(NOTICE "Build type: ${CMAKE_BUILD_TYPE}")
endif()

# configuration options
include(cmake/user_config_options.cmake)

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

# Run packaging scripts: requires semantic version
add_subdirectory(package)

# Libraries
add_subdirectory(libs)

# Add unit tests
if(ENABLE_UNIT_TESTING)
  add_subdirectory(tests)
endif()
