project(
  UGridNET
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API"
  LANGUAGES CSharp
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# set(ASSEMBLY_INFO_CS ${CMAKE_CURRENT_BINARY_DIR}/Properties/AssemblyInfo.cs)
# configure_file(
#   ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/AssemblyInfo.cs.in
#   ${ASSEMBLY_INFO_CS}
#   @ONLY
# )

set(APP_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/App.config)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/App.config.in
  ${APP_CONFIG}
  @ONLY
)

set(PACKAGE_REFERENCES
  "System.Buffers_4.5.1"
  "System.Memory_4.5.5"
)

set(PACKAGES_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/packages.config)

include(${CMAKE_SOURCE_DIR}/cmake/configure_dotnet_packages.cmake)
configure_dotnet_packages(
  ${PACKAGES_CONFIG}
  "${PACKAGE_REFERENCES}"
  ${GLOB_DOTNET_TARGET_FRAMEWORK}
)

add_library(
  ${TARGET_NAME}
  SHARED 
  #${ASSEMBLY_INFO_CS}
  ${APP_CONFIG}
  ${PACKAGES_CONFIG}
  ${SWIG_GENERATED_CSHARP_SRCS}
)

execute_process(
  COMMAND ${NUGET_EXECUTABLE} restore packages.config
    -SolutionDirectory ${CMAKE_CURRENT_BINARY_DIR}
    -Force
    -NoHttpCache
    -Verbosity detailed
    -Source https://api.nuget.org/v3/index.json
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RESULT_VARIABLE NUGET_RESULT
  ERROR_VARIABLE NUGET_ERROR
  OUTPUT_VARIABLE NUGET_OUTPUT
  ECHO_OUTPUT_VARIABLE
  ECHO_ERROR_VARIABLE
  COMMAND_ECHO STDOUT
  COMMAND_ERROR_IS_FATAL LAST
)

set_target_properties(
  ${TARGET_NAME}
  PROPERTIES
    DOTNET_TARGET_FRAMEWORK ${GLOB_DOTNET_TARGET_FRAMEWORK}
    DOTNET_TARGET_FRAMEWORK_VERSION ${GLOB_DOTNET_TARGET_FRAMEWORK_VERSION}
    DOTNET_SDK ${GLOB_DOTNET_SDK}
    VS_DOTNET_REFERENCES "System"
    VS_PACKAGE_REFERENCES "${PACKAGE_REFERENCES}"
)
