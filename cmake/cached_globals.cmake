string(TIMESTAMP GLOB_CURRENT_YEAR "%Y")
set(GLOB_CURRENT_YEAR ${GLOB_CURRENT_YEAR} CACHE STRING "Current year")

if (WIN32)
  set(DOTNET_TARGET_FRAMEWORK "netstandard2.0" CACHE STRING ".NET target framework")
  set(DOTNET_TARGET_FRAMEWORK_VERSION "v4.8.1" CACHE STRING ".NET target framework version")
  set(DOTNET_TARGET_PLATFORM "x64" CACHE STRING ".NET target platform")
endif()

