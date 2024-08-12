# Boost
SET(BOOST_MIN_VERSION "1.78.0")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Could not find Boost >= 1.78.0" )
endif

# netCDF
find_package(netCDF REQUIRED COMPONENTS C)
if (netCDF_FOUND)
  message(STATUS "Found NetCDF ${netCDF_VERSION}")
else()
  message(FATAL_ERROR "Could not find NetCDF" )
endif()

# netCDFCxx
find_package(netCDFCxx REQUIRED)
if (netCDFCxx_FOUND)
    message(STATUS "Found NetCDFCxx ${netCDFCxx_VERSION}")
    else()
    message(FATAL_ERROR "Could not find NetCDFCxx")
endif()

# hdf5
find_package(hdf5 REQUIRED)
if (hdf5_FOUND)
    message(STATUS "Found HDF5 ${hdf5_VERSION}")
endif()
