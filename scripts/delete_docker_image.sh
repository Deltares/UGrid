#!/usr/bin/env bash

set -e

function usage() {
  echo "Usage: "
  echo "$0 \\"
  echo "  --username <HARBOR_USERNAME> \\"
  echo "  --password <HARBOR_PASSWORD> \\"
  echo "  --server_address <SERVER_ADDRESS> \\"
  echo "  --project_path <PROJECT_PATH> \\"
  echo "  --repository_name <REPOSITORY_NAME> \\"
  echo "  --artifact <ARTIFACT>"
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
    --username)
      declare -g USERNAME="$2"
      shift 2
      ;;
    --password)
      declare -g PASSWORD="$2"
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
    --repository_name)
      declare -g REPOSITORY_NAME="$2"
      shift 2
      ;;
    --artifact)
      declare -g ARTIFACT="$2"
      shift 2
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

  if [ -z "${USERNAME}" ]; then
    echo "Missing parameter --username."
    do_exit=true
  fi
  if [ -z "${PASSWORD}" ]; then
    echo "Missing parameter --password."
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
  if [ -z "${REPOSITORY_NAME}" ]; then
    echo "Missing parameter --repository_name."
    do_exit=true
  fi
  if [ -z "${ARTIFACT}" ]; then
    echo "Missing parameter --artifact."
    do_exit=true
  fi

  if ${do_exit}; then
    usage
    exit 1
  fi
}

# Function for deleting a untagged Docker images from a repository in a registry
delete_docker_image() {
  local username="$1"
  local password="$2"
  local server_address="$3"
  local project_path="$4"
  local repository_name="$5"
  local artifact="$6"

  local page=1
  local has_more_pages=true

  while ${has_more_pages}; do
    # get all artifacts from current page
    artifacts=$(
      curl \
        -s \
        -u "${username}:${password}" \
        -X 'GET' \
        "https://${server_address}/api/v2.0/projects/${project_path}/repositories/${repository_name}/artifacts?page=${page}" \
        -H 'accept: application/json'
    )

    if [ "$(echo "${artifacts}" | jq '. | length')" -eq 0 ]; then
      has_more_pages=false
    else
      # determine which artifacts are not tagged
      case $artifact in
      untagged)
        # remove all untagged artifacts
        digests_of_artifacts_to_delete=$(
          echo "${artifacts}" | jq -r '.[] | select(.tags == null) | .digest'
        )
        ;;
      features)
        # remove all artifacts stating with feature_ (these are usually associated with feature branches)
        digests_of_artifacts_to_delete=$(
          echo "${artifacts}" |
            jq -r '.[] | select(
              .tags != null 
              and (.tags[].name | test("^feature_"))
            ) | .digest'
        )
        ;;
      stale)
        # remove all artifacts except the ones tagged as main or release_vx.y.z
        # this mode is best for scheduled clean-ups
        digests_of_artifacts_to_delete=$(
          echo "${artifacts}" |
            jq -r '.[] | select(
            .tags == null
            or(
              (.tags[].name != "main")
              and (.tags[].name | test("^release_v[0-9]+\\.[0-9]+\\.[0-9]+$") | not) )
            ) | .digest'
        )
        ;;
      *)
        # remove specific artifact by exact tag, for example a specific feature
        # use with extreme care: can be used to delete any artifact including ones tagged as main or release_vx.y.z
        digests_of_artifacts_to_delete=$(
          echo "${artifacts}" |
            jq -r --arg artifact "${artifact}" '.[] | select(
              .tags != null
              and (.tags[].name == $artifact)
            ) | .digest'
        )
        ;;
      esac

      # then delete them
      for digest in ${digests_of_artifacts_to_delete}; do
        echo "Deleting artifact with digest ${digest} from page ${page}..."
        curl \
          -u "${username}:${password}" \
          -X 'DELETE' \
          "https://${server_address}/api/v2.0/projects/${project_path}/repositories/${repository_name}/artifacts/${digest}" \
          -H 'accept: application/json'
      done

      # go to next page
      ((page++))
    fi
  done
  echo "Clean-up complete."
}

function main() {
  parse_args "$@"
  echo "Username        : ${USERNAME}"
  echo "Server address  : ${SERVER_ADDRESS}"
  echo "Project path    : ${PROJECT_PATH}"
  echo "Repository name : ${REPOSITORY_NAME}"
  echo "Artifact        : ${ARTIFACT}"
  echo ""

  delete_docker_image \
    "${USERNAME}" \
    "${PASSWORD}" \
    "${SERVER_ADDRESS}" \
    "${PROJECT_PATH}" \
    "${REPOSITORY_NAME}" \
    "${ARTIFACT}"
}

main "$@"
