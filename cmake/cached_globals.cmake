string(TIMESTAMP GLOB_CURRENT_YEAR "%Y")
set(GLOB_CURRENT_YEAR ${GLOB_CURRENT_YEAR} CACHE STRING "Current year")

if(WIN32 AND BUILD_DOTNET_WRAPPER)
  # set(GLOB_DOTNET_TARGET_FRAMEWORK "net462" CACHE STRING ".NET target framework")
  # set(GLOB_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.2" CACHE STRING ".NET target framework version")
  # set(GLOB_CSHARP_LANGUAGE_VERSION "6" CACHE STRING "C# Compiler Options for language feature rules")

  # set(GLOB_DOTNET_TARGET_FRAMEWORK "net472" CACHE STRING ".NET target framework")
  # set(GLOB_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2" CACHE STRING ".NET target framework version")
  # set(GLOB_CSHARP_LANGUAGE_VERSION "7.3" CACHE STRING "C# Compiler Options for language feature rules")

  #set(GLOB_DOTNET_TARGET_FRAMEWORK "netstandard2.0" CACHE STRING ".NET target framework")
  set(GLOB_DOTNET_TARGET_FRAMEWORK "net481" CACHE STRING ".NET target framework")
  set(GLOB_DOTNET_TARGET_FRAMEWORK_VERSION "v4.8.1" CACHE STRING ".NET target framework version")
  set(GLOB_CSHARP_LANGUAGE_VERSION "7.3" CACHE STRING "C# Compiler Options for language feature rules")

  # set(GLOB_DOTNET_TARGET_FRAMEWORK "net40" CACHE STRING ".NET target framework")
  # set(GLOB_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0.0" CACHE STRING ".NET target framework version")
  # set(GLOB_CSHARP_LANGUAGE_VERSION "4" CACHE STRING "C# Compiler Options for language feature rules")

  set(GLOB_DOTNET_TARGET_PLATFORM "x64" CACHE STRING ".NET target platform")
endif()
