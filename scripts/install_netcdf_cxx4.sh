#!/usr/bin/env bash

set -e

# defaults of optional arguments
declare -g RUN_TESTS=false
declare -g REINSTALL=false
declare -g CLEAN=false
declare -g BUILD_TYPE="Release"
declare -g BUILD_TESTS="OFF"

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
      BUILD_TYPE="$2"
      shift 2
      ;;
    --run_tests)
      RUN_TESTS=true
      BUILD_TESTS="ON"
      shift
      ;;
    --reinstall)
      REINSTALL=true
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
  if ${REINSTALL}; then
    rm -rf "${WORK_DIR}"
    rm -rf "${INSTALL_DIR}"
  fi

  mkdir -p ${WORK_DIR}
  WORK_DIR=$(realpath "${WORK_DIR}")

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
  local lib_name="$1"
  local git_url="$2"
  local git_tag="$3"
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
    ctest --test-dir ${build_dir} --build-config ${BUILD_TYPE} --rerun-failed --output-on-failure --verbose || true
  fi

  # install
  cmake --install ${build_dir} --config ${BUILD_TYPE}
}

function get_shared_lib_extension() {
  local os_type=$(uname)
  case "${os_type}" in
  Darwin)
    echo "dylib"
    ;;
  Linux)
    echo "so"
    ;;
  MINGW* | MSYS* | MYSYS*)
    echo "dll"
    ;;
  *)
    echo "Unsupported OS: ${os_type}"
    ;;
  esac
}

function get_zlib_path() {
  local zlib_root="$1"
  echo $(find $(realpath "${zlib_root}/lib") -name *.$(get_shared_lib_extension))
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
  zlib_root="${INSTALL_DIR}/zlib"
  zlib_include_dir="${zlib_root}/include"
  zlib_library=$(get_zlib_path "${zlib_root}")
  hdf5_root="${INSTALL_DIR}/hdf5"
  install "hdf5" \
    "https://github.com/HDFGroup/hdf5.git" \
    "hdf5-1_10_11" \
    "-DBUILD_STATIC_LIBS:BOOL=OFF \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    -DHDF5_BUILD_TOOLS:BOOL=OFF \
    -DHDF5_BUILD_EXAMPLES:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${BUILD_TESTS} \
    -DZLIB_USE_EXTERNAL:BOOL=OFF \
    -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=ON \
    -DZLIB_DIR=${zlib_root} \
    -DZLIB_ROOT=${zlib_root} \
    -DZLIB_INCLUDE_DIR:PATH=${zlib_include_dir} \
    -DZLIB_LIBRARY:FILEPATH=${zlib_library} \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR}"

  # netcdf 4.8.1
  install "netcdf" \
    "https://github.com/Unidata/netcdf-c.git" \
    "v4.8.1" \
    "-DBUILD_SHARED_LIBS:BOOL=ON \
    -DENABLE_TESTS:BOOL=${BUILD_TESTS} \
    -DENABLE_NETCDF_4:BOOL=ON \
    -DENABLE_DAP:BOOL=OFF \
    -DENABLE_BYTERANG:BOOL=OFF \
    -DZLIB_DIR=${zlib_root} \
    -DZLIB_ROOT=${zlib_root} \
    -DZLIB_INCLUDE_DIR:PATH=${zlib_include_dir} \
    -DZLIB_LIBRARY:FILEPATH=${zlib_library} \
    -DHDF5_ROOT=${hdf5_root} \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR}"

  local os_type=$(uname)
  case "${os_type}" in
  Darwin | Linux)
    install "netcdf_cxx4" \
      "https://github.com/Unidata/netcdf-cxx4.git" \
      "dev" \
      "-DNCXX_ENABLE_TESTS:BOOL=${BUILD_TESTS} \
      -DCMAKE_PREFIX_PATH=${INSTALL_DIR}"
    ;;
  MINGW* | MSYS* | MYSYS*)
    local HDF5_C_LIBRARIES="${hdf5_root}/lib/hdf5.lib"
    local HDF5_INCLUDE_DIRS="${hdf5_root}/include"
    COMMON_CONFIG_OPTS="-DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
      -DNCXX_ENABLE_TESTS:BOOL=${BUILD_TESTS} \
      -DHDF5_C_LIBRARIES=${HDF5_C_LIBRARIES} \
      -DHDF5_INCLUDE_DIRS=${HDF5_INCLUDE_DIRS} \
      -DHDF5_C_LIBRARY_hdf5=${HDF5_C_LIBRARIES}"
    local netcdf_cxx_name="netcdf_cxx4"
    local necdf_cxx_git_tag="dev"
    local netcdf_cxx_repo="https://github.com/Unidata/netcdf-cxx4.git"
    # Shared/static lib build is controlled by BUILD_SHARED_LIBS.
    # BUILD_STATIC_LIBS does not exist. To install both, first runwith
    # BUILD_SHARED_LIBS:BOOL=ON (shared)
    # then with
    # BUILD_SHARED_LIBS:BOOL=OFF (static)
    install ${netcdf_cxx_name} \
      ${netcdf_cxx_repo} \
      ${necdf_cxx_git_tag} \
      "-DBUILD_SHARED_LIBS:BOOL=OFF ${COMMON_CONFIG_OPTS}"
    install ${netcdf_cxx_name} \
      ${netcdf_cxx_repo} \
      ${necdf_cxx_git_tag} \
      "-DBUILD_SHARED_LIBS:BOOL=ON ${COMMON_CONFIG_OPTS}"
    ;;
  *)
    echo "Unsupported OS: ${os_type}"
    ;;
  esac
}

function main() {
  parse_args "$@"
  validate_args
  create_directories
  install_all
  clean
}

main "$@"
