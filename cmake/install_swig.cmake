if(WIN32)

    # Downloads and installs SWIG
    function(install_swig)
        cmake_parse_arguments(
            INSTALL_SWIG_ARGS
            ""
            "VERSION;INSTALL_DIR"
            ""
            ${ARGN}
        )

        # Check if all required arguments are provided
        if(NOT INSTALL_SWIG_ARGS_VERSION)
            message(FATAL_ERROR "VERSION argument is required.")
        endif()
        if(NOT INSTALL_SWIG_ARGS_INSTALL_DIR)
            message(FATAL_ERROR "INSTALL_DIR argument is required.")
        endif()

        # Set the download URL and file paths
        set(SWIG_URL "http://prdownloads.sourceforge.net/swig/swigwin-${INSTALL_SWIG_ARGS_VERSION}.zip")
        set(DOWNLOAD_DIR "${INSTALL_SWIG_ARGS_INSTALL_DIR}/download")
        set(SWIG_ZIP "${DOWNLOAD_DIR}/swigwin-${INSTALL_SWIG_ARGS_VERSION}.zip")

        # Ensure the download and extract directories exist
        file(MAKE_DIRECTORY ${DOWNLOAD_DIR})
        file(MAKE_DIRECTORY ${INSTALL_SWIG_ARGS_INSTALL_DIR})

        # Download the SWIG zip file
        #file(TOUCH ${SWIG_ZIP})
        if(NOT EXISTS ${SWIG_ZIP})
          file(DOWNLOAD ${SWIG_URL} ${SWIG_ZIP} SHOW_PROGRESS)
        endif()

        # Unzip the downloaded file
        file(ARCHIVE_EXTRACT INPUT ${SWIG_ZIP} DESTINATION ${INSTALL_SWIG_ARGS_INSTALL_DIR})

        # Remove the zip file after extraction
        file(REMOVE ${SWIG_ZIP})

        message(STATUS "SWIG ${INSTALL_SWIG_ARGS_VERSION} downloaded and extracted to: ${INSTALL_SWIG_ARGS_INSTALL_DIR}")
    endfunction()

endif()
