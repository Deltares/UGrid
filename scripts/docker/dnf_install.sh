#!/usr/bin/env bash

set -e

source /workspace/scripts/docker/error.sh
source /workspace/scripts/docker/utilities.sh

dnf install -y epel-release
dnf config-manager --enable epel
dnf install -y dnf-utils
dnf config-manager --set-enabled powertools
dnf update -y

dev_toolset="gcc-toolset-12"

packages=(
  "git"
  "cmake"
  "boost1.78-devel.x86_64"
  "m4"
  "perl"
  "openssl-devel"
  "libcurl-devel"
  ${dev_toolset}
  "doxygen"
  "python311"
)

for package in "${packages[@]}"; do
  col_echo --blue "Installing ${package}..."
  dnf install -y "${package}" || error "[dnf] Failed to install ${package}"
done

dnf clean all

# enable the gcc toolset
scl enable "${dev_toolset}" bash || error "[scl] Failed to enable ${dev_toolset}"

# make pip available and install requirements
python3 -m ensurepip
python3 -m pip install -r /workspace/scripts/docker/requirements.txt
