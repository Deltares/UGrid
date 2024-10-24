# version
set(UGRID_PROJECT_VERSION_MAJOR 0)
set(UGRID_PROJECT_VERSION_MINOR 0)
set(UGRID_PROJECT_VERSION_PATCH 0)
if(DEFINED ENV{UGRID_BUILD_NUMBER})
  set(UGRID_PROJECT_VERSION_TWEAK $ENV{UGRID_BUILD_NUMBER})
else()
  set(UGRID_PROJECT_VERSION_TWEAK 0)
endif()
set(
  GLOB_UGRID_PROJECT_VERSION 
  "${UGRID_PROJECT_VERSION_MAJOR}.${UGRID_PROJECT_VERSION_MINOR}.${UGRID_PROJECT_VERSION_PATCH}.${UGRID_PROJECT_VERSION_TWEAK}"
  CACHE STRING "Project version"
)

# year
string(TIMESTAMP GLOB_CURRENT_YEAR "%Y")
set(GLOB_CURRENT_YEAR ${GLOB_CURRENT_YEAR} CACHE STRING "Current year")

# .NET config
if(WIN32 AND BUILD_DOTNET_WRAPPER)
  # .NET SDK
  set(GLOB_DOTNET_SDK "Microsoft.NET.Sdk" CACHE STRING ".NET SDK")
  # .NET target frameworks
  # Note: Even if the list consists of a single element, make sure to terminate with a semicolon
  #       For ex: set(LIST_OF_DOTNET_TARGET_FRAMEWORKS "net481;" CACHE STRING "Supported .NET target framework")
  #       Otherwise, Visual Studio will not create subdir net481 under Release (of whichever build config is used) in the CMake binary dir
  #       <TargetFramework>net481</TargetFramework> works (notice the singular form in xml element "TargetFramework")
  #       <TargetFrameworks>net481</TargetFrameworks> does not work (notice the plural form in xml element "TargetFrameworks")
  #       <TargetFrameworks>net481;</TargetFrameworks> works (notice the plural form in xml element "TargetFrameworks" and the semicolon after "net481")
  #       Is this a VS bug? "TargetFramework" should be a special case of "TargetFrameworks" when the length of the list is 1.
  set(GLOB_LIB_DOTNET_TARGET_FRAMEWORKS "netstandard2.0;net481" CACHE STRING "Library .NET target framework")
  set(GLOB_TEST_DOTNET_TARGET_FRAMEWORKS "net481;" CACHE STRING "Test .NET target framework")
  # .NET language version
  set(GLOB_CSHARP_LANGUAGE_VERSION "7.3" CACHE STRING "C# Compiler Options for language feature rules") 
endif()
