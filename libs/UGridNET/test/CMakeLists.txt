project(
  UGridNETTests
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API tests"
  LANGUAGES CSharp
)

include(CSharpUtilities)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# Specify the version of .NET to be used
set(DOTNET_VERSION "net6.0")

# Create a shared library (DLL)
add_library(${TARGET_NAME} SHARED src/UGridNETTests.cs)

# Specify target framework
set_property(
  TARGET ${TARGET_NAME}
  PROPERTY
    VS_GLOBAL_TargetFramework "netstandard2.0"
)

#Set C# compiler options
set_property(
  TARGET ${TARGET_NAME}
  PROPERTY
    VS_DOTNET_REFERENCES
      "System"
)

set_property(
  TARGET ${TARGET_NAME}
  PROPERTY 
    VS_PACKAGE_REFERENCES "nunit_4.1.0"
)

#set(SOLUTION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.sln")

# add_custom_target(
#   dotnet_restore
#   DEPENDS ${SOLUTION_FILE}
#   COMMAND dotnet restore ${SOLUTION_FILE}
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#   COMMENT "Restoring NuGet packages using dotnet..."
# )

# add_custom_command(
#     TARGET ${TARGET_NAME}
#     PRE_BUILD
#     COMMAND dotnet restore ${SOLUTION_FILE}
# )

target_link_libraries(${TARGET_NAME} PRIVATE UGridNET)

