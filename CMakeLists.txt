# Works with 3.14 and tested through 3.18
cmake_minimum_required(VERSION 3.16...3.18)

# Set compiler flags
enable_language(C CXX)

option(PRODUCE_CODE_COVERAGE "Produce code coverage files under GNU compilers" OFF)

# Project name and a few useful settings. Other commands can pick up the results
project(
  UGrid
  VERSION 0.0.0
  DESCRIPTION "Library for reading/writing UGrid files."
  LANGUAGES CXX C)

# Make sure all executables and shared libraries end up in one directory,
# otherwise the executables won't run under Windows. This will cause
# the build to fail.
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# Create position independent binaries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Disable compiler specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)
# Set cmake installation prefix, so UGrid library gets installed under /UGrid
SET(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/UGrid")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O2 -std=c++17 -Werror -Wall -Wextra -pedantic -Wno-unused-function")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -std=c++17")
  else()
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} /EHsc /MP /std:c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc /MP /std:c++17")
  endif()

  # Create position independent binaries
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)

  # Disable compiler specific extensions
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Note this needs to be done in the main CMakeLists since it calls
  # enable_testing, which must be in the main CMakeLists.

  include(FetchContent)
  set(FETCHCONTENT_QUIET off)

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
  )
  if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()

  FetchContent_GetProperties(googletest)
  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()

  include(CTest)
  
  # Docs only available if this is the main app
  add_subdirectory(docs)
  
endif()

# Determine version suffix from environment variable
if(DEFINED ENV{VERSION_SUFFIX})
  set(VERSION_SUFFIX $ENV{VERSION_SUFFIX})
else()
  set(VERSION_SUFFIX .0)
endif()

# Boost
if(WIN32)
  # In windows use static libraries
  set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIR})

# Use NetCDF
find_package(netCDF REQUIRED COMPONENTS C)
if (netCDF_FOUND)
  message(STATUS "Found NetCDF ${netCDF_VERSION}")
else()
  message(FATAL_ERROR "Could not find NetCDF" )
endif()

# Use NetCDF-cxx
find_package(netCDFCxx REQUIRED)
if (netCDFCxx_FOUND)
    message(STATUS "Found NetCDFCxx ${netCDFCxx_VERSION}")
    else()
    message(FATAL_ERROR "Could not find NetCDFCxx" )
endif()

find_package(hdf5 REQUIRED)
if (hdf5_FOUND)
    message(STATUS "Found HDF5 ${hdf5_VERSION}")
# else()
#     message(FATAL_ERROR "Could not find HDF5" )
endif()

# Run packaging scripts
add_subdirectory(package)

# The version file
add_subdirectory(include/Version)

# The static library
add_subdirectory(src/UGrid)

# The dynamic library
add_subdirectory(src/UGridApi)

# Testing only available if this is the main app.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(tests)
endif()