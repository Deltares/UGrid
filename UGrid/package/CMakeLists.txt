# Configure DLL version
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_BINARY_DIR}/version.rc @ONLY
)

# Configure the version header file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version/Version.hpp.in
               ${CMAKE_CURRENT_SOURCE_DIR}/Version/Version.hpp @ONLY)