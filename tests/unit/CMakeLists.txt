# Tests need to be added as executables first
file(GLOB UNIT_TEST_LIST CONFIGURE_DEPENDS "*.cpp")
add_executable(UGridUnitTests ${UNIT_TEST_LIST})

# Expose the interface of the shared lib
set(NETCDF_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf_4_8_0")
message("NETCDF_PATH is ${NETCDF_PATH}")

set(NETCDFCXX_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf-cxx/netcdf-cxx4-4.3.1")
message("NETCDFCXX_PATH is ${NETCDFCXX_PATH}")

# Should be linked to the main library, as well as the google test library
target_link_libraries(UGridUnitTests PRIVATE UGrid UtilsStatic gtest_main gmock "${NETCDF_PATH}/lib/netcdf.lib" "${NETCDFCXX_PATH}/lib/netcdf-cxx4.lib")

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(
    UGridUnitTests
    PROPERTIES COMPILE_FLAGS
               "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++"
  )
  set_target_properties(UGridUnitTests PROPERTIES LINK_FLAGS
                                                       "-ldl -lgcov --coverage")
endif()

# If you register a test, then ctest and make test will run it. You can also run
# examples and check the output, as well. Command can be a target.
add_test(NAME UGridUnitTests COMMAND UGridUnitTests)

# On Windows we bundle the dlls and then copy them to the target file directory
if(WIN32)
  file(
    GLOB
    NETCDF_FILES_LIST
    CONFIGURE_DEPENDS
    "${NETCDF_BIN_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}*${CMAKE_SHARED_LIBRARY_SUFFIX}"
  )
  foreach(CurrentNetcdfLibFile IN LISTS NETCDF_FILES_LIST)
    add_custom_command(
      TARGET UGridUnitTests
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CurrentNetcdfLibFile}
              "$<TARGET_FILE_DIR:UGridUnitTests>"
      COMMENT "Copying netcdf lib file: ${CurrentNetcdfLibFile}")
  endforeach()
endif()
