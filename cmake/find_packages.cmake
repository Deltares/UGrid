# Boost
find_package(Boost REQUIRED)

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
