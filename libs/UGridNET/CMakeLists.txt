# Download necessary tools
add_subdirectory(tools)

# Add UGrid (SWIG-based)dynamic lib
add_subdirectory(SWIG)

# Add UGridNET dynamic lib
add_subdirectory(dll)

# Add the UGridNET tests
add_subdirectory(test)

# Add pre-build (post config) targets
add_subdirectory(pre_build)

# Add post-build targets
add_subdirectory(post_build)

