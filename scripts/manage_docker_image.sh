#!/usr/bin/env bash
set -e

function usage() {
  echo "Usage: "
  echo "$0 \\"
  echo "  --repo_path <REPO_PATH> \\"
  echo "  --harbor_username <HARBOR_USERNAME> \\"
  echo "  --harbor_password <HARBOR_PASSWORD> \\"
  echo "  --server_address <SERVER_ADDRESS> \\"
  echo "  --project_path <PROJECT_PATH> \\"
  echo "  --docker_image_name <DOCKER_IMAGE_NAME> \\"
  echo "  --docker_image_tag <DOCKER_IMAGE_TAG> \\"
  echo "  --docker_file_name <DOCKER_FILE_NAME> \\"
  echo "  --files_to_check <FILE_1> <FILE_2>... <FILE_N>"
  echo "or"
  echo "$0 --json <JSON_CONFIG>"
  echo "JSON configutaion template:"
  echo "{"
  echo "  \"repo_path\": \"<REPO_PATH>\","
  echo "  \"harbor_username\": \"<HARBOR_USERNAME>\","
  echo "  \"harbor_password\": \"<HARBOR_PASSWORD>\","
  echo "  \"server_address\": \"<SERVER_ADDRESS>\","
  echo "  \"project_path\": \"<PROJECT_PATH>\","
  echo "  \"docker_image_name\": \"<DOCKER_IMAGE_NAME>\","
  echo "  \"docker_image_tag\": \"<DOCKER_IMAGE_TAG>\","
  echo "  \"docker_file_name\": \"<DOCKER_FILE_NAME>\","
  echo "  \"files_to_check\": ["
  echo "    \"<FILE_1>\","
  echo "    \"<FILE_2>\","
  echo "    ."
  echo "    ."
  echo "    ."
  echo "    \"<FILE_N>\","
  echo "  ]"
  echo "}"
}

function load_json_config() {
  local json_config="$1"
  declare -g REPO_PATH=$(jq -r '.repo_path' "${json_config}")
  declare -g HARBOR_USERNAME=$(jq -r '.harbor_username' "${json_config}")
  declare -g HARBOR_PASSWORD=$(jq -r '.harbor_password' "${json_config}")
  declare -g SERVER_ADDRESS=$(jq -r '.server_address' "${json_config}")
  declare -g PROJECT_PATH=$(jq -r '.project_path' "${json_config}")
  declare -g DOCKER_IMAGE_NAME=$(jq -r '.docker_image_name' "${json_config}")
  declare -g DOCKER_IMAGE_TAG=$(jq -r '.docker_image_tag' "${json_config}")
  declare -g DOCKER_FILE_NAME=$(jq -r '.docker_file_name' "${json_config}")
  mapfile -t FILES_TO_CHECK < <(jq -r '.files_to_check[]' "${json_config}")
}

function parse_args() {
  local do_exit=false

  # store the args to allow resetting their position in case a json config is not provided
  local args=("$@")

  local json_config=""
  while [[ "$#" -gt 0 ]]; do
    case "$1" in
    --json)
      json_config=$(realpath "$2")
      echo "json_config: ${json_config}"
      load_json_config "${json_config}"
      shift 2
      break
      ;;
    *)
      shift
      ;;
    esac
  done

  if [[ -z "${json_config}" ]]; then
    set -- "${args[@]}"
    while [[ $# -gt 0 ]]; do
      key="$1"
      case ${key} in
      --help)
        usage
        exit 0
        ;;
      --repo_path)
        declare -g REPO_PATH=$(realpath "$2")
        shift 2
        ;;
      --harbor_username)
        declare -g HARBOR_USERNAME="$2"
        shift 2
        ;;
      --harbor_password)
        declare -g HARBOR_PASSWORD="$2"
        shift 2
        ;;
      --server_address)
        declare -g SERVER_ADDRESS="$2"
        shift 2
        ;;
      --project_path)
        declare -g PROJECT_PATH="$2"
        shift 2
        ;;
      --docker_image_name)
        declare -g DOCKER_IMAGE_NAME="$2"
        shift 2
        ;;
      --docker_image_tag)
        declare -g DOCKER_IMAGE_TAG="$2"
        shift 2
        ;;
      --docker_file_name)
        declare -g DOCKER_FILE_NAME=$(realpath "$2")
        shift 2
        ;;
      --files_to_check)
        shift 1
        declare -ag FILES_TO_CHECK=()
        while [[ "$#" -gt 0 && "$1" != --* ]]; do
          FILES_TO_CHECK+=($(realpath "$1"))
          shift
        done
        ;;
      -* | --*)
        echo "Unknown named parameter $1"
        shift 2
        do_exit=true
        ;;
      *)
        echo "Unauthorised positional argument: ${key}"
        shift
        do_exit=true
        ;;
      esac
    done
  fi

  if [ -z "${REPO_PATH}" ]; then
    echo "Missing parameter --repo_path."
    do_exit=true
  fi
  if [ -z "${HARBOR_USERNAME}" ]; then
    echo "Missing parameter --harbor_username."
    do_exit=true
  fi
  if [ -z "${HARBOR_PASSWORD}" ]; then
    echo "Missing parameter --harbor_password."
    do_exit=true
  fi
  if [ -z "${SERVER_ADDRESS}" ]; then
    echo "Missing parameter --server_address."
    do_exit=true
  fi
  if [ -z "${PROJECT_PATH}" ]; then
    echo "Missing parameter --project_path."
    do_exit=true
  fi
  if [ -z "${DOCKER_IMAGE_NAME}" ]; then
    echo "Missing parameter --docker_image_name."
    do_exit=true
  fi
  if [ -z "${DOCKER_IMAGE_TAG}" ]; then
    echo "Missing parameter --docker_image_tag."
    do_exit=true
  fi
  if [ "${#FILES_TO_CHECK[@]}" -eq 0 ]; then
    echo "Missing parameter --files_to_check."
    do_exit=true
  fi

  if ${do_exit}; then
    usage
    exit 1
  fi
}

