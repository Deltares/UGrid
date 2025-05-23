# Boost
set(BOOST_MIN_VERSION "1.78.0")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (Boost_FOUND)
  message(STATUS "Found Boost ${Boost_VERSION}")
else()
  message(FATAL_ERROR "Could not find Boost (minimum required version is ${BOOST_MIN_REQ_VERSION})")
endif()

# zlib
if(WIN32)
  # Set the path to the shared library
  if(DEFINED ENV{ZLIB_ROOT})
    message(VERBOSE "ZLIB_ROOT is an env var")
    set(ZLIB_LIBRARY_PATH "$ENV{ZLIB_ROOT}")
  elseif(DEFINED ZLIB_ROOT)
    message(VERBOSE "ZLIB_ROOT is a config option")
    set(ZLIB_LIBRARY_PATH "${ZLIB_ROOT}")
  else()
    message(FATAL_ERROR "ZLIB_ROOT is undefined")
  endif()
  set(ZLIB_LIBRARY "${ZLIB_LIBRARY_PATH}/bin/zlib.dll")
endif()
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
  message(STATUS "Found ZLIB ${ZLIB_VERSION}")
else()
  message(FATAL_ERROR "Could not find ZLIB")
endif()

# hdf5
find_package (HDF5 NAMES hdf5 COMPONENTS C HL shared)
if (HDF5_FOUND)
  message(STATUS "Found HDF5 ${HDF5_VERSION}")
else()
  message(FATAL_ERROR "Could not find HDF5") 
endif()

# netCDF
find_package(netCDF REQUIRED COMPONENTS C)
if (netCDF_FOUND)
  message(STATUS "Found NetCDF ${netCDF_VERSION}")
else()
  message(FATAL_ERROR "Could not find NetCDF")
endif()

# netCDFCxx
find_package(netCDFCxx REQUIRED)
if (netCDFCxx_FOUND)
  message(STATUS "Found NetCDFCxx ${netCDFCxx_VERSION}")
else()
    message(FATAL_ERROR "Could not find NetCDFCxx")
endif()

# curl
find_package(CURL REQUIRED)
if (CURL_FOUND)
  message(STATUS "Found CURL ${CURL_VERSION_STRING}")
else()
    message(FATAL_ERROR "Could not find CURL")
endif()

# python
set(Python_FIND_VIRTUALENV FIRST)
find_package(Python REQUIRED COMPONENTS Interpreter)
if (Python_FOUND)
  message(STATUS "Found Python ${Python_VERSION}")
else()
  message(FATAL_ERROR "Could not find Python")
endif()

# git
find_package(Git)
if (Git_FOUND)
  message(STATUS "Found Git ${GIT_VERSION_STRING}")
endif()

# check the dependencies that must be collected later
function(check_runtime_dependency IMPORTED_TARGET_NAME REQUIRED_IMPORTED_TARGET_TYPE)
    get_target_property(TARGET_TYPE ${IMPORTED_TARGET_NAME} TYPE)
    if(NOT "${TARGET_TYPE}" STREQUAL "${REQUIRED_IMPORTED_TARGET_TYPE}")
      message(FATAL_ERROR "Dependency ${IMPORTED_TARGET_NAME}: ${REQUIRED_IMPORTED_TARGET_TYPE} was requested but ${TARGET_TYPE} was found")
    endif()
    get_target_property(IMPORTED_TARGET_LOCATION ${IMPORTED_TARGET_NAME} LOCATION)
    message(STATUS "Runtime dependency ${IMPORTED_TARGET_NAME} (${REQUIRED_IMPORTED_TARGET_TYPE}): ${IMPORTED_TARGET_LOCATION}")
endfunction()

check_runtime_dependency(netCDF::netcdf SHARED_LIBRARY)
check_runtime_dependency(netCDF::netcdf-cxx4 SHARED_LIBRARY)
check_runtime_dependency(hdf5-shared SHARED_LIBRARY)
check_runtime_dependency(hdf5_hl-shared SHARED_LIBRARY)
check_runtime_dependency(ZLIB::ZLIB UNKNOWN_LIBRARY)
check_runtime_dependency(CURL::libcurl SHARED_LIBRARY)

# cache the third-party runtime dependencies
# It would be cool to use TARGET_RUNTIME_DLLS which is described here
# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
# Unfortunately ZLIB, which is an HDF5 dependency, does not provide an IMPORTED_LOCATION (dll path),
# so it does not make it to the list of run time dependencies. 
# Besides, UGrid is a static library and TARGET_RUNTIME_DLLS works with executables, modules and shared libraries.
# The runtime dependencies are set manually here.
set(
  THIRD_PARTY_RUNTIME_DEPS
  $<TARGET_FILE:netCDF::netcdf>
  $<TARGET_FILE:netCDF::netcdf-cxx4>
  $<TARGET_FILE:hdf5-shared>
  $<TARGET_FILE:hdf5_hl-shared>
  $<TARGET_FILE:ZLIB::ZLIB> # not an element of TARGET_RUNTIME_DLLS, helaas speculaas
  $<TARGET_FILE:CURL::libcurl>
  CACHE STRING "Third-party runtime dependencies" FORCE
)

# cache all runtime dependencies
set(
  ALL_RUNTIME_DEPS
  ${THIRD_PARTY_RUNTIME_DEPS}
  $<TARGET_FILE:UGridCSharpWrapper>
  CACHE STRING "All runtime dependencies" FORCE
)
