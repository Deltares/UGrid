import xml.etree.ElementTree as ET
import sys
from pathlib import Path

# CMake sets AppendTargetFrameworkToOutputPath to false if there a single target framework.
# The dlls are therefore written to:
# ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ome_app.dll
# When multiple frameworks are targeted, this property is not set and therefore defaults to true.
# ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/<TARGET_FRAMEWORK>/ome_app.dll

def append_target_framework_to_output_path(file_path: Path) -> None:
    # Parse the cs project
    tree = ET.parse(file_path)
    root = tree.getroot()

    # Find the AppendTargetFrameworkToOutputPath property and set iot to true
    for prop in root.findall(".//PropertyGroup/AppendTargetFrameworkToOutputPath"):
        prop.text = 'true'
    
    # Save changes back to the file, will likely mess up the original formatting 
    tree.write(file_path, encoding="utf-8", method="xml", xml_declaration=False)

if __name__ == "__main__":
    file_path = Path(sys.argv[1])
    append_target_framework_to_output_path(file_path)
