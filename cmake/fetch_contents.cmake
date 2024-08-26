include(FetchContent)

#set(FETCHCONTENT_QUIET off)

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

  # Use with CMake 3.28+ instead ogf FetchContent_Populate
  #FetchContent_MakeAvailable(googletest EXCLUDE_FROM_ALL )

  FetchContent_GetProperties(googletest)
  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
  
  #include(CTest)
  enable_testing()
endif()
