# Set library files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/UGrid/*.hpp")
 
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/UGrid/*.cpp")

# Create the static lib
add_library(UGrid STATIC ${SOURCE_LIST} ${HEADER_LIST})

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(
    UGrid
    PROPERTIES COMPILE_FLAGS
               "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++"
  )
  set_target_properties(UGrid PROPERTIES LINK_FLAGS
                                              "-ldl -lgcov --coverage")
endif()

# NetCDF
set(NETCDF_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf_4_8_0")
message("NETCDF_PATH is ${NETCDF_PATH}")

set(NETCDFCXX_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf-cxx4")
message("NETCDFCXX_PATH is ${NETCDFCXX_PATH}")

# NetCDF CPP wrapper
target_include_directories(UGrid PUBLIC "${PROJECT_SOURCE_DIR}/include" "${NETCDFCXX_PATH}/cxx4")

# Add target link dependencies
target_link_libraries(UGrid LINK_PUBLIC netcdf-cxx  "${NETCDF_PATH}/lib/netcdf.lib")

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include/UGrid"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})