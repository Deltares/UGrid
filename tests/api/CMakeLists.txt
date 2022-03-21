# Tests need to be added as executables first
file(GLOB API_TEST_LIST CONFIGURE_DEPENDS "*.cpp")

# Make a test executable
set(target_name UGridApiTests)
add_executable(${target_name} ${API_TEST_LIST})

# Add a dependency to ensure the correct build order
add_dependencies(${target_name} UGridApi)

# Add precompiled header 
target_precompile_headers(${target_name} PRIVATE <map> 
                                        <string> 
                                        <exception>
                                        <unordered_map> 
                                        <vector> 
                                        <memory>
                                        <sstream>
                                        <ncDim.h> 
                                        <ncVar.h> 
                                        <ncFile.h>)
# Should be linked to the main library, as well as the google test library
target_link_libraries(${target_name} PRIVATE UGridApi UtilsStatic
                                                 gtest_main gmock)

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++")
    set_target_properties(${target_name} PROPERTIES LINK_FLAGS "-ldl -lgcov --coverage")
endif()

# Link shared lib to static lib, but don't expose it
target_link_directories(${target_name} PRIVATE ${netCDF_LIB_DIR})
target_link_libraries(${target_name} PRIVATE UGrid netCDF::netcdf-cxx4 netcdf)

# If you register a test, then ctest and make test will run it. You can also run
# examples and check the output, as well. Command can be a target.
add_test(NAME ${target_name} COMMAND UGridApiTests)

# Copy the UGrid shared library to the target directory
add_custom_command(
  TARGET ${target_name}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridApi>"
          "$<TARGET_FILE_DIR:UGridApiTests>"
  COMMENT "Copying UGrid shared library")
