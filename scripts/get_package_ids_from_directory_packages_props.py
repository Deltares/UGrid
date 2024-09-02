import sys
import xml.etree.ElementTree as ET


def parse_xml(file_path):
    # Parse the XML file
    tree = ET.parse(file_path)
    root = tree.getroot()

    # Initialize a list to store Include attribute values
    include_values = []

    # Find all PackageVersion elements and get their Include attributes
    for package in root.findall(".//PackageVersion"):
        include_attr = package.get("Include")
        if include_attr:
            include_values.append(include_attr + "_")

    # Join the values into a comma-separated string for easy output
    print(";".join(include_values))


if __name__ == "__main__":
    # The file path is passed as the first command line argument
    if len(sys.argv) != 2:
        print(
            "Usage: get_package_ids_from_directory_packages_prop.py <path_to_xml_file>"
        )
        sys.exit(1)

    file_path = sys.argv[1]
    parse_xml(file_path)
