# Tests need to be added as executables first
file(GLOB API_TEST_LIST CONFIGURE_DEPENDS "*.cpp")

# Make a test executable
add_executable(UGridApiTests ${API_TEST_LIST})

# Add a dependency to ensure the correct build order
add_dependencies(UGridApiTests UGridApi)

# Should be linked to the main library, as well as the google test library
target_link_libraries(UGridApiTests PRIVATE UGridApi UtilsStatic
                                                 gtest_main gmock)

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(
    UGridApiTests
    PROPERTIES COMPILE_FLAGS
               "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++"
  )
  set_target_properties(UGridApiTests PROPERTIES LINK_FLAGS
                                                      "-ldl -lgcov --coverage")
endif()

# If you register a test, then ctest and make test will run it. You can also run
# examples and check the output, as well. Command can be a target.
add_test(NAME UGridApiTests COMMAND UGridApiTests)

# On Windows we bundle the netcdf dlls and then copy them to the target file
# directory
set(NETCDF_PATH "${PROJECT_SOURCE_DIR}/extern/netcdf/netcdf_4_8_0")
message("NETCDF_PATH is ${NETCDF_PATH}")
if(WIN32)
  file(GLOB NETCDF_FILES_LIST CONFIGURE_DEPENDS
       "${NETCDF_PATH}/bin/*.dll")
  foreach(CurrentNetcdfLibFile IN LISTS NETCDF_FILES_LIST)
    add_custom_command(
      TARGET UGridApiTests
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CurrentNetcdfLibFile}
              "$<TARGET_FILE_DIR:UGridApiTests>"
      COMMENT "Copying netcdf lib file: ${CurrentNetcdfLibFile}")
  endforeach()
endif()

# Copy the UGrid shared library to the target directory
add_custom_command(
  TARGET UGridApiTests
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridApi>"
          "$<TARGET_FILE_DIR:UGridApiTests>"
  COMMENT "Copying UGrid shared library")
