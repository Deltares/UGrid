# Download google test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)

FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include(CTest)

set(TEST_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/utils/include/TestUtils/Definitions.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/utils/include/TestUtils/Definitions.hpp")

add_subdirectory(utils)
add_subdirectory(api)
