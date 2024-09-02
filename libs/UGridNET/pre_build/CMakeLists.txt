project(
  UGridNETPreBuildCommands
  LANGUAGES NONE
)

set(TARGET_NAME ${PROJECT_NAME})

add_custom_target(
    ${TARGET_NAME} ALL
    #DEPENDS ${TARGET_NAME}
    COMMAND ${NUGET_EXECUTABLE} restore ${CMAKE_CURRENT_BINARY_DIR}/../dll/UGridNET.sln
      -Force
      -NoHttpCache
      -Verbosity detailed
      -Source https://api.nuget.org/v3/index.json
    COMMAND ${NUGET_EXECUTABLE} restore ${CMAKE_CURRENT_BINARY_DIR}/../test/UGridNET.Tests.sln
      -Force
      -NoHttpCache
      -Verbosity detailed
      -Source https://api.nuget.org/v3/index.json
    COMMENT "Restoring NuGet packages"
)

add_dependencies(UGridNET ${TARGET_NAME})
add_dependencies(UGridNET.Tests ${TARGET_NAME})
