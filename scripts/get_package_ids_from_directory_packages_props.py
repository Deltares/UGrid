import sys
import xml.etree.ElementTree as ET
from pathlib import Path


def parse_xml(file_path: Path) -> str:
    """
    Finds all the PackageVersion elements and joins the underscore-suffixed Include values into a comma-separated string
    reason behind underscore is related to how set_target_properties sets the property VS_PACKAGE_REFERENCES
    see https://cmake.org/cmake/help/latest/prop_tgt/VS_PACKAGE_REFERENCES.html
    set_target_properties( <some_csharp_target> PROPERTIES VS_PACKAGE_REFERENCES "package1_x.y.z;package2_x.y.z;...")
    sets the following in a csproj
    #
    <ItemGroup>
       <PackageReference Include="package1" Version="x.y.z" />
       <PackageReference Include="package2" Version="x.y.z" />
    <PackageReference Include="NUnit3TestAdapter" Version="" />
    </ItemGroup>
    #
    the underscore in between separates the package's name from its version.
    #
    The packages are defined in Directory.Packages.props as follows:
    #
    <Project>
    <PropertyGroup>
        <ManagePackageVersionsCentrally>true</ManagePackageVersionsCentrally>
    </PropertyGroup>
    <ItemGroup>
        <PackageVersion Include="package1" Version="x.y.z" />
        <PackageVersion Include="package2" Version="x.y.z" />
         .
         .
         .
    </ItemGroup>
    </Project>
    #
    but when central package management is used (ManagePackageVersionsCentrally set to true).
    it is not allowed to set the version in the csproj PackageReference anymore. So we leave out the version.
    This results in
    #
    <ItemGroup>
       <PackageReference Include="package1" Version="" />
       <PackageReference Include="package2" Version="" />
    <PackageReference Include="NUnit3TestAdapter" Version="" />
    </ItemGroup>
    #
    Visual Studio does not complain about the attribute "Version" having no value.

    Args:
    - file_path (Path): path to Directory.Packages.props file.

    Returns:
    - str: Underscore-suffixed semicolon-delimited package references.
    """

    # Parse the XML file
    tree = ET.parse(file_path)
    root = tree.getroot()

    # Initialize a list to store Include attribute values
    include_values = []

    for package in root.findall(".//PackageVersion"):
        include_attr = package.get("Include")
        if include_attr:
            include_values.append(include_attr + "_")

    return ";".join(include_values)


if __name__ == "__main__":
    # The file path is passed as the first command line argument
    if len(sys.argv) != 2:
        print(
            "Usage: python get_package_ids_from_directory_packages_prop.py <path_to_directory_packages_props>"
        )
        sys.exit(1)

    file_path = sys.argv[1]
    result = parse_xml(file_path)
    print(result)
