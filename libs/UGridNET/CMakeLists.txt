# Download necessary tools
add_subdirectory(tools)

# Add UGrid (SWIG-based) dynamic lib
add_subdirectory(SWIG)

# Add UGridNET dynamic lib
add_subdirectory(dll)

# Add the UGridNET tests
add_subdirectory(test)

# Add nuget config
add_subdirectory(nuget)
