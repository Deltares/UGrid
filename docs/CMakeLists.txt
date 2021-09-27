find_package(Doxygen)
if(NOT Doxygen_FOUND)
  message(STATUS "Doxygen not found, not building docs")
  return()
endif()

set(DOXYGEN_INPUT_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/main_page.md \
    ${PROJECT_SOURCE_DIR}/include/UGrid   \
    ${PROJECT_SOURCE_DIR}/include/UGridApi")
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
set(DOXYFILE_EXTRACT_PRIVATE "FALSE")

# This is only used for the CI
set(DOXYGEN_WARN_LOG_FILE "")

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# UGrid headers
file(GLOB UGrid_HEADER_LIST CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/include/UGrid/*.hpp")

# UGridapi headers
file(GLOB UGridAPI_HEADER_LIST CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/include/UGridApi/*.hpp")

# markdown file
file(GLOB DOXYGEN_MARKDOWN_FILES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.md")

# Only regenerate Doxygen when the Doxyfile or public headers change
add_custom_command(
  OUTPUT ${DOXYGEN_INDEX_FILE}
  DEPENDS ${UGrid_HEADER_LIST} ${UGridAPI_HEADER_LIST}
          ${DOXYGEN_MARKDOWN_FILES} 
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include"
  MAIN_DEPENDENCY ${DOXYFILE_OUT}
  ${DOXYFILE_IN}
  COMMENT "Generating docs")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})
