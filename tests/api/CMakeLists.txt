# project name
project(
  UGridAPITests
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGridAPI tests"
  LANGUAGES CXX C
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# Make a test executable
add_executable(${TARGET_NAME})

# source directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# include directory
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# list of target sources
set(SRC_LIST
    ${SRC_DIR}/APITests.cpp
)

set(INC_LIST)

# add sources to target
target_sources(${TARGET_NAME} PRIVATE ${SRC_LIST} ${INC_LIST})

# Expose the interface of the shared lib
target_include_directories(${TARGET_NAME} PUBLIC ${INC_DIR})

# Should be linked to the main library, as well as the google test library
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE
    "$<TARGET_NAME:UGridAPI>"
    TestUtils
    GTest::gmock
    GTest::gtest_main
)

# If you register a test, then ctest and make test will run it. You can also run
# examples and check the output, as well. Command can be a target.
add_test(
  NAME ${TARGET_NAME}
  COMMAND ${TARGET_NAME}
)

# Copy the UGridAPI shared library to the target directory
add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridAPI>" "$<TARGET_FILE_DIR:UGridAPITests>"
    COMMENT "Copy $<TARGET_FILE:UGridAPI> to $<TARGET_FILE_DIR:UGridAPITests>"
)

# group the sources in IDE tree
source_group("Source Files" FILES ${SRC_LIST})

# group the headers in IDE tree
source_group("Header Files" FILES ${INC_LIST})
