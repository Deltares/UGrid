# Configure the nuspec files
project(
  UGridNET.NuGetPack
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "Configures and conditionally generates the UGridNET NuGet package"
  LANGUAGES NONE
)

set(TARGET_NAME ${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/cmake/git_helpers.cmake)
set(VERSION_SUFFIX "")
if(${GIT_FOUND})
  get_current_branch_name(current_branch_name)
  is_main_or_release_branch("${current_branch_name}" on_main_or_release_branch)
  if(NOT ${on_main_or_release_branch})
    set(VERSION_SUFFIX "-dev")
  endif()
endif()

# Add dependencies and files to nuspec ocnfig template
execute_process(
  COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/configure_nuspec.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.nuspec.in
    --destination ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec.preconfig
    --build_type ${CMAKE_BUILD_TYPE} # undefined for multi-config generators... so must configure with -DCMAKE_BUILD_TYPE anyway
    --dotnet_target_frameworks ${GLOB_LIB_DOTNET_TARGET_FRAMEWORKS}
  OUTPUT_VARIABLE PACKAGE_REFERENCES
  RESULT_VARIABLE RETURN_CODE
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL LAST
)

# Then substitute the CMake variables
configure_file(
  ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec.preconfig
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

if(PACK_NUPKG_LOCALLY)
  add_custom_target(
    ${TARGET_NAME} ALL
    DEPENDS
      "$<TARGET_NAME:UGridCSharpWrapper>"
      "$<TARGET_NAME:UGridNET>"
    COMMAND ${NUGET_EXECUTABLE} pack ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec
      -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}/packed
      -Properties Configuration=Release
      -Verbosity detailed
    COMMENT "Executing NuGet pack"
  )
endif()
