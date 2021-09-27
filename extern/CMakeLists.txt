cmake_minimum_required(VERSION 3.14...3.18)
include(FetchContent)
# Download netcdf-cxx4
set(NETCDF_CXX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/netcdf-cxx4")
##message("NETCDF_CXX_PATH is ${NETCDF_CXX_PATH}")
 
FetchContent_Declare(
  netcdf-cxx4
  GIT_REPOSITORY https://github.com/Unidata/netcdf-cxx4.git
  SOURCE_DIR ${NETCDF_CXX_PATH}
  GIT_TAG v4.3.1)

FetchContent_GetProperties(netcdf-cxx4)
if(NOT foo_POPULATED)
  FetchContent_Populate(netcdf-cxx4)
endif()

# Set library files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${NETCDF_CXX_PATH}/cxx4/nc*.h")
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${NETCDF_CXX_PATH}/cxx4/nc*.cpp")

# Create the static lib
add_library(netcdf-cxx STATIC ${SOURCE_LIST} ${HEADER_LIST})
set_target_properties(netcdf-cxx PROPERTIES LINKER_LANGUAGE CXX)

# Create the static lib
set(NETCDF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/netcdf/netcdf_4_8_0")
##message("NETCDF_PATH is ${NETCDF_PATH}")

target_include_directories(netcdf-cxx PUBLIC "${NETCDF_PATH}/include" "${NETCDF_CXX_PATH}/cxx4")
target_link_libraries(netcdf-cxx LINK_PUBLIC "${NETCDF_PATH}/lib/netcdf.lib")

# IDEs should put the headers in a nice place
source_group(
	TREE "${NETCDF_CXX_PATH}/cxx4"
	PREFIX "Header Files"
	FILES ${HEADER_LIST})

source_group(
	TREE "${NETCDF_CXX_PATH}/cxx4"
	PREFIX "Source Files"
	FILES ${SOURCE_LIST})
	