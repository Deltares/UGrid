configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
               ${CMAKE_BINARY_DIR}/version.rc @ONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGrid.Native.nuspec.in
  ${CMAKE_BINARY_DIR}/package/Deltares.UGrid.Native.nuspec @ONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGrid.Devel.nuspec.in
  ${CMAKE_BINARY_DIR}/package/Deltares.UGrid.Devel.nuspec @ONLY)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Deltares.UGrid.Release.nuspec.in
  ${CMAKE_BINARY_DIR}/package/Deltares.UGrid.Release.nuspec @ONLY)

configure_file(Deltares.UGrid.targets
               ${CMAKE_BINARY_DIR}/package/Deltares.UGrid.targets COPYONLY)

configure_file(Deltares.UGrid.Native.targets
               ${CMAKE_BINARY_DIR}/package/Deltares.UGrid.Native.targets COPYONLY)