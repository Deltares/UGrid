# Gets the current branch name
function(get_current_branch_name branch_name)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH_NAME
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL LAST
  )
  set(${branch_name} "${GIT_BRANCH_NAME}" PARENT_SCOPE)
endfunction()

# Checks if the branch name is "main" or starts with "release/"
function(is_main_or_release_branch branch_name result)
  if("${branch_name}" STREQUAL "main" OR "${branch_name}" MATCHES "^release/")
    set(${result} "true" PARENT_SCOPE)
  else()
    set(${result} "false" PARENT_SCOPE)
  endif()
endfunction()