project(
  UGridNETTests
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API tests"
  LANGUAGES CSharp
)

include(CSharpUtilities)

# target name
set(TARGET_NAME ${PROJECT_NAME})

set(ASSEMBLY_INFO_CS ${CMAKE_CURRENT_BINARY_DIR}/Properties/AssemblyInfo.cs)
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/AssemblyInfo.cs.in
  ${ASSEMBLY_INFO_CS}
)

set(APP_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/App.config)
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/App.config.in
  ${APP_CONFIG}
)

set(PACKAGES_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/packages.config)
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/packages.config.in
  ${PACKAGES_CONFIG}
)

add_library(${TARGET_NAME} 
  SHARED
  ${ASSEMBLY_INFO_CS}
  ${APP_CONFIG}
  ${PACKAGES_CONFIG}
  src/UGridNETTests.cs
)

target_link_libraries(${TARGET_NAME} PRIVATE UGridNET)

#target_compile_options(${TARGET_NAME} PRIVATE "/platform:${GLOB_DOTNET_TARGET_PLATFORM}")
#target_compile_options(${TARGET_NAME} PRIVATE "/langversion:${GLOB_CSHARP_LANGUAGE_VERSION}")

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DOTNET_REFERENCES "System")
set_property(TARGET ${TARGET_NAME} PROPERTY VS_PACKAGE_REFERENCES "NUnit_4.1.0;NUnit.ConsoleRunner_3.18.1;NUnit3TestAdapter_4.6.0;nunit.framework.dll")

set(NUGET_DOWNLOAD_DIR ${TOOLS_INSTALL_DIR}/nuget)
file(MAKE_DIRECTORY ${NUGET_DOWNLOAD_DIR})
set(NUGET_EXE ${NUGET_DOWNLOAD_DIR}/nuget.exe)
if(NOT EXISTS ${NUGET_EXE})
  set(NUGET_URL "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe")
  file(DOWNLOAD ${NUGET_URL} ${NUGET_EXE})
endif()

add_custom_command(
  TARGET ${TARGET_NAME} 
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "[[[ NuGet: Restoring packages ]]]"  
  COMMAND ${NUGET_EXE} restore ${TARGET_NAME}.sln    # also packages.config works
    -SolutionDirectory ${CMAKE_CURRENT_BINARY_DIR}
    -Force
    -NoHttpCache
    -Verbosity detailed
  COMMAND ${CMAKE_COMMAND} -E echo "[[[ NuGet: Done ]]]"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "NuGet packages installation"
)

#Copy UGridCSWrapper
# add_custom_command(
#   TARGET ${TARGET_NAME}
#   POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridCSWrapper>" "$<TARGET_FILE_DIR:UGridNETTests>"
#     COMMENT "Copy $<TARGET_FILE:UGridCSWrapper> to $<TARGET_FILE_DIR:UGridNETTests>"
# )

add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
    COMMAND echo "[[[[ ${TARGET_NAME}  : DONE ]]]"
)