project(
  UGridNET
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API"
  LANGUAGES CSharp
)

include(CSharpUtilities)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# Specify the version of .NET to be used
set(DOTNET_VERSION "net6.0")

# Specify the source files
set(SWIG_GENERATED_CSHARP_SRC_DIR ${CMAKE_BINARY_DIR}/libs/UGridNET/SWIG/generated/csharp)
message("${SWIG_GENERATED_CSHARP_SRC_DIR}")
file(GLOB SWIG_GENERATED_CSHARP_SRCS CONFIGURE_DEPENDS "${SWIG_GENERATED_CSHARP_SRC_DIR}/*.cs")

# Create a shared library (DLL)
add_library(${TARGET_NAME} SHARED ${SWIG_GENERATED_CSHARP_SRCS})

# Specify target framework
set_property(
    TARGET ${TARGET_NAME}
    PROPERTY
      VS_GLOBAL_TargetFramework "netstandard2.0"
)

# Set C# compiler options
set_property(
    TARGET ${TARGET_NAME}
    PROPERTY
      VS_DOTNET_REFERENCES "System"
      VS_GLOBAL_ROOTNAMESPACE ${TARGET_NAME}
)

target_link_libraries(${TARGET_NAME} PRIVATE UGridCSWrapper)

