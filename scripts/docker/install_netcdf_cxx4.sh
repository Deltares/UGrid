#!/usr/bin/env bash

set -e

# defaults of optional arguments
declare -g RUN_TESTS=false
declare -g CLEAN=false
declare -g BUILD_TYPE="Release"

function usage() {
  echo "Usage: $0\
 <--work_dir WORK_DIR>\
 <--install_dir INSTALL_DIR>\
 [--build_type BUILD_TYPE={Release | Debug | RelWithDebInfo}]\
 [--run_tests]\
 [--clean]"
}

function parse_args() {
  while [[ $# -gt 0 ]]; do
    key="$1"
    case ${key} in
    --help)
      usage
      exit 0
      ;;
    --work_dir)
      declare -g WORK_DIR="$2"
      shift 2
      ;;
    --install_dir)
      declare -g INSTALL_DIR="$2"
      shift 2
      ;;
    --build_type)
      BUILD_TYPE=$"$2"
      shift 2
      ;;
    --run_tests)
      RUN_TESTS=true
      shift
      ;;
    --clean)
      CLEAN=true
      shift
      ;;
    *)
      echo "Unknown argument: ${key}"
      usage
      exit 1
      ;;
    esac
  done
}

function validate_args() {
  # Check if all mandatory arguments are provided
  if [ -z "${WORK_DIR}" ] || [ -z "${INSTALL_DIR}" ]; then
    usage
    exit 1
  fi

  # if provided the release type can be one of Release, Debug, or RelWithDebInfo
  local valid_build_types=("Release" "Debug" "RelWithDebInfo")
  local build_type_is_valid=false
  for valid_build_type in "${valid_build_types[@]}"; do
    if [[ "${BUILD_TYPE}" == "${valid_build_type}" ]]; then
      build_type_is_valid=true
      break
    fi
  done

  if ! ${build_type_is_valid}; then
    echo "${BUILD_TYPE} is an invalid build type."
    printf "Valid build types: { %s }\n" "$(
      IFS=\|
      echo "${valid_build_types[*]}"
    )"
    exit 1
  fi
}

# Creates work directory
function create_directories() {
  # if [ -d "${WORK_DIR}" ]; then
  #   rm -rf "${WORK_DIR}"
  # fi
  # mkdir ${WORK_DIR}
  mkdir -p ${WORK_DIR}
  WORK_DIR=$(realpath "${WORK_DIR}")

  # if [ -d "${INSTALL_DIR}" ]; then
  #   rm -rf "${INSTALL_DIR}"
  # fi
  # mkdir ${INSTALL_DIR}
  mkdir -p ${INSTALL_DIR}
  INSTALL_DIR=$(realpath "${INSTALL_DIR}")
}

# Removes work directory
clean() {
  if ${CLEAN}; then
    rm -rf "${WORK_DIR}"
  fi
}

function install() {
  # parameters
  local lib_name=$1
  local git_url=$2
  local git_tag=$3
  local cmake_config_options="${4:-}"

  # create build dir
  local lib_work_dir=${WORK_DIR}/${lib_name}
  mkdir -p ${lib_work_dir}

  # clone and checkout tag
  local git_repo=${lib_work_dir}/repo
  if [ ! -d ${git_repo} ]; then
    git clone ${git_url} ${git_repo}
    if [ "${git_tag}" != "dev" ]; then
      git -C ${git_repo} fetch --all --tags
      git -C ${git_repo} checkout tags/${git_tag} -b ${git_tag}
    fi
  fi

  # configure the build
  local build_dir=${lib_work_dir}/build
  local install_dir=${INSTALL_DIR}/${lib_name}
  cmake \
    -S ${git_repo} \
    -B ${build_dir} \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    ${cmake_config_options} \
    --install-prefix ${install_dir}

  # build
  cmake --build ${build_dir} --config ${BUILD_TYPE} --parallel

  # test
  if ${RUN_TESTS}; then
    ctest --test-dir ${build_dir}
  fi

  # install
  cmake --install ${build_dir}
}

function install_all() {
  # zlib v1.2.1
  install \
    "zlib" \
    "https://github.com/madler/zlib.git" \
    "v1.2.13"

  # # curl curl-8_9_1
  # install \
  #   "curl" \
  #   "https://github.com/curl/curl.git" \
  #   "curl-8_9_1" \
  #   "-DCURL_ENABLE_SSL:BOOL=ON \
  #    -DCMAKE_PREFIX_PATH=${INSTALL_DIR}/zlib"

  # hdf5 1.10.11
  install "hdf5" \
    "https://github.com/HDFGroup/hdf5.git" \
    "hdf5-1_10_11" \
    "-DBUILD_STATIC_LIBS:BOOL=OFF \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    -DHDF5_BUILD_TOOLS:BOOL=OFF \
    -DHDF5_BUILD_EXAMPLES:BOOL=OFF \
    -DBUILD_TESTING:BOOL=ON \
    -DZLIB_USE_EXTERNAL:BOOL=OFF \
    -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=ON \
    -DZLIB_ROOT=${INSTALL_DIR}/zlib \
    -DZLIB_INCLUDE_DIR:PATH=${INSTALL_DIR}/zlib/include \
    -DZLIB_LIBRARY:FILEPATH=${INSTALL_DIR}/zlib/lib/libz.so \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR}"

  # netcdf 4.8.1
  install "netcdf" \
    "https://github.com/Unidata/netcdf-c.git" \
    "v4.8.1" \
    "-DBUILD_SHARED_LIBS:BOOL=ON \
    -DENABLE_NETCDF_4:BOOL=ON \
    -DENABLE_DAP:BOOL=OFF \
    -DENABLE_BYTERANG:BOOL=OFF \
    -DZLIB_ROOT=${INSTALL_DIR}/zlib \
    -DHDF5_ROOT=${INSTALL_DIR}/hdf5 \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR}"

  # netcdf-cxx4 v4.3.2-development
  install "netcdf_cxx4" \
    "https://github.com/Unidata/netcdf-cxx4.git" \
    "dev" \
    "-DCMAKE_PREFIX_PATH=${INSTALL_DIR}"
}

function main() {
  parse_args "$@"
  validate_args
  create_directories
  install_all
  clean
}

main "$@"
