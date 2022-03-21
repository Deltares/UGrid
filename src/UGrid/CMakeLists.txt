# Set library files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/UGrid/*.hpp")
 
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/UGrid/*.cpp")

# Create the static lib
add_library(UGrid STATIC ${SOURCE_LIST} ${HEADER_LIST})

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set_target_properties(UGrid PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++")
   set_target_properties(UGrid PROPERTIES LINK_FLAGS "-ldl -lgcov --coverage")
endif()

# NetCDF CPP wrapper
target_include_directories(UGrid PUBLIC "${PROJECT_SOURCE_DIR}/include" "${netCDF_INCLUDE_DIR}" "${NETCDFCXX_PATH}/cxx4")

# Add target link dependencies
target_link_directories(UGrid PRIVATE ${netCDF_LIB_DIR})
target_link_libraries(UGrid PRIVATE netCDF::netcdf-cxx4 netcdf)

# Add precompiled header 
target_precompile_headers(UGrid PRIVATE <map> 
                                        <string> 
                                        <unordered_map> 
                                        <vector> 
                                        <memory>
                                        <sstream>
                                        <ncDim.h> 
                                        <ncVar.h> 
                                        <ncFile.h>)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include/UGrid"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})