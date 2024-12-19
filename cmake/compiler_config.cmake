# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# Create position-independent executables and shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add compiler-specific options and definitions per supported platform
if (UNIX)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-fvisibility=hidden;-Werror;-Wall;-Wextra;-pedantic;-Wno-unused-function>")
    add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>:-O2>")
    add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>:-g>")
  else()
    message(FATAL_ERROR "Unsupported compiler. Only GNU is supported under Linux. Found ${CMAKE_CXX_COMPILER_ID}.")
  endif()
elseif(WIN32)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/EHsc;/MP;/MD;/W3;/WX>")
    add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>:/O2>")
    add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>:/Od;/DEBUG>")
    add_compile_definitions("$<$<COMPILE_LANGUAGE:CXX>:_USE_MATH_DEFINES>")
    add_compile_definitions("$<$<COMPILE_LANGUAGE:CXX>:_CRT_SECURE_NO_WARNINGS>")
  else()
    message(FATAL_ERROR "Unsupported compiler. Only MSVC is supported under Windows. Found ${CMAKE_CXX_COMPILER_ID}.")
  endif()
else()
    message(FATAL_ERROR "Unsupported platform. Only Linux and Windows are supported.")
endif()
