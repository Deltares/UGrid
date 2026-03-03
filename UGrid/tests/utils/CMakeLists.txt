# project name
project(
  TestUtils
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "Test utilities"
  LANGUAGES CXX
)

# target name
set(TARGET_NAME ${PROJECT_NAME})

# static library
add_library(${TARGET_NAME} STATIC)

# source directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# include directory
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DOMAIN_INC_DIR ${INC_DIR}/TestUtils)

# list of target sources
set(SRC_LIST
  ${SRC_DIR}/Utils.cpp
)

set(INC_LIST
  ${DOMAIN_INC_DIR}/Utils.hpp
  ${DOMAIN_INC_DIR}/Definitions.hpp
)

# add sources to target
# add sources to target
target_sources(
  ${TARGET_NAME}
  PRIVATE
    ${SRC_LIST}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${INC_DIR}
      FILES
        ${INC_LIST}
)

# Expose the interface of the shared lib
target_include_directories(${TARGET_NAME} PUBLIC ${INC_DIR})

# Should be linked to the main library, as well as the google test library
target_link_libraries(${TARGET_NAME} PRIVATE UGrid)

# group the sources in IDE tree
source_group("Source Files" FILES ${SRC_LIST})

# group the headers in IDE tree
source_group("Header Files" FILES ${INC_LIST})
