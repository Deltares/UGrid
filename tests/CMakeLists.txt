set(TEST_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/utils/include/TestUtils/Definitions.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/utils/include/TestUtils/Definitions.hpp")

add_subdirectory(utils)

add_subdirectory(api)
