# Add UGrid static lib
add_subdirectory(UGrid)

# Add UGridAPI dynamic lib
add_subdirectory(UGridAPI)

# Add UGridNET
if(BUILD_DOTNET_WRAPPER)
  add_subdirectory(UGridNET)
endif()