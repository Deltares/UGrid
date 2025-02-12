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
  set(ZLIB_LIBRARY "${ZLIB_ROOT}/bin/zlib.dll")
endif()
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
  message(STATUS "Found ZLIB ${ZLIB_VERSION}")
else()
  message(FATAL_ERROR "Could not find ZLIB")
endif()

# hdf5
find_package(HDF5 REQUIRED COMPONENTS C HL)
if (HDF5_FOUND)
  message(STATUS "Found HDF5 ${HDF5_VERSION}")
  #add_library(hdf5::hdf5-shared ALIAS hdf5-shared)
  #add_library(hdf5::hdf5_hl-shared ALIAS hdf5_hl-shared)
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
check_runtime_dependency(hdf5::hdf5-shared SHARED_LIBRARY)
check_runtime_dependency(hdf5::hdf5_hl-shared SHARED_LIBRARY)
check_runtime_dependency(ZLIB::ZLIB UNKNOWN_LIBRARY) #SHARED_LIBRARY)
check_runtime_dependency(CURL::libcurl SHARED_LIBRARY)