function docker_login() {
  local harbor_username="$1"
  local harbor_password="$2"
  local server_address="$3"
  echo "${harbor_password}" | docker login "${server_address}" --username "${harbor_username}" --password-stdin
}

function docker_logout() {
  docker logout
}

function docker_build_and_push() {
  local full_docker_image_name="$1"
  local docker_file_name="$2"
  # build image
  docker build -t "${full_docker_image_name}" --progress=plain -f "${docker_file_name}" .
  # push imgae
  docker push "${full_docker_image_name}"
}

function docker_pull() {
  local full_docker_image_name=$1
  # pull image
  docker pull "${full_docker_image_name}"
}

# One should not compare the current branch to the parent branch
# from which the current branch has been created.
# This does not work if the current and parent branches are the same.
# Ideally, look for the diffs between last 2 push events to the current branch.
# This can account for multiple commits. But this requires github CLI and complicates things a lot.
# Instead, look for diffs with respect to the last commit. If everything is done by squashed PRs,
# things should be ok.
function has_git_diffs() {
  local repo_path="$1"
  declare -a files_to_check=("${!2}")

  local changes=$(
    git -C ${repo_path} diff --name-status HEAD~1 HEAD -- "${files_to_check[@]}"
  )
  if [[ -n "${changes}" ]]; then
    echo "Found changes with respect to the previous commit:"
    echo "${changes}"
    return 0
  else
    echo "No changes found with respect to previous commit."
    return 1
  fi
}

# Docker tags can only contain letters, digits, periods, hyphens and underscores.
# This function replaces anything else by underscores,
# This is particularly useful when an image is tagged by the name of the branch.
# Branches such as release/vx.y.z or feature/feature_name can result in failed tagging.
# Therefore, they are reformatted as release_vx.y.z or feature_feature_name.
function replace_special_chars_in_docker_tag() {
  local input="$1"
  echo "${input//[^a-zA-Z0-9._-]/_}"
}

function manage_docker_image() {
  local repo_path="$1"
  local server_address="$2"
  local project_path="$3"
  local docker_image_name="$4"
  local docker_image_tag="$5"
  local docker_file_name="$6"
  declare -a files_to_check=("${!7}")

  local reformatted_docker_image_tag=$(replace_special_chars_in_docker_tag "${docker_image_tag}")
  local full_docker_image_name="${server_address}/${project_path}/${docker_image_name}:${reformatted_docker_image_tag}"

  if has_git_diffs "${repo_path}" files_to_check[@]; then
    echo "Image will be built and pushed"
    docker_build_and_push "${full_docker_image_name}" "${docker_file_name}"
  else
    # pull
    echo "Image will be pulled"
    docker_pull "${full_docker_image_name}"
  fi
}

function main() {
  parse_args "$@"
  echo "Repo path       : ${REPO_PATH}"
  echo "Server name     : ${SERVER_ADDRESS}"
  echo "Project path    : ${PROJECT_PATH}"
  echo "Username        : ${HARBOR_USERNAME}"
  echo "Image name      : ${DOCKER_IMAGE_NAME}"
  echo "Image tag       : ${DOCKER_IMAGE_TAG}"
  echo "Full image name : ${SERVER_ADDRESS}/${PROJECT_PATH}/${DOCKER_IMAGE_NAME}:${DOCKER_IMAGE_TAG}"
  echo "File name       : ${DOCKER_FILE_NAME}"
  echo "Files to check  : "
  for file_to_check in "${FILES_TO_CHECK[@]}"; do
    echo "                  ${file_to_check}"
  done

  docker_login ${HARBOR_USERNAME} ${HARBOR_PASSWORD} ${SERVER_ADDRESS}

  manage_docker_image "${REPO_PATH}" \
    "${SERVER_ADDRESS}" \
    "${PROJECT_PATH}" \
    "${DOCKER_IMAGE_NAME}" \
    "${DOCKER_IMAGE_TAG}" \
    "${DOCKER_FILE_NAME}" \
    FILES_TO_CHECK[@]

  docker_logout
}

main "$@"
