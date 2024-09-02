project(
  UGridCSharpWrapper
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "C# wrapper around UGridAPI"
  LANGUAGES CXX C
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# Find SWIG
find_package(SWIG REQUIRED COMPONENTS csharp)
if (NOT SWIG_FOUND)
  message(FATAL_ERROR "Could not find SWIG")
endif()

# relevant directories
set(UGridAPI_ROOT_DIR ${CMAKE_SOURCE_DIR}/libs/UGridAPI)
set(UGridAPI_SRC_DIR ${UGridAPI_ROOT_DIR}/src)
set(UGridAPI_INC_DIR ${UGridAPI_ROOT_DIR}/include)
set(UGridAPI_DOMAIN_INC_DIR ${UGridAPI_INC_DIR}/UGridAPI)
set(VERSION_INC_DIR ${CMAKE_SOURCE_DIR}/package)


set(SWIG_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/interface/UGrid.i)
set(SWIG_OUTPUT_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(SWIG_OUTPUT_DIR ${SWIG_OUTPUT_SRC_DIR}/csharp)
set(SWIG_OUTFILE_DIR ${SWIG_OUTPUT_SRC_DIR}/cxx)
set(SWIG_CPP_OUTFILE_NAME "${TARGET_NAME}.cxx")

file(MAKE_DIRECTORY ${SWIG_OUTPUT_DIR})
file(MAKE_DIRECTORY ${SWIG_OUTFILE_DIR})

message(STATUS "Generating code...")
execute_process(
  COMMAND ${SWIG_EXECUTABLE}
    -v                                              # verbose
    -module UGrid                                   # module name
    -dllimport ${TARGET_NAME}                       # override DllImport attribute name to target name
    -namespace UGridNET
    -csharp                                         # target language
    -c++                                            # enable c++ processing
    -I${UGridAPI_INC_DIR}                           # look for SWIG files in the specified directory
    -outdir ${SWIG_OUTPUT_DIR}                      # language specific files output directory, C# src in tis case
    -o ${SWIG_OUTFILE_DIR}/${SWIG_CPP_OUTFILE_NAME} # name of C/C++ output file
    -O                                              # enable optimization
    ${SWIG_INTERFACE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RESULT_VARIABLE SWIG_RESULT
  ERROR_VARIABLE SWIG_ERROR
  ECHO_OUTPUT_VARIABLE
  ECHO_ERROR_VARIABLE
  COMMAND_ECHO STDOUT
  COMMAND_ERROR_IS_FATAL LAST
)

# Get list of generated sources
file(GLOB SWIG_GENERATED_CSHARP_SRCS CONFIGURE_DEPENDS "${SWIG_OUTPUT_DIR}/*.cs")
set(SWIG_GENERATED_CSHARP_SRCS ${SWIG_GENERATED_CSHARP_SRCS} PARENT_SCOPE)

add_library(${TARGET_NAME} SHARED ${CMAKE_BINARY_DIR}/version.rc)

set(
  SRC_LIST
  ${UGridAPI_SRC_DIR}/UGrid.cpp
  ${SWIG_OUTFILE_DIR}/${SWIG_CPP_OUTFILE_NAME}
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

# group the sources in IDE tree
source_group("Source Files" FILES ${SRC_LIST})

# group the headers in IDE tree
source_group("Header Files" FILES ${INC_LIST})

