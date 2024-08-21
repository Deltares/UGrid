project(
  UGridNET
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGridNET C# wrapper around UGridAPI"
  LANGUAGES CXX C
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

#install swig
include(${CMAKE_SOURCE_DIR}/cmake/install_swig.cmake)
set(SWIG_VERSION 4.2.1)
set(SWIG_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/swig)
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
endif()
include(${SWIG_USE_FILE})

# Include directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../UGridAPI/src)

set(UGridAPI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../UGridAPI)
set(UGridAPI_INC_DIR ${UGridAPI_DIR}/include/UGridAPI)
set(UGridAPI_SRC_DIR ${UGridAPI_DIR}/src)

set(SWIG_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/interface/UGrid.i)
set(OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_property(
  SOURCE 
    ${SWIG_INTERFACE}
  PROPERTY
    CPLUSPLUS ON
    SWIG_MODULE_NAME ${TARGET_NAME}
)

include_directories(${UGridAPI_INC_DIR})

swig_add_library(
  ${TARGET_NAME}
  TYPE SHARED
  LANGUAGE csharp
  SOURCES
    ${SWIG_INTERFACE}
    ${UGridAPI_SRC_DIR}/UGrid.cpp
    ${UGridAPI_INC_DIR}/Contacts.hpp
    ${UGridAPI_INC_DIR}/Mesh1D.hpp
    ${UGridAPI_INC_DIR}/Mesh2D.hpp
    ${UGridAPI_INC_DIR}/Network1D.hpp
    ${UGridAPI_INC_DIR}/UGrid.hpp
    ${UGridAPI_INC_DIR}/UGridState.hpp
  OUTPUT_DIR ${OUT_DIR}
  OUTFILE_DIR ${OUT_DIR}
)

set_property(
  SOURCE 
    ${${SWIG_INTERFACE}}
  PROPERTY
    SWIG_FLAGS "-namespace UGridNET"
)

set_property(TARGET ${TARGET_NAME} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

target_link_libraries(
    ${TARGET_NAME} 
    PUBLIC
      UGrid
    PRIVATE
      netCDF::netcdf
      netCDF::netcdf-cxx4
)

target_include_directories(${TARGET_NAME} PUBLIC ${UGridAPI_DIR}/include)

# Copy the UGridAPITests shared library to the target directory
set(DESTINATION "${CMAKE_SOURCE_DIR}/libs/UGridNET/test/bin/$<CONFIG>")
add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      "$<TARGET_FILE:${TARGET_NAME}>"
      "${DESTINATION}"
    COMMENT "Copy $<TARGET_FILE:UGridAPITests> to ${DESTINATION}"
)
