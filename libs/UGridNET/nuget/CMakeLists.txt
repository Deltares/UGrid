# Configure the nuspec files

string(SUBSTRING ${GLOB_DOTNET_TARGET_FRAMEWORK_VERSION} 1 -1 DOTNET_TARGET_FRAMEWORK_VERSION_WITHOUT_V_PREFIX)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.nuspec.in
  ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec
  @ONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.targets
  ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.targets
  COPYONLY
)
