#!/usr/bin/python3
"""
Script to harvest and restore dlls for signing purposes.
"""

__author__ = "Maarten Tegelaers"
__copyright__ = "Copyright (C) Stichting Deltares, 2024"
__version__ = "0.1.1"
__maintainer__ = "Ahmad El Sayed"
__email__ = "ahmad.elsayed@deltares.nl"
__status__ = "Development"


from pathlib import Path
from typing import List
import shutil
import argparse
import json


def get_paths_of_dlls_to_sign(build_dir: Path) -> List[Path]:
    """
    Gather all the dll names which need to be signed.

    It is assumed that the only dlls produced by the solution are the ones created
    by their corresponding .vcxproj and .csproj projects in Release mode.

    Args:
        build_dir (Path): Path to the build dir

    Returns:
        List[Path]: List of dll paths to sign.
    """
    vs_proj_types = ("vcxproj", "csproj")
    vs_proj_paths = []
    for vs_proj_type in vs_proj_types:
        vs_proj_paths.extend(Path(f"{build_dir}/libs").glob(f"*/*.{vs_proj_type}"))
        vs_proj_paths.extend(Path(f"{build_dir}/libs").glob(f"*/*/*.{vs_proj_type}"))

    dll_paths = []
    for vs_proj_path in vs_proj_paths:
        parent = vs_proj_path.parent
        stem = vs_proj_path.stem
        dll_path = Path(f"{parent}/Release/{stem}.dll")
        if dll_path.exists() and "test" not in stem.lower():
            dll_paths.append(dll_path)

    return dll_paths


def generate_json_mapping_content(
    build_dir: Path,
    dll_paths: List[Path],
) -> dict:
    """
    Generate the json mapping file, with which the signed dlls can be restored to their original paths.

    Args:
        build_dir (Path): Path to the build dir
        dll_paths (List[Path]): List of dll paths to sign.

    Returns:
        dict: Dict describing the mapping
    """
    return {
        "mapping": list(
            (
                {
                    "dll": dll_path.name,
                    "path": str(dll_path.relative_to(build_dir)),
                }
                for dll_path in dll_paths
            )
        )
    }


DLL_MAPPING_FILE_NAME = "dll_mapping.json"


def write_json_mapping_file(
    sign_dir: Path,
    content: dict,
) -> None:
    """
    Write the specified content to the mapping file within sign_dir.

    Args:
        sign_dir (Path): Path to the directory containing the dlls to sign.
        content (dict): dictionary describing the dll mapping.
    """
    json_mapping_file = sign_dir / Path(DLL_MAPPING_FILE_NAME)
    with json_mapping_file.open("w") as f:
        json.dump(content, f, indent=2)


def read_json_mapping_file(sign_dir: Path) -> dict:
    """
    Load the dll mapping file from the sign_dir.
    Args:
        sign_dir (Path): Path to the directory containing the signed dlls.
    Returns:
        dict: A dictionary containing the mapping of the dlls.
    """
    json_mapping_file = sign_dir / Path(DLL_MAPPING_FILE_NAME)
    with json_mapping_file.open("r") as f:
        return json.load(f)


def harvest_to_sign_dlls(
    dll_paths: List[Path],
    sign_dir: Path,
) -> None:
    """
    Move the dlls specified by dll_paths to the sign_dir.

    Args:
        dll_paths: The dlls to copy
        sign_dir (Path): Path to the directory containing the dlls to sign.
    """
    print("DLLs to sign:")
    for dll_path in dll_paths:
        print(" -", dll_path)
        shutil.move(str(dll_path), str(sign_dir))


def restore_signed_dlls(build_dir: Path, dll_mapping: dict, sign_dir) -> None:
    """
    Restore the signed dlls to their original locations.
    Args:
        build_dir (Path): The path to the build dir.
        dll_mapping (dict): Dictionary describing the mapping of dlls
        sign_dir (Path): Path to the directory containing the signed dlls.
    """
    print("DLLs to restore:")
    for dll_info in dll_mapping["mapping"]:
        dll = str(sign_dir / dll_info["dll"])
        destination = str(build_dir / Path(dll_info["path"]))
        print(dll, "->", destination)
        shutil.move(dll, destination)


TO_SIGN_DIR_NAME = "to_sign"


def harvest(build_dir: Path) -> None:
    """
    Create a to_sign folder containing all the dlls to sign, and their corresponding mapping

    Args:
        build_dir: Path to the build dir.
    """

    dll_paths = get_paths_of_dlls_to_sign(build_dir)

    sign_dir = build_dir / Path(TO_SIGN_DIR_NAME)
    if not (sign_dir.exists() and sign_dir.is_dir()):
        sign_dir.mkdir(parents=True)

    dll_mapping = generate_json_mapping_content(build_dir, dll_paths)
    write_json_mapping_file(sign_dir, dll_mapping)

    harvest_to_sign_dlls(dll_paths, sign_dir)


def restore(build_dir: Path) -> None:
    """
    Restore the signed dlls to their original locations.
    Args:
        build_dir: Path to the build dir.
    """
    sign_dir = build_dir / Path(TO_SIGN_DIR_NAME)
    dll_mapping = read_json_mapping_file(sign_dir)

    restore_signed_dlls(build_dir, dll_mapping, sign_dir)


def parse_arguments():
    """
    Parse the arguments with which this script was called through
    argparse.ArgumentParser
    """
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--build_dir",
        type=Path,
        required=True,
        help="Path to the root of the repository.",
    )
    action_group = parser.add_mutually_exclusive_group(required=True)
    action_group.add_argument(
        "--harvest",
        action="store_true",
    )
    action_group.add_argument(
        "--restore",
        action="store_false",
    )
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_arguments()
    build_dir = Path(args.build_dir)
    if args.harvest:
        harvest(build_dir)
    else:
        restore(build_dir)
