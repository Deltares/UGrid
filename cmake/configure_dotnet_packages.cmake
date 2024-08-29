#  function that generates packages.config from package references
function(configure_dotnet_packages PACKAGES_CONFIG PACKAGE_REFERENCES DOTNET_TARGET_FRAMEWORK)
    # Create the packages.config file
    file(WRITE ${PACKAGES_CONFIG} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<packages>\n")

    # Generate the package entries for packages.config
    foreach(PACKAGE ${PACKAGE_REFERENCES})
        # Split the package name and version using the underscore
        string(REPLACE "_" ";" PACKAGE_LIST ${PACKAGE})
        list(GET PACKAGE_LIST 0 PACKAGE_ID)
        list(GET PACKAGE_LIST 1 PACKAGE_VERSION)
        # Append package entry to packages.config
        file(APPEND ${PACKAGES_CONFIG} "  <package id=\"${PACKAGE_ID}\" version=\"${PACKAGE_VERSION}\" targetFramework=\"${DOTNET_TARGET_FRAMEWORK}\" />\n")
    endforeach()

    # Close the packages tag
    file(APPEND ${PACKAGES_CONFIG} "</packages>\n")
endfunction()