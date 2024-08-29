set(TOOLS_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/tools)

# Add UGrid static lib
add_subdirectory(SWIG)

# default .NET Framework
# set(CMAKE_DOTNET_TARGET_FRAMEWORK ${GLOB_DOTNET_TARGET_FRAMEWORK})
# set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION ${GLOB_DOTNET_TARGET_FRAMEWORK_VERSION})
# set(CMAKE_DOTNET_SDK "Microsoft.NET.Sdk")

#set(CMAKE_CSharp_FLAGS "${CMAKE_CSharp_FLAGS} /platform:${GLOB_DOTNET_TARGET_PLATFORM}")
#set(CMAKE_CSharp_FLAGS "/langversion:${GLOB_CSHARP_LANGUAGE_VERSION}")

#add_compile_options("$<$<COMPILE_LANGUAGE:CSharp>:/platform:${GLOB_DOTNET_TARGET_PLATFORM}>")
#add_compile_options("$<$<COMPILE_LANGUAGE:CSharp>:/langversion:${GLOB_CSHARP_LANGUAGE_VERSION}>")


# Add UGridNET dynamic lib
add_subdirectory(dll)

# Add the UGridNET tests
add_subdirectory(test)

add_subdirectory(post_build)

