# Configure the nuspec files

string(SUBSTRING ${GLOB_DOTNET_TARGET_FRAMEWORK_VERSION} 1 -1 DOTNET_TARGET_FRAMEWORK_VERSION_WITHOUT_V_PREFIX)

include(${CMAKE_SOURCE_DIR}/cmake/git_helpers.cmake)
set(VERSION_SUFFIX "")
if(${GIT_FOUND})
  get_current_branch_name(current_branch_name)
  is_main_or_release_branch("${current_branch_name}" on_main_or_release_branch)
  if(NOT ${on_main_or_release_branch})
    set(VERSION_SUFFIX "-dev")
  endif()
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.nuspec.in
  ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec
  @ONLY
)

# Copy the targets
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.targets
  ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.targets
  COPYONLY
)

# Copy the README
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  ${CMAKE_CURRENT_BINARY_DIR}/README.md
  COPYONLY
)
