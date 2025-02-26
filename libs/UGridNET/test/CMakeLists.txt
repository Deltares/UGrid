project(
  UGridNET.Tests
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API tests"
  LANGUAGES CSharp
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

set(NUGET_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/NuGet.config)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../config/NuGet.config
  ${NUGET_CONFIG}
  COPYONLY
)

list(LENGTH GLOB_TEST_DOTNET_TARGET_FRAMEWORKS LIST_LENGTH)
if(${LIST_LENGTH} EQUAL 1)
    set(PROJECT_DOTNET_TARGET_FRAMEWORKS_TAG "TargetFramework")
else()
    set(PROJECT_DOTNET_TARGET_FRAMEWORKS_TAG "TargetFrameworks")
endif()
set(PROJECT_DOTNET_TARGET_FRAMEWORKS ${GLOB_TEST_DOTNET_TARGET_FRAMEWORKS})
set(DIRECTORY_BUILD_PROPS ${CMAKE_CURRENT_BINARY_DIR}/Directory.Build.props)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../config/Directory.Build.props.in
  ${DIRECTORY_BUILD_PROPS}
  @ONLY
)

set(DIRECTORY_PACKAGES_PROPS ${CMAKE_CURRENT_BINARY_DIR}/Directory.Packages.props)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Directory.Packages.props
  ${DIRECTORY_PACKAGES_PROPS}
  COPYONLY
)

execute_process(
  COMMAND ${Python_EXECUTABLE}
    ${CMAKE_SOURCE_DIR}/scripts/get_package_ids_from_directory_packages_props.py
    ${DIRECTORY_PACKAGES_PROPS}
  OUTPUT_VARIABLE PACKAGE_REFERENCES
  RESULT_VARIABLE RETURN_CODE
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL LAST
)

add_library(
  ${TARGET_NAME} 
  SHARED
  ${NUGET_CONFIG}
  ${DIRECTORY_PACKAGES_PROPS}
  ${DIRECTORY_BUILD_PROPS}
  src/ByteArrayExtensionsTests.cs
  src/IntPtrExtensionsTests.cs
  src/IntPtrHelpersTests.cs
  src/StringExtensionsTests.cs
  src/SWIGTests.cs
  src/TestUtilities.cs
  src/TopologyExtensionsTests.cs
  src/UGridNETExceptionTests.cs
  src/UGridReaderTests.cs
  src/UGridWriterTests.cs
)

target_link_libraries(${TARGET_NAME} PRIVATE "$<TARGET_NAME:UGridNET>")

set_target_properties(
  ${TARGET_NAME}
  PROPERTIES
    DOTNET_TARGET_FRAMEWORK "${GLOB_TEST_DOTNET_TARGET_FRAMEWORKS}"
    DOTNET_SDK ${GLOB_DOTNET_SDK}
    VS_PACKAGE_REFERENCES "${PACKAGE_REFERENCES}"
    VS_GLOBAL_PlatformTarget "AnyCPU"
    #VS_GLOBAL_AppendTargetFrameworkToOutputPath "true"
    VS_DOTNET_REFERENCEPROP_UGridNET_TAG_SetTargetFramework "TargetFramework=netstandard2.0"
)

target_compile_options(${TARGET_NAME} PRIVATE "/platform:anycpu")

# add_custom_command(
#   TARGET ${TARGET_NAME}
#   PRE_BUILD
#   COMMAND ${Python_EXECUTABLE}
#     ${CMAKE_SOURCE_DIR}/scripts/append_target_framework_to_output_path_in_csproj.py
#     ${CMAKE_CURRENT_BINARY_DIR}/UGridNET.Test.csproj
#   COMMENT "Configure CS projects to append the target framework to the output path"
# )

# restore nuget packages
add_custom_command(
  TARGET ${TARGET_NAME}
  PRE_BUILD
  COMMAND ${NUGET_EXECUTABLE} restore ${CMAKE_CURRENT_BINARY_DIR}/UGridNET.Tests.sln
    -Force
    -NoHttpCache
    -Verbosity detailed
    -ConfigFile ${CMAKE_CURRENT_BINARY_DIR}//NuGet.config
  COMMENT "Restore NuGet packages"
)

# create empty list of the commands which must to be called in the custom target
set(TARGET_COMMANDS)

# For each test .NET target framework, UGridNET.Test needs the third party dependencies and the SWIG-generated dependencies
foreach(FRAMEWORK ${GLOB_TEST_DOTNET_TARGET_FRAMEWORKS})
  list(
    APPEND
    TARGET_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E echo "Copying ${ALL_RUNTIME_DEPS} to $<TARGET_FILE_DIR:UGridNET.Tests>/${FRAMEWORK}"
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:UGridNET.Tests>/${FRAMEWORK} ${ALL_RUNTIME_DEPS}
  )
endforeach()


add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  ${TARGET_COMMANDS}
  COMMENT "Copying runtime dependencies..."
)