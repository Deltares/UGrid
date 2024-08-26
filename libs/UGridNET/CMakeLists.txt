# default .NET Framework
set(CMAKE_DOTNET_TARGET_FRAMEWORK ${GLOB_DOTNET_TARGET_FRAMEWORK})
set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION ${GLOB_DOTNET_TARGET_FRAMEWORK_VERSION})

#set(CMAKE_CSharp_FLAGS "/langversion:${GLOB_CSHARP_LANGUAGE_VERSION}")
#set(CMAKE_CSharp_FLAGS "${CMAKE_CSharp_FLAGS} /platform:${GLOB_DOTNET_TARGET_PLATFORM}")

add_compile_options("$<$<COMPILE_LANGUAGE:CSharp>:/langversion:${GLOB_CSHARP_LANGUAGE_VERSION}>")
add_compile_options("$<$<COMPILE_LANGUAGE:CSharp>:/platform:${GLOB_DOTNET_TARGET_PLATFORM}>")

set(TOOLS_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/tools)

# Add UGrid static lib
add_subdirectory(SWIG)

# Add UGridAPI dynamic lib
add_subdirectory(dll)

# Add the tests
add_subdirectory(test)

