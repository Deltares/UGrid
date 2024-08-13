include(FetchContent)
set(FETCHCONTENT_QUIET off)

if(ENABLE_UNIT_TESTING)
  # Fetch google test
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
  )

  if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()

  # FetchContent_GetProperties(googletest)
  # if(NOT googletest_POPULATED)
  #   FetchContent_Populate(googletest)
  #   add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
  # endif()

  FetchContent_MakeAvailable(googletest)

  include(CTest)
endif()
