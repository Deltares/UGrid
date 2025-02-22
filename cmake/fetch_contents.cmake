include(FetchContent)

if(ENABLE_UNIT_TESTING)

# Fetch googletest
  set(googletest_url "https://github.com/google/googletest.git")
  set(googletest_tag "v1.15.1")
  set(cmake_FetchContent_Populate_deprecation_version "3.30.0")

  if(CMAKE_VERSION VERSION_GREATER_EQUAL "${cmake_FetchContent_Populate_deprecation_version}")
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY ${googletest_url}
      GIT_TAG ${googletest_tag}
      EXCLUDE_FROM_ALL # available from v3.28.0+
    )
  else()
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY ${googletest_url}
      GIT_TAG ${googletest_tag}
    )
  endif()

  if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()

  if(CMAKE_VERSION VERSION_GREATER_EQUAL "${cmake_FetchContent_Populate_deprecation_version}")
    FetchContent_MakeAvailable(googletest)
  else()
    # deprecated
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
      FetchContent_Populate(googletest)
      add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
  endif()
  
  enable_testing()
endif()
