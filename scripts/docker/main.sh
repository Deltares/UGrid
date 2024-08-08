#!/usr/local/bin/bash

set -e

source /workspace/scripts/docker/error.sh

export netCDFCxx_DIR=${THIRD_PARTY_INSTALL_DIR}/netcdf_cxx4/lib64/cmake/netCDF/

build_dir="/workspace/build"
if ${CLEAN_BUILD}; then
  rm -fr ${build_dir}
fi

cmake -S . -B ${build_dir} -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="${THIRD_PARTY_INSTALL_DIR}" ||
  error "[cmake] Failed to configure project"
cmake --build ${build_dir} --config Release --parallel ||
  error "[cmake] Failed to build project"

${build_dir}/bin/UGridApiTests
