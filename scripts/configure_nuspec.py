import argparse
import os
import sys
import xml.etree.ElementTree as ET
from pathlib import Path
from xml.dom import minidom


def parse_args():
    """
    Parse the arguments with which this script is called
    """
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--template",
        type=Path,
        required=True,
        help="Path to the nuspec template",
    )

    parser.add_argument(
        "--destination",
        type=str,
        required=True,
        help="Path where the nuspec config is to be written",
    )

    parser.add_argument(
        "--dotnet_target_frameworks",
        type=str,
        nargs="+",
        required=True,
        help=".NET target framework",
    )

    args = parser.parse_args()
    return args


def prettify_xml(xml_string: str):
    # Parse the XML string
    dom = minidom.parseString(xml_string)
    # Pretty print the XML
    pretty_xml = dom.toprettyxml(indent="  ")
    # Remove empty lines
    pretty_xml = "\n".join(line for line in pretty_xml.split("\n") if line.strip())
    return pretty_xml


def configure_nuspec(
    nuspec_template_path: Path,
    output_path: Path,
    frameworks: list[str],
) -> None:
    # Parse the template file into an ElementTree object
    tree = ET.parse(nuspec_template_path)

    # Get root element
    root = tree.getroot()

    # Extract the namespace from the root element
    ns_attr = root.tag.split("}")[0][1:]

    # Register the namespace for later XML operations
    ET.register_namespace("", ns_attr)

    # Build a namespace-aware query for finding elements
    ns = {"ns": ns_attr}
    # Find the metadata/dependencies section
    metadata = root.find("ns:metadata", ns)
    if metadata is None:
        raise ValueError("Could not find <metadata> section in the nuspec file.")
    dependencies = metadata.find("ns:dependencies", ns)
    if dependencies is None:
        raise ValueError(
            "Could not find <dependencies> section in the <metadata> section."
        )

    # Add dependencies based on the frameworks
    for framework in frameworks:
        group = ET.Element("group", targetFramework=framework)
        dependencies.append(group)

    # Find the files section
    files = root.find("ns:files", ns)
    if files is None:
        raise ValueError("Could not find <files> section in the nuspec file.")
    # Create the <file> elements for the DLL
    for framework in frameworks:
        file_dll = ET.Element(
            "file",
            src=f"@CMAKE_BINARY_DIR@/libs/UGridNET/dll/Release/{framework}/UGridNET.dll",
            target=f"lib/{framework}",
        )
        files.append(file_dll)

    # Create the <file> elements for the targets
    for framework in frameworks:
        file_targets = ET.Element(
            "file",
            src=f"@CMAKE_BINARY_DIR@/libs/UGridNET/nuget/Deltares.UGridNET.targets",
            target=f"build/{framework}",
        )
        files.append(file_targets)

    # Create the <file> elements for the runtimes
    extensions = ["dll", "lib", "exp"]
    for extension in extensions:
        file_runtimes = ET.Element(
            "file",
            src=f"@CMAKE_BINARY_DIR@/libs/UGridNET/SWIG/Release/UGridCSharpWrapper.{extension}",
            target="runtimes/win-x64/native",
        )
        files.append(file_runtimes)

    # Create the <file> element for the readme
    file_readme = ET.Element(
        "file",
        src="@CMAKE_BINARY_DIR@/libs/UGridNET/nuget/README.md",
        target="",
    )
    files.append(file_readme)

    # Convert the ElementTree to a string first
    xml_str = ET.tostring(root, "utf-8")

    # Write the formatted XML to the output file
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(prettify_xml(xml_str))

    # print(f"Updated nuspec file saved to: {output_path}")


if __name__ == "__main__":
    # parse the args
    args = parse_args()
    # write the configuration
    configure_nuspec(
        args.template,
        args.destination,
        args.dotnet_target_frameworks,
    )
