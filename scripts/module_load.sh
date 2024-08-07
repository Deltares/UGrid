# Script that is meant to be sourced in the CI
# It includes the necessary module loads to build MeshKernel on Deltares Linux machines

module --verbose load gcc/12.2.0_gcc12.2.0
module --verbose load cmake/3.26.4_gcc12.2.0
module --verbose load boost/1.83.0_gcc12.2.0
module --verbose load hdf5/1.10.11_gcc12.2.0
module --verbose load netcdf/4.8.1_gcc12.2.0
module --verbose load netcdf-cxx/4.3.2_gcc12.2.0
module --verbose load curl/8.9.1_gcc12.2.0
