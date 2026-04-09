#!/usr/bin/env bash

set -e

build_dir="/workspace/build"

# ========================
# clean
# ========================
if [[ "${CLEAN_BUILD}" = "true" && -d ${build_dir} ]]; then
  print_text_box "Clean build"
  rm -fr ${build_dir}
  echo "Removed ${build_dir}"
fi

# ========================
# configure
# ========================
print_text_box "Configure build"
cmake \
  -S . \
  -B ${build_dir} \
  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
  -DCMAKE_PREFIX_PATH="${THIRD_PARTY_INSTALL_DIR}" ||
  error "[cmake] Failed to configure project"

# ========================
# build
# ========================
verbose_switch=""
if [ "${VERBOSE_BUILD}" = "true" ]; then
  verbose_switch="--verbose"
fi
print_text_box "Build"
cmake \
  --build ${build_dir} \
  --config ${BUILD_TYPE} \
  --parallel \
  ${verbose_switch} ||
  error "[cmake] Failed to build project"

# ========================
# run tests
# ========================
print_text_box "Test"
ctest --output-on-failure --extra-verbose --test-dir ${build_dir}

# ========================
# install
# ========================
print_text_box "Install"
cmake --install ${build_dir} --prefix ${build_dir}/install
