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
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_LINKER_FILE:netCDF::netcdf>" "$<TARGET_FILE_DIR:UGridNET>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_LINKER_FILE:netCDF::netcdf>" "$<TARGET_FILE_DIR:UGridNET.Tests>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_LINKER_FILE:netCDF::netcdf-cxx4>" "$<TARGET_FILE_DIR:UGridNET>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_LINKER_FILE:netCDF::netcdf-cxx4>" "$<TARGET_FILE_DIR:UGridNET.Tests>"
  COMMENT "Copy dlls to target directories"
)
