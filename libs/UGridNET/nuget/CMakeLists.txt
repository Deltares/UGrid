# Configure the nuspec files
project(
  UGridNET.NuGetPack
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "Configures and conditionally generates the UGridNET NuGet package"
  LANGUAGES NONE
)

set(TARGET_NAME ${PROJECT_NAME})

set(NUSPEC_IN ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.nuspec.in)
set(NUPKG_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGridNET.targets)
set(NUPKG_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

add_custom_target(
  ${TARGET_NAME} ALL
  DEPENDS
    $<TARGET_NAME:UGridNETPostBuildCommands>
    $<$<BOOL:${PACK_NUPKG_LOCALLY}>:$<TARGET_NAME:UGridCSharpWrapper>>
    $<$<BOOL:${PACK_NUPKG_LOCALLY}>:$<TARGET_NAME:UGridNET>>
  SOURCES
    ${NUSPEC_IN}
    ${NUPKG_TARGETS}
    ${NUPKG_README}
)

# generate version string
include(${CMAKE_SOURCE_DIR}/cmake/git_helpers.cmake)
set(VERSION_SUFFIX "")
if(${GIT_FOUND})
  get_current_branch_name(current_branch_name)
  is_main_or_release_branch("${current_branch_name}" on_main_or_release_branch)
  if(NOT ${on_main_or_release_branch})
    set(VERSION_SUFFIX "-dev")
  endif()
endif()
string(JOIN "." NUPKG_VERSION
    ${PROJECT_VERSION_MAJOR}
    ${PROJECT_VERSION_MINOR}
    ${PROJECT_VERSION_PATCH}
    ${PROJECT_VERSION_TWEAK}
)
string(APPEND NUPKG_VERSION ${VERSION_SUFFIX})

# Add dependencies and files to nuspec config template
set(NUSPEC_PRECONFIG ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec.preconfig)
set(NUSPEC_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/Deltares.UGridNET.nuspec)
get_target_property(RUNTIME_DEPS UGridNETPostBuildCommands RUNTIME_DEPS)
add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/configure_nuspec.py
    --template ${NUSPEC_IN}
    --destination ${NUSPEC_PRECONFIG}
    --build_type $<CONFIG>
    --dotnet_target_frameworks ${GLOB_LIB_DOTNET_TARGET_FRAMEWORKS}
    --runtime_dependencies ${RUNTIME_DEPS}
  COMMAND_EXPAND_LISTS
  VERBATIM
  COMMENT "Generating nuspec pre-config file"
)

# Then substitute the CMake variables
set(CONFIGURE_NUSPEC_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/configure_nuspec.cmake)
file(
  GENERATE
  OUTPUT ${CONFIGURE_NUSPEC_SCRIPT}
  CONTENT "configure_file(${NUSPEC_PRECONFIG} ${NUSPEC_CONFIG} @ONLY)"
)
add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND}
    -DVERSION=${NUPKG_VERSION}
    -DCOPYRIGHT_YEAR=${GLOB_CURRENT_YEAR}
    -DBUILD_DIR=${CMAKE_BINARY_DIR}
    -P ${CONFIGURE_NUSPEC_SCRIPT}
  COMMENT "Generating nuspec config file"
)

# Copy the targets
configure_file(${NUPKG_TARGETS} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Copy the README
configure_file(${NUPKG_README} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

if(PACK_NUPKG_LOCALLY)
  set(NUPKG_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/packed)
  set(NUPKG ${NUPKG_OUT_DIR}/Deltares.UGridNET.${NUPKG_VERSION})
  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${NUGET_EXECUTABLE} pack ${NUSPEC_CONFIG}
      -OutputDirectory ${NUPKG_OUT_DIR}
      -Properties Configuration=Release
      -Verbosity detailed
    COMMENT "Execute NuGet pack"
  )
endif()
