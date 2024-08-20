if(WIN32)

    # Downloads and installs SWIG
    function(install_swig)
        set(options "")
        set(oneValueArgs SWIG_VERSION SWIG_INSTALL_PREFIX SWIG_DIR_OUT SWIG_EXECUTABLE_OUT)
        set(multiValueArgs "")

        cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        # Combine single-value and multi-value arguments to check
        set(required_args ${oneValueArgs} ${multiValueArgs})

        # Initialize a boolean variable to track if any arguments are missing
        set(missing_required_args FALSE)
        set(missing_args_msg "")
        # Loop to check if all required arguments are provided
        foreach(arg ${required_args})
          set(arg_name ARGS_${arg})
          if(NOT DEFINED ${arg_name})
            #message(STATUS "Warning: ${arg} argument is required but not provided.")
            string(APPEND missing_args_msg "${arg} argument is required but not provided.\n")
            set(missing_required_args TRUE)
          endif()
        endforeach()

        # If any arguments are missing, raise a fatal error
        if(missing_required_args)
            message(FATAL_ERROR "One or more required arguments are missing:\n${missing_args_msg}")
        endif()


        # Set the download URL and file paths
        set(SWIG_URL "http://prdownloads.sourceforge.net/swig/swigwin-${ARGS_SWIG_VERSION}.zip")
        set(DOWNLOAD_DIR "${ARGS_SWIG_INSTALL_PREFIX}/download")
        set(SWIG_ZIP "${DOWNLOAD_DIR}/swigwin-${ARGS_SWIG_VERSION}.zip")

        # Ensure the download and extract directories exist
        file(MAKE_DIRECTORY ${DOWNLOAD_DIR})
        file(MAKE_DIRECTORY ${ARGS_SWIG_INSTALL_PREFIX})

        # Download the SWIG zip file
        #file(TOUCH ${SWIG_ZIP})
        if(NOT EXISTS ${SWIG_ZIP})
          file(DOWNLOAD ${SWIG_URL} ${SWIG_ZIP} SHOW_PROGRESS)
        endif()

        # Unzip the downloaded file
        file(ARCHIVE_EXTRACT INPUT ${SWIG_ZIP} DESTINATION ${ARGS_SWIG_INSTALL_PREFIX})

        # Remove the zip file after extraction
        file(REMOVE ${SWIG_ZIP})

        set(INSTALL_DIR ${ARGS_SWIG_INSTALL_PREFIX}/swigwin-${ARGS_SWIG_VERSION})
        set(LIBRARY_DIR ${INSTALL_DIR}/Lib)
        set(EXECUTABLE_DIR ${INSTALL_DIR}/swig.exe)
        set(${ARGS_SWIG_DIR_OUT} "${LIBRARY_DIR}" PARENT_SCOPE)
        set(${ARGS_SWIG_EXECUTABLE_OUT} "${EXECUTABLE_DIR}" PARENT_SCOPE)
        
        message(STATUS "SWIG ${ARGS_SWIG_VERSION} downloaded and extracted to: ${INSTALL_DIR}")
    endfunction()

endif()
