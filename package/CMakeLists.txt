# Configure DLL version
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_BINARY_DIR}/version.rc @ONLY
)

# Configure the nuspec files
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