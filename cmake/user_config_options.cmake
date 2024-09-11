# unit testing option
option(
  ENABLE_UNIT_TESTING
  "Enables building the unit test executables"
  ON
)

# code coverage option
if(LINUX AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  option(
    ENABLE_CODE_COVERAGE
    "Generates code coverage statistics."
    OFF
  )
endif()

option(BUILD_DOTNET_WRAPPER "Configures and builds the C# targets that use code generated by SWIG" OFF)
