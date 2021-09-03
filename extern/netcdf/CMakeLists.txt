# Set library files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx/netcdf-cxx4-4.3.1/cxx4/*.h")

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx/netcdf-cxx4-4.3.1/cxx4/*.cpp")

# Create the static lib
add_library(netcdf-cxx STATIC ${SOURCE_LIST} ${HEADER_LIST})

# Create the static lib
set(NETCDF_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf-cxx/netcdf-cxx4-4.3.1")
message("NETCDF_PATH is ${NETCDF_PATH}")

target_link_libraries(netcdf-cxx LINK_PUBLIC "${NETCDF_PATH}/lib/netcdf.lib")

# IDEs should put the headers in a nice place
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx/netcdf-cxx4-4.3.1/cxx4"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})


