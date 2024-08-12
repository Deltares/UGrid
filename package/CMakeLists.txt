# Determine version suffix (current build number) from environment variable
if(DEFINED ENV{VERSION_SUFFIX})
  set(VERSION_SUFFIX $ENV{VERSION_SUFFIX})
else()
  set(VERSION_SUFFIX 0)
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_BINARY_DIR}/version.rc @ONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/nuget/Deltares.UGrid.Development.nuspec.in
  ${CMAKE_BINARY_DIR}/package/nuget/Deltares.UGrid.Development.nuspec @ONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/nuget/Deltares.UGrid.Release.nuspec.in
  ${CMAKE_BINARY_DIR}/package/nuget/Deltares.UGrid.Release.nuspec @ONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/nuget/Deltares.UGrid.targets
  ${CMAKE_BINARY_DIR}/package/nuget/Deltares.UGrid.targets COPYONLY
)

# Configure the version header file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version/Version.hpp.in
               ${CMAKE_CURRENT_SOURCE_DIR}/Version/Version.hpp @ONLY)