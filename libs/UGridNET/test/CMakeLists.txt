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
  ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/AssemblyInfo.cs.template
  ${ASSEMBLY_INFO_CS}
)

# Create a shared library (DLL)
add_library(${TARGET_NAME} SHARED src/UGridNETTests.cs)

target_link_libraries(${TARGET_NAME} PRIVATE UGridNET)

target_compile_options(${TARGET_NAME} PUBLIC "/platform:${DOTNET_TARGET_PLATFORM}")

set_property(
  TARGET ${TARGET_NAME}
  PROPERTY
    VS_DOTNET_REFERENCES "System"
    VS_PACKAGE_REFERENCES "NUnit_4.1.0"
    VS_GLOBAL_ROOTNAMESPACE ${TARGET_NAME}
    VS_DOTNET_TARGET_FRAMEWORK ${DOTNET_TARGET_FRAMEWORK}
    VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DOTNET_TARGET_FRAMEWORK_VERSION}
)

set(NUGET_EXE ${CMAKE_BINARY_DIR}/nuget.exe)
if(NOT EXISTS ${NUGET_EXE})
  set(NUGET_URL "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe")
  file(DOWNLOAD ${NUGET_URL} ${NUGET_EXE})
endif()

set(WHERE_TO_DO_IT ${CMAKE_CURRENT_BINARY_DIR})
set(SOLUTION_NAME_PREFIX ${TARGET_NAME})

#set(WHERE_TO_DO_IT ${CMAKE_BINARY_DIR})
#set(SOLUTION_NAME_PREFIX ${CMAKE_BINARY_DIR}/UGrid.sln)

set(SOLUTION_NAME ${WHERE_TO_DO_IT}/${SOLUTION_NAME_PREFIX}.sln)
message(">>>>>>>>>>>>>>>> ${SOLUTION_NAME}")
file(MAKE_DIRECTORY ${WHERE_TO_DO_IT}/packages)
add_custom_command(
  TARGET ${TARGET_NAME} 
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Installing nuget packages"
  COMMAND ${NUGET_EXE} install NUnit
    -Version 4.1.0
    -OutputDirectory ${WHERE_TO_DO_IT}/packages 
    -Source "https://api.nuget.org/v3/index.json"
  COMMAND ${NUGET_EXE} restore ${SOLUTION_NAME} -OutputDirectory ${WHERE_TO_DO_IT}/packages -verbosity detailed
  #COMMAND dotnet restore ${SOLUTION_NAME} #-Source ${WHERE_TO_DO_IT}/packages 
  WORKING_DIRECTORY ${WHERE_TO_DO_IT}
  COMMENT "NuGet packages installation"
)

#Copy UGridCSWrapper
add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridCSWrapper>" "$<TARGET_FILE_DIR:UGridNETTests>"
    COMMENT "Copy $<TARGET_FILE:UGridCSWrapper> to $<TARGET_FILE_DIR:UGridNETTests>"
)
