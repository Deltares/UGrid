#!/usr/bin/env bash

set -e

export netCDFCxx_DIR=${THIRD_PARTY_INSTALL_DIR}/netcdf_cxx4/lib64/cmake/netCDF/
export ZLIB_ROOT=${THIRD_PARTY_INSTALL_DIR}/zlib/

source /workspace/scripts/docker/error.sh
source /workspace/scripts/docker/utilities.sh
source /workspace/scripts/docker/build_and_test.sh
if [[ "${RUN_SONAR_SCANNER}" = "true" ]]; then
  source /workspace/scripts/docker/sonar_scanner.sh
fi
