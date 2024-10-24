project(
  UGridNETPreBuildCommands
  LANGUAGES NONE
)

set(TARGET_NAME ${PROJECT_NAME})

add_custom_target(
    ${TARGET_NAME} ALL
    COMMAND ${NUGET_EXECUTABLE} restore ${CMAKE_CURRENT_BINARY_DIR}/../dll/UGridNET.sln
      -Force
      -NoHttpCache
      -Verbosity detailed
      -ConfigFile ${CMAKE_CURRENT_BINARY_DIR}/../dll/NuGet.config
    COMMAND ${NUGET_EXECUTABLE} restore ${CMAKE_CURRENT_BINARY_DIR}/../test/UGridNET.Tests.sln
      -Force
      -NoHttpCache
      -Verbosity detailed
      -ConfigFile ${CMAKE_CURRENT_BINARY_DIR}/../test/NuGet.config
    COMMENT "Restoring NuGet packages"
)

# cannot use generator expressions here
add_dependencies(UGridNET ${TARGET_NAME})
add_dependencies(UGridNET.Tests ${TARGET_NAME})
