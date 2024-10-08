#!/usr/bin/env bash

set -e

function usage() {
  echo "Usage: $0 \
--repo_path <REPO_PATH> \
--harbor_username <HARBOR_USERNAME> \
--harbor_password <HARBOR_PASSWORD> \
--server_address <SERVER_ADDRESS> \
--project_path <PROJECT_PATH> \
--docker_image_name <DOCKER_IMAGE_NAME> \
--docker_image_tag <DOCKER_IMAGE_TAG> \
--docker_file_name <DOCKER_FILE_NAME>"
}

function parse_args() {
  local do_exit=false
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

  # ensure all required arguments are provided
  if [ -z "${REPO_PATH}" ] ||
    [ -z "${HARBOR_USERNAME}" ] ||
    [ -z "${HARBOR_PASSWORD}" ] ||
    [ -z "${SERVER_ADDRESS}" ] ||
    [ -z "${PROJECT_PATH}" ] ||
    [ -z "${DOCKER_IMAGE_NAME}" ] ||
    [ -z "${DOCKER_IMAGE_TAG}" ] ||
    [ -z "${DOCKER_FILE_NAME}" ] ||
    [ "${#FILES_TO_CHECK[@]}" -eq 0 ]; then
    echo "Missing parameter --."
    do_exit=true
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

function get_default_branch_name() {
  local repo_path="$1"
  echo $(
    git -C "${repo_path}" symbolic-ref refs/remotes/origin/HEAD |
      sed 's@^refs/remotes/origin/@@'
  )
}

function get_parent_branch_name() {
  local repo_path="$1"
  echo $(
    git -C ${repo_path} show-branch |
      grep '\*' |
      grep -v $(git -C "${repo_path}" rev-parse --abbrev-ref HEAD) |
      head -n1 |
      sed 's/.*\[//g' | sed 's/\].*//g'
  )
}

function has_git_diffs() {
  local repo_path="$1"
  local parent_branch="$2"
  declare -a files_to_check=("${!3}")

  local changes=$(
    git -C ${repo_path} diff --name-status "${parent_branch}" -- "${files_to_check[@]}"
  )
  if [[ -n "${changes}" ]]; then
    echo "Found changes with respect to branch ${parent_branch}:"
    echo "${changes}"
    return 0
  else
    echo "No changes found with respect to branch ${parent_branch}"
    return 1
  fi
}

function manage_docker_image() {
  local repo_path="$1"
  local server_address="$2"
  local project_path="$3"
  local docker_image_name="$4"
  local docker_image_tag="$5"
  local docker_file_name="$6"
  declare -a files_to_check=("${!7}")

  local full_docker_image_name="${server_address}/${project_path}/${docker_image_name}:${docker_image_tag}"

  local parent_branch=$(get_parent_branch_name "${repo_path}")
  if has_git_diffs "${repo_path}" "${parent_branch}" files_to_check[@]; then
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
