# project name
project(
  UGrid
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid static library"
  LANGUAGES CXX
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# static library
add_library(${TARGET_NAME} STATIC)

# source directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# include directories
set(INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(DOMAIN_INC_DIR ${INC_DIR}/UGrid)

# list of target sources
set(
  SRC_LIST
  ${SRC_DIR}/Contacts.cpp
  ${SRC_DIR}/Mesh1D.cpp
  ${SRC_DIR}/Mesh2D.cpp
  ${SRC_DIR}/Network1D.cpp
  ${SRC_DIR}/UGridEntity.cpp
)

# list of target headers
set(
  INC_LIST
  ${DOMAIN_INC_DIR}/Constants.hpp
  ${DOMAIN_INC_DIR}/Contacts.hpp
  ${DOMAIN_INC_DIR}/Mesh1D.hpp
  ${DOMAIN_INC_DIR}/Mesh2D.hpp
  ${DOMAIN_INC_DIR}/Network1D.hpp
  ${DOMAIN_INC_DIR}/Operations.hpp
  ${DOMAIN_INC_DIR}/UGridEntity.hpp
  ${DOMAIN_INC_DIR}/UGridVarAttributeStringBuilder.hpp
)

# add sources to target
target_sources(
  ${TARGET_NAME}
  PRIVATE
    ${SRC_LIST}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${INC_DIR}
      FILES
        ${INC_LIST}
)

# Add precompiled header 
target_precompile_headers(
  ${TARGET_NAME}
  PRIVATE
    <map> 
    <string> 
    <unordered_map> 
    <vector> 
    <memory>
    <sstream>
    <ncDim.h> 
    <ncVar.h> 
    <ncFile.h>
)

# Expose the interface of the static lib
target_include_directories(${TARGET_NAME} PUBLIC ${INC_DIR} ${PROJECT_SOURCE_DIR}/../UGridAPI/include)

# Boost
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

# link required libraries
target_link_libraries(
  ${TARGET_NAME}
  PUBLIC
    netCDF::netcdf
    netCDF::netcdf-cxx4
)

# Make sure that coverage information is produced when using gcc
if(ENABLE_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(
    ${TARGET_NAME}
    PRIVATE
      --coverage # equivalent to -f profile-arcs -ftest-coverage
      -static-libgcc
      -static-libstdc++
  )
  target_link_libraries(${TARGET_NAME} PRIVATE gcov)
endif()

install(TARGETS ${TARGET_NAME} FILE_SET HEADERS DESTINATION "include")

# group the sources in IDE tree
source_group("Source Files" FILES ${SRC_LIST})
# group the headers in IDE tree
source_group("Header Files" FILES ${INC_LIST})

# Add unit tests
# if(ENABLE_UNIT_TESTING)
#   add_subdirectory(tests)
# endif()

# get_target_property(NetCDF_DLL_PATH netCDF::netcdf LOCATION)
# get_target_property(NetCDFCxx_DLL_PATH netCDF::netcdf-cxx4 LOCATION)
# get_target_property(HDF5_DLL_PATH hdf5::hdf5 LOCATION)
# get_target_property(HDF5_HL_DLL_PATH hdf5::hdf5_hl LOCATION)
# get_target_property(ZLIB_DLL_PATH ZLIB::ZLIB LOCATION)

# add_custom_command(
#   TARGET ${TARGET_NAME}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${NetCDF_DLL_PATH} $<TARGET_FILE_DIR:${TARGET_NAME}>
#   COMMAND ${CMAKE_COMMAND} -E copy ${NetCDFCxx_DLL_PATH} $<TARGET_FILE_DIR:${TARGET_NAME}>
#   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hdf5::hdf5> $<TARGET_FILE_DIR:${TARGET_NAME}>
#   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hdf5::hdf5_hl> $<TARGET_FILE_DIR:${TARGET_NAME}>
#   COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL_PATH} $<TARGET_FILE_DIR:${TARGET_NAME}>
# )


get_target_property(NetCDF_TARGET_TYPE netCDF::netcdf TYPE)
get_target_property(NetCDFCxx_TARGET_TYPE netCDF::netcdf-cxx4 TYPE)
get_target_property(HDF5_TARGET_TYPE hdf5::hdf5 TYPE)
get_target_property(HDF5_HL_TARGET_TYPE hdf5::hdf5_hl TYPE)
get_target_property(ZLIB_TARGET_TYPE ZLIB::ZLIB TYPE)
message(STATUS "netCDF ${NetCDF_TARGET_TYPE}")
message(STATUS "netcdfCxx ${NetCDFCxx_TARGET_TYPE}")
message(STATUS "hdf5 ${HDF5_TARGET_TYPE}")
message(STATUS "hdf5_hl ${HDF5_HL_TARGET_TYPE}")
message(STATUS "zlib ${ZLIB_TARGET_TYPE}")

add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:netCDF::netcdf> $<TARGET_FILE_DIR:${TARGET_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:netCDF::netcdf-cxx4> $<TARGET_FILE_DIR:${TARGET_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hdf5::hdf5> $<TARGET_FILE_DIR:${TARGET_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hdf5::hdf5_hl> $<TARGET_FILE_DIR:${TARGET_NAME}>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ZLIB::ZLIB> $<TARGET_FILE_DIR:${TARGET_NAME}>
)
