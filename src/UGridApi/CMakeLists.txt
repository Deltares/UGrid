# Set api files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/include/UGridApi/*.hpp")

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/UGridApi/*.cpp")

# Make a shared library
add_library(UGridApi SHARED ${SOURCE_LIST} ${HEADER_LIST}
                                 ${CMAKE_BINARY_DIR}/version.rc)

# Expose the interface of the shared lib
set(NETCDF_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf_4_8_0")
message("NETCDF_PATH is ${NETCDF_PATH}")

set(NETCDFCXX_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf-cxx/netcdf-cxx4-4.3.1")
message("NETCDFCXX_PATH is ${NETCDFCXX_PATH}")

target_include_directories(UGridApi PUBLIC "${PROJECT_SOURCE_DIR}/include" "${NETCDF_PATH}/include" "${NETCDFCXX_PATH}/cxx4")

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(
    UGridApi
    PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++"
  )
  set_target_properties(UGridApi PROPERTIES LINK_FLAGS "-ldl -lgcov --coverage")
endif()

# Link shared lib to static lib, but don't expose it
target_link_libraries(UGridApi PRIVATE UGrid netcdf-cxx "${NETCDF_PATH}/lib/netcdf.lib")

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include/UGridApi"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
