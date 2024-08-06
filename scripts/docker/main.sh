#!/bin/bash

set -e

third_party_dir="/workspace/third_party"
mkdir -p ${third_party_dir}

chmod +x /workspace/scripts/install_netcdf_cxx4.sh

/workspace/scripts/install_netcdf_cxx4.sh \
  --work_dir ${third_party_dir}/work \
  --install_dir ${third_party_dir}/install

export netCDFCxx_DIR=${third_party_dir}/install/netcdf_cxx4/lib64/cmake/netCDF/

build_dir="/workspace/build"
rm -fr ${build_dir}
cmake -S . -B ${build_dir} -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=${third_party_dir}/install
cmake --build ${build_dir} --config Release --parallel

${build_dir}/bin/UGridApiTests
