#!/usr/bin/env bash

set -e

source error.sh

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
