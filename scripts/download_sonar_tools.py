"""
Script used to download sonar tools
"""

import requests
import zipfile
import shutil
import platform
import argparse

from pathlib import Path

SONAR_SCANNER_VERSION = "5.0.1.3006"


def download_file(url: str, save_path: Path, chunk_size=128) -> None:
    r = requests.get(url, stream=True)
    with save_path.open("wb") as fd:
        for chunk in r.iter_content(chunk_size=chunk_size):
            fd.write(chunk)


def unzip_file(zip_file_path: Path, unzip_directory: Path) -> None:
    with zipfile.ZipFile(str(zip_file_path), "r") as zip_ref:
        zip_ref.extractall(str(unzip_directory))


def get_build_wrapper(save_dir: Path) -> None:

    if platform.system() == "Windows":
        url = "https://sonarcloud.io/static/cpp/build-wrapper-win-x86.zip"
    elif platform.system() == "Linux":
        url = "https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip"
    else:
        raise Exception("Unsupported OS used.")

    save_path = save_dir / Path("build-wrapper.zip")
    download_file(url, save_path)
    unzip_file(save_path, save_dir)


def rename_sonar_scanner_folder(save_dir: Path) -> None:
    sonar_scanner_folder = next(save_dir.glob("sonar-scanner-*"))
    shutil.move(
        str(sonar_scanner_folder), str(sonar_scanner_folder.with_name("sonar-scanner"))
    )


def get_scanner(save_dir: Path, sonar_scanner_version: str) -> None:
    if platform.system() == "Windows":
        url = f"https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-{sonar_scanner_version}-windows.zip"
    elif platform.system() == "Linux":
        url = f"https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-{sonar_scanner_version}-linux.zip"
    else:
        raise Exception("Unsupported OS used.")

    save_path = save_dir / Path("download_sonar_scanner.zip")
    download_file(url, save_path)
    unzip_file(save_path, save_dir)
    rename_sonar_scanner_folder(save_dir)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-ssv",
        "--sonar_scanner_version",
        help="The sonar scanner version",
        default=f"{SONAR_SCANNER_VERSION}",
    )
    parser.add_argument(
        "--save_dir",
        type=Path,
        required=True,
        help="The sonar scanner save directory",
    )
    args = vars(parser.parse_args())

    save_dir = args["save_dir"]
    save_dir.mkdir(exist_ok=True)
    get_build_wrapper(save_dir)
    get_scanner(save_dir, args["sonar_scanner_version"])
