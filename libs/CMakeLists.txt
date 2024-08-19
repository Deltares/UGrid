# Add UGrid static lib
add_subdirectory(UGrid)

# Add UGridAPI dynamic lib
add_subdirectory(UGridAPI)

# Add UGridNET
if(WIN32)
  add_subdirectory(UGridNET)
endif