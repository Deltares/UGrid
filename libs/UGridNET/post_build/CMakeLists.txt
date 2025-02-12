project(
  UGridNETPostBuildCommands
  DESCRIPTION "Copies runtime dependencies to binary directories"
  LANGUAGES NONE
)

set(TARGET_NAME ${PROJECT_NAME})

# It would be cool to use TARGET_RUNTIME_DLLS which is described here
# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
# Unfortunately ZLIB, which is an HDF5 dependency, does not provide an IMPORTED_LOCATION (dll path),
# so it does not make it to the list of run time dependencies. 
# Besides, UGrid is a static library and TARGET_RUNTIME_DLLS works with executables, modules and shared libraries.
# The runtime dependencies are set manually here.
set(
  THIRD_PARTY_RUNTIME_DEPS
  $<TARGET_FILE:netCDF::netcdf>
  $<TARGET_FILE:netCDF::netcdf-cxx4>
  $<TARGET_FILE:hdf5::hdf5>
  $<TARGET_FILE:hdf5::hdf5_hl>
  $<TARGET_FILE:ZLIB::ZLIB> # not a element of TARGET_RUNTIME_DLLS
  $<TARGET_FILE:CURL::libcurl>
)

set(
  ALL_RUNTIME_DEPS
  ${THIRD_PARTY_RUNTIME_DEPS}
  $<TARGET_FILE:UGridCSharpWrapper>
)

# create empty list of the commands which must to be called in the custom target
set(TARGET_COMMANDS)

# UGrid needs the third party dependencies
list(
  APPEND
  TARGET_COMMANDS
  COMMAND ${CMAKE_COMMAND} -E echo "Copying ${THIRD_PARTY_RUNTIME_DEPS} to $<TARGET_NAME:UGrid>"
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:UGrid> ${THIRD_PARTY_RUNTIME_DEPS}
)

# UGridAPITests needs the third party dependencies
list(
  APPEND
  TARGET_COMMANDS
  COMMAND ${CMAKE_COMMAND} -E echo "Copying ${THIRD_PARTY_RUNTIME_DEPS} to $<TARGET_NAME:UGridAPITests>"
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:UGridAPITests> ${THIRD_PARTY_RUNTIME_DEPS}
)

# For each supported .NET target framework, UGridNET needs the third party dependencies and the SWIG-generated dependencies
foreach(FRAMEWORK ${GLOB_LIB_DOTNET_TARGET_FRAMEWORKS})
  list(
    APPEND
    TARGET_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E echo "Copying ${ALL_RUNTIME_DEPS} to $<TARGET_FILE_DIR:UGridNET>/${FRAMEWORK}"
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:UGridNET>/${FRAMEWORK} ${ALL_RUNTIME_DEPS}
  )
endforeach()

# For each test .NET target framework, UGridNET.Test needs the third party dependencies and the SWIG-generated dependencies
foreach(FRAMEWORK ${GLOB_TEST_DOTNET_TARGET_FRAMEWORKS})
  list(
    APPEND
    TARGET_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E echo "Copying ${ALL_RUNTIME_DEPS} to $<TARGET_FILE_DIR:UGridNET.Tests>/${FRAMEWORK}"
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:UGridNET.Tests>/${FRAMEWORK} ${ALL_RUNTIME_DEPS}
  )
endforeach()

add_custom_target(
  ${TARGET_NAME} ALL
  DEPENDS
    $<TARGET_NAME:UGrid>
    $<TARGET_NAME:UGridAPITests>
    $<TARGET_NAME:UGridCSharpWrapper>
    $<TARGET_NAME:UGridNET>
    $<TARGET_NAME:UGridNET.Tests>
  ${TARGET_COMMANDS}
  COMMENT "Copying runtime dependencies..."
)

set_target_properties(
  ${TARGET_NAME}
  PROPERTIES
    RUNTIME_DEPS "${ALL_RUNTIME_DEPS}"
)
