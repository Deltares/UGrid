project(
  UGridNETPostBuildCommands
  LANGUAGES NONE
)

set(TARGET_NAME ${PROJECT_NAME})

add_custom_target(
  ${TARGET_NAME} ALL
  DEPENDS
    "$<TARGET_NAME:UGridCSharpWrapper>"
    "$<TARGET_NAME:UGridNET>"
    "$<TARGET_NAME:UGridNET.Tests>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridCSharpWrapper>" "$<TARGET_FILE_DIR:UGridNET>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridCSharpWrapper>" "$<TARGET_FILE_DIR:UGridNET.Tests>"
  COMMENT "Copy $<TARGET_FILE:UGridCSharpWrapper> to $<TARGET_FILE_DIR:UGridNET> and $<TARGET_FILE_DIR:UGridNET.Tests>"
)
