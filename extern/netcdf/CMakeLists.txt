# Set library files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx/netcdf-cxx4-4.3.1/cxx4/nc*.h")

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx/netcdf-cxx4-4.3.1/cxx4/nc*.cpp")

# Create the static lib
add_library(netcdf-cxx STATIC ${SOURCE_LIST} ${HEADER_LIST})

# Create the static lib
set(NETCDF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/netcdf_4_8_0")
message("NETCDF_PATH is ${NETCDF_PATH}")

target_include_directories(netcdf-cxx PUBLIC "${NETCDF_PATH}/include")

target_link_libraries(netcdf-cxx LINK_PUBLIC "${NETCDF_PATH}/lib/netcdf.lib")

# IDEs should put the headers in a nice place
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx/netcdf-cxx4-4.3.1/cxx4"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})