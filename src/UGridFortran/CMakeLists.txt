# Set library files 
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/UGridFortran/*.f90")

# Create the static lib
add_executable(UGridFortran ${SOURCE_LIST})

# Make sure that coverage information is produced when using gcc
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(
    UGridFortran
    PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -static-libgcc -static-libstdc++")
  set_target_properties(UGridFortran PROPERTIES LINK_FLAGS "-ldl -lgcov --coverage")
endif()

add_dependencies(UGridFortran UGridApi)

# Expose the interface of the static lib
target_include_directories(UGridFortran PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_directories(UGridFortran PUBLIC ${NETCDF_LIB})
target_link_libraries(UGridFortran UGridApi)

  