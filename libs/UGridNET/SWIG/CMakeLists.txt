project(
  UGridCSWrapper
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "C# wrapper around UGridAPI"
  LANGUAGES CXX C
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

#install swig
include(${CMAKE_SOURCE_DIR}/cmake/install_swig.cmake)
set(SWIG_VERSION 4.2.1)
set(SWIG_INSTALL_PREFIX ${TOOLS_INSTALL_DIR}/swig)
install_swig(
  SWIG_VERSION ${SWIG_VERSION}
  SWIG_INSTALL_PREFIX ${SWIG_INSTALL_PREFIX}
  SWIG_CLEAN FALSE
  SWIG_DIR_OUT SWIG_DIR
  SWIG_EXECUTABLE_OUT SWIG_EXECUTABLE
)
# Find SWIG
find_package(SWIG REQUIRED)
if (SWIG_FOUND)
  message(STATUS "Found SWIG ${SWIG_VERSION}")
  else()
  message(FATAL_ERROR "Could not find SWIG")
endif()
include(${SWIG_USE_FILE})

# relevant directories
set(UGridAPI_ROOT_DIR ${CMAKE_SOURCE_DIR}/libs/UGridAPI)
set(UGridAPI_SRC_DIR ${UGridAPI_ROOT_DIR}/src)
set(UGridAPI_INC_DIR ${UGridAPI_ROOT_DIR}/include)
set(UGridAPI_DOMAIN_INC_DIR ${UGridAPI_INC_DIR}/UGridAPI)
set(VERSION_INC_DIR ${CMAKE_SOURCE_DIR}/package)


set(SWIG_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/interface/UGrid.i)
set(OUTPUT_DIR_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(OUTPUT_DIR ${OUTPUT_DIR_PREFIX}/csharp)
set(OUTFILE_DIR ${OUTPUT_DIR_PREFIX}/cxx)

file(MAKE_DIRECTORY ${OUTPUT_DIR})
file(MAKE_DIRECTORY ${OUTFILE_DIR})
execute_process(
  COMMAND ${SWIG_EXECUTABLE}
    -v        # verbose
    -csharp   # target language
    -c++      # enable c++ processing
    -I${UGridAPI_INC_DIR} #${UGridAPI_DOMAIN_INC_DIR}
    -outdir ${OUTPUT_DIR} # language specific files output directory
    -o ${OUTFILE_DIR}/UGridCSHARP_wrap.cxx
    ${SWIG_INTERFACE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RESULT_VARIABLE SWIG_RESULT
  OUTPUT_VARIABLE SWIG_OUTPUT
  ERROR_VARIABLE SWIG_ERROR
)

# Get list of generated sources
file(GLOB SWIG_GENERATED_CSHARP_SRCS CONFIGURE_DEPENDS "${OUTPUT_DIR}/*.cs")
set(SWIG_GENERATED_CSHARP_SRCS ${SWIG_GENERATED_CSHARP_SRCS} PARENT_SCOPE)

if(NOT ${SWIG_RESULT} EQUAL 0)
    message(FATAL_ERROR "SWIG failed with the following error:\n${SWIG_ERROR}")
endif()

add_library(${TARGET_NAME} SHARED ${CMAKE_BINARY_DIR}/version.rc)

set(
  SRC_LIST
  ${UGridAPI_SRC_DIR}/UGrid.cpp
  ${OUTFILE_DIR}/UGridCSHARP_wrap.cxx
)

set(
  INC_LIST
  ${UGridAPI_DOMAIN_INC_DIR}/Contacts.hpp
  ${UGridAPI_DOMAIN_INC_DIR}/Mesh1D.hpp
  ${UGridAPI_DOMAIN_INC_DIR}/Mesh2D.hpp
  ${UGridAPI_DOMAIN_INC_DIR}/Network1D.hpp
  ${UGridAPI_DOMAIN_INC_DIR}/UGrid.hpp
  ${UGridAPI_DOMAIN_INC_DIR}/UGridState.hpp
  ${VERSION_INC_DIR}/Version/Version.hpp
)

target_sources(
  ${TARGET_NAME}
  PRIVATE
    ${SRC_LIST}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${UGridAPI_INC_DIR} ${VERSION_INC_DIR}
      FILES
        ${INC_LIST}
)

target_link_libraries(
  ${TARGET_NAME} 
  PUBLIC
    UGrid
  PRIVATE
    netCDF::netcdf
    netCDF::netcdf-cxx4
)

#target_include_directories(${TARGET_NAME} PUBLIC ${UGridAPI_DOMAIN_INC_DIR})

# group the sources in IDE tree
source_group("Source Files" FILES ${SRC_LIST})

# group the headers in IDE tree
source_group("Header Files" FILES ${INC_LIST})


# set_property(
#   SOURCE 
#     ${SWIG_INTERFACE}
#   PROPERTY
#     CPLUSPLUS ON
#     SWIG_MODULE_NAME ${TARGET_NAME}
# )

# include_directories(${UGridAPI_INC_DIR})

# swig_add_library(
#   ${TARGET_NAME}
#   TYPE SHARED
#   LANGUAGE csharp
#   SOURCES
#     ${SWIG_INTERFACE}
#     ${UGridAPI_SRC_DIR}/UGrid.cpp
#     ${UGridAPI_INC_DIR}/Contacts.hpp
#     ${UGridAPI_INC_DIR}/Mesh1D.hpp
#     ${UGridAPI_INC_DIR}/Mesh2D.hpp
#     ${UGridAPI_INC_DIR}/Network1D.hpp
#     ${UGridAPI_INC_DIR}/UGrid.hpp
#     ${UGridAPI_INC_DIR}/UGridState.hpp
#   OUTPUT_DIR ${OUTPUT_DIR}
#   OUTFILE_DIR ${OUTFILE_DIR}
# )

# # set_property(
# #   SOURCE 
# #     ${${SWIG_INTERFACE}}
# #   PROPERTY
# #     SWIG_FLAGS "-namespace UGridNET"
# # )

# set_property(TARGET ${TARGET_NAME} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

# target_link_libraries(
#   ${TARGET_NAME} 
#   PUBLIC
#     UGrid
#   PRIVATE
#     netCDF::netcdf
#     netCDF::netcdf-cxx4
# )

# target_include_directories(${TARGET_NAME} PUBLIC ${UGridAPI_DIR}/include)
