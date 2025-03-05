project(
  UGridNET
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API"
  LANGUAGES CSharp
)

#set(CMAKE_GENERATOR_PLATFORM win32)

# target name
set(TARGET_NAME ${PROJECT_NAME})

set(NUGET_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/NuGet.config)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../config/NuGet.config
  ${NUGET_CONFIG}
  COPYONLY
)

list(LENGTH GLOB_LIB_DOTNET_TARGET_FRAMEWORKS LIST_LENGTH)
if(${LIST_LENGTH} EQUAL 1)
    set(PROJECT_DOTNET_TARGET_FRAMEWORKS_TAG "TargetFramework")
else()
    set(PROJECT_DOTNET_TARGET_FRAMEWORKS_TAG "TargetFrameworks")
endif()
set(PROJECT_DOTNET_TARGET_FRAMEWORKS ${GLOB_LIB_DOTNET_TARGET_FRAMEWORKS})
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

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(
  SOURCES 
    ${SWIG_GENERATED_CSHARP_SRCS}
    ${SOURCE_DIR}/AssemblyInfo.cs
    ${SOURCE_DIR}/ByteArrayExtensions.cs
    ${SOURCE_DIR}/ContactsExtensions.cs
    ${SOURCE_DIR}/DisposableMesh2D.cs
    ${SOURCE_DIR}/DisposableNativeObject.cs
    ${SOURCE_DIR}/IntPtrExtensions.cs
    ${SOURCE_DIR}/IntPtrHelpers.cs
    ${SOURCE_DIR}/Mesh1DExtensions.cs
    ${SOURCE_DIR}/Mesh2DExtensions.cs
    ${SOURCE_DIR}/Network1DExtensions.cs
    ${SOURCE_DIR}/ProjectedCoordinateSystem.cs
    ${SOURCE_DIR}/StringExtensions.cs
    ${SOURCE_DIR}/UGridNETException.cs
    ${SOURCE_DIR}/UGridBase.cs
    ${SOURCE_DIR}/UGridReader.cs
    ${SOURCE_DIR}/UGridWriter.cs
)

add_library(
  ${TARGET_NAME}
  SHARED 
  ${NUGET_CONFIG}
  ${DIRECTORY_PACKAGES_PROPS}
  ${DIRECTORY_BUILD_PROPS}
  ${SOURCES}
)

set_target_properties(
  ${TARGET_NAME}
  PROPERTIES
    DOTNET_TARGET_FRAMEWORK "${GLOB_LIB_DOTNET_TARGET_FRAMEWORKS}"
    DOTNET_SDK ${GLOB_DOTNET_SDK}
    VS_PACKAGE_REFERENCES "${PACKAGE_REFERENCES}"
    VS_GLOBAL_PlatformTarget "AnyCPU"
    #VS_GLOBAL_AppendTargetFrameworkToOutputPath "true"
)

target_compile_options(${TARGET_NAME} PRIVATE "/unsafe")

target_compile_options(${TARGET_NAME} PRIVATE "/platform:anycpu")

# add_custom_command(
#   TARGET ${TARGET_NAME}
#   PRE_BUILD
#   COMMAND ${Python_EXECUTABLE}
#     ${CMAKE_SOURCE_DIR}/scripts/append_target_framework_to_output_path_in_csproj.py
#     ${CMAKE_CURRENT_BINARY_DIR}/UGridNET.csproj
#   COMMENT "Configure CS projects to append the target framework to the output path"
# )

# restore nuget packages
add_custom_command(
  TARGET ${TARGET_NAME}
  PRE_BUILD
  COMMAND ${NUGET_EXECUTABLE} restore ${CMAKE_CURRENT_BINARY_DIR}/UGridNET.sln
    -Force
    -NoHttpCache
    -Verbosity detailed
    -ConfigFile ${CMAKE_CURRENT_BINARY_DIR}//NuGet.config
  COMMENT "Restore NuGet packages"
)

# create empty list of the commands which must to be called in the custom target
set(TARGET_COMMANDS)

# For each supported .NET target framework, UGridNET needs the third party dependencies and the SWIG-generated dependencies
foreach(FRAMEWORK ${GLOB_LIB_DOTNET_TARGET_FRAMEWORKS})
  list(
    APPEND
    TARGET_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E echo "Copying ${ALL_RUNTIME_DEPS} to $<TARGET_FILE_DIR:UGridNET>/${FRAMEWORK}"
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:UGridNET>/${FRAMEWORK} ${ALL_RUNTIME_DEPS}
  )
endforeach()


add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  ${TARGET_COMMANDS}
  COMMENT "Copying runtime dependencies..."
)