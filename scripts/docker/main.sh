#!/bin/bash

set -e

export netCDFCxx_DIR=${THIRD_PARTY_INSTALL_DIR}/netcdf_cxx4/lib64/cmake/netCDF/

build_dir="/workspace/build"
rm -fr ${build_dir}
cmake -S . -B ${build_dir} -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=${THIRD_PARTY_INSTALL_DIR}
cmake --build ${build_dir} --config Release --parallel

${build_dir}/bin/UGridApiTests
