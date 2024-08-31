# Download necessary tools
add_subdirectory(tools)

# Add UGrid static lib
add_subdirectory(SWIG)

# Add UGridNET dynamic lib
add_subdirectory(dll)

# Add the UGridNET tests
add_subdirectory(test)

add_subdirectory(post_build)

