#!/usr/bin/env bash

set -e

source error.sh

dnf update -y

dev_toolset="gcc-toolset-12"

packages=(
  "git"
  "cmake"
  "boost-devel"
  "m4"
  "perl"
  "openssl"
  "libcurl-devel"
  ${dev_toolset}
  "doxygen"
)

for package in "${packages[@]}"; do
  dnf install -y "${package}" || error "[dnf] Failed to install ${package}"
done

scl enable "${dev_toolset}" bash || error "[scl] Failed to enable ${dev_toolset}"

dnf clean all
