if(WIN32)

  function(check_arguments function_name args_prefix required_args)
    set(missing_required_args FALSE)
    set(missing_args_msg "")
    foreach(arg IN LISTS required_args)
      if (NOT DEFINED ${args_prefix}_${arg})
        string(APPEND missing_args_msg "  ${arg} argument is required but it was not provided.\n")
        set(missing_required_args TRUE)
      endif()
    endforeach()
    if(missing_required_args)
        message(FATAL_ERROR "Error in function ${function_name}: One or more required arguments are missing.\n${missing_args_msg}")
    endif()
  endfunction()

  # Downloads and installs SWIG
  function(install_swig)
    set(prefix ARGS)
    set(options SWIG_CLEAN)
    set(one_value_args SWIG_VERSION SWIG_INSTALL_PREFIX SWIG_DIR_OUT SWIG_EXECUTABLE_OUT)
    set(multi_value_args "")

    # Parse the arguments
    cmake_parse_arguments(
      "${prefix}" 
      "${options}"
      "${one_value_args}"
      "${multi_value_args}"
      ${ARGN}
    )

    # Check if all required arguments are provided
    set(required_args ${options} ${one_value_args} ${multi_value_args})
    check_arguments("${CMAKE_CURRENT_FUNCTION}" "${prefix}" "${required_args}")

    message(STATUS "Installing SWIG ${ARGS_SWIG_VERSION}...")

    # Set the download URL and file paths
    set(SWIG_URL "http://prdownloads.sourceforge.net/swig/swigwin-${ARGS_SWIG_VERSION}.zip")
    set(DOWNLOAD_DIR "${ARGS_SWIG_INSTALL_PREFIX}/download")

    # Create the download and extraction directories
    file(MAKE_DIRECTORY ${ARGS_SWIG_INSTALL_PREFIX})
    file(MAKE_DIRECTORY ${DOWNLOAD_DIR})

    # Download the SWIG zip file
    set(SWIG_ZIP "${DOWNLOAD_DIR}/swigwin-${ARGS_SWIG_VERSION}.zip")
    if(NOT EXISTS ${SWIG_ZIP})
      file(DOWNLOAD ${SWIG_URL} ${SWIG_ZIP})
    endif()

    # Unzip the downloaded file
    set(INSTALL_DIR ${ARGS_SWIG_INSTALL_PREFIX}/swigwin-${ARGS_SWIG_VERSION})
    if(NOT EXISTS ${INSTALL_DIR})
      file(ARCHIVE_EXTRACT 
        INPUT ${SWIG_ZIP} 
        DESTINATION ${ARGS_SWIG_INSTALL_PREFIX})
    endif()

    # Remove the zip file after extraction
    if(${ARGS_SWIG_CLEAN})
      file(REMOVE_RECURSE ${DOWNLOAD_DIR})
    endif()

    # Set the output
    set(LIBRARY_DIR ${INSTALL_DIR}/Lib)
    set(EXECUTABLE_DIR ${INSTALL_DIR}/swig.exe)
    set(${ARGS_SWIG_DIR_OUT} "${LIBRARY_DIR}" PARENT_SCOPE)
    set(${ARGS_SWIG_EXECUTABLE_OUT} "${EXECUTABLE_DIR}" PARENT_SCOPE)
    
    # Summary
    message(STATUS "SWIG ${ARGS_SWIG_VERSION} installed to ${INSTALL_DIR}")
  endfunction()

  # Dowanlod and install nuget
  function(install_nuget)

    set(prefix ARGS)
    set(options "")
    set(one_value_args NUGET_VERSION NUGET_INSTALL_PREFIX NUGET_EXECUTABLE_OUT)
    set(multi_value_args "")

    # Parse the arguments
    cmake_parse_arguments(
      "${prefix}" 
      "${options}"
      "${one_value_args}"
      "${multi_value_args}"
      ${ARGN}
    )

    # Check if all required arguments are provided
    set(required_args ${options} ${one_value_args} ${multi_value_args})
    check_arguments("${CMAKE_CURRENT_FUNCTION}" "${prefix}" "${required_args}")

    message(STATUS "Installing NuGet ${ARGS_NUGET_VERSION}...")

    set(NUGET_DOWNLOAD_DIR ${ARGS_NUGET_INSTALL_PREFIX}/nuget)
    file(MAKE_DIRECTORY ${NUGET_DOWNLOAD_DIR})
    set(NUGET_EXECUTABLE ${NUGET_DOWNLOAD_DIR}/nuget.exe)
    if(NOT EXISTS ${NUGET_EXE})
      set(NUGET_URL "https://dist.nuget.org/win-x86-commandline/${ARGS_NUGET_VERSION}/nuget.exe")
      file(DOWNLOAD ${NUGET_URL} ${NUGET_EXECUTABLE})
    endif()

    # Set the output
    set(${ARGS_NUGET_EXECUTABLE_OUT} "${NUGET_EXECUTABLE}" PARENT_SCOPE)

    # Summary
    message(STATUS "NuGet ${ARGS_NUGET_VERSION} installed to ${NUGET_DOWNLOAD_DIR}")
  endfunction()

endif()
