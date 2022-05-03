# Set library files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/UGrid/*.hpp") 
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/UGrid/*.cpp")

# Create the static lib
set(target_name UGrid)
add_library(${target_name} STATIC ${SOURCE_LIST} ${HEADER_LIST})

# define the public header
set_target_properties(${target_name} PROPERTIES PUBLIC_HEADER "${HEADER_LIST}")

# Make sure that coverage information is produced when using gcc
if(PRODUCE_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++")
   set_target_properties(${target_name} PROPERTIES LINK_FLAGS "-ldl -lgcov --coverage")
endif()

# NetCDF CPP wrapper
target_include_directories(${target_name} PUBLIC "${PROJECT_SOURCE_DIR}/include" ${netCDF_INCLUDE_DIR} "${NETCDFCXX_PATH}/cxx4")

# Add target link dependencies
target_link_directories(${target_name} PRIVATE ${netCDF_LIB_DIR})
target_link_libraries(${target_name} PRIVATE netCDF::netcdf-cxx4 netcdf)

# Add precompiled header 
target_precompile_headers(${target_name} PRIVATE <map> 
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

install(TARGETS ${target_name} PUBLIC_HEADER DESTINATION "include/${target_name}")