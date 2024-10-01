#!/usr/bin/env bash

set -e

export netCDFCxx_DIR=${THIRD_PARTY_INSTALL_DIR}/netcdf_cxx4/lib64/cmake/netCDF/

build_dir="/workspace/build"

if [[ "${CLEAN_BUILD}" = "true" && -d ${build_dir} ]]; then
  print_text_box "Clean build"
  # cmake --build ${build_dir} --target clean ||
  #   error "[cmake] Failed to clean project"
  rm -fr ${build_dir}
fi

# configure
print_text_box "Configure build"
cmake \
  -S . \
  -B ${build_dir} \
  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
  -DCMAKE_PREFIX_PATH="${THIRD_PARTY_INSTALL_DIR}" ||
  error "[cmake] Failed to configure project"

verbose_switch=""
if [ "${VERBOSE_BUILD}" = "true" ]; then
  verbose_switch="--verbose"
fi

# build
print_text_box "Build"
cmake \
  --build ${build_dir} \
  --config ${BUILD_TYPE} \
  --parallel \
  ${verbose_switch} ||
  error "[cmake] Failed to build project"

# run tests
print_text_box "Test"
ctest --output-on-failure --extra-verbose --test-dir ${build_dir}

# install
print_text_box "Install"
cmake --install ${build_dir} --prefix ${build_dir}/install
