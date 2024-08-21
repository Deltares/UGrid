# Add UGrid static lib
add_subdirectory(SWIG)

# Add UGridAPI dynamic lib
if(ENABLE_CSHARP_TARGETS)
  add_subdirectory(dll)
  add_subdirectory(test)
endif()
