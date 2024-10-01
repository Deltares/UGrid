#!/usr/bin/env bash

set -e

source /workspace/scripts/docker/error.sh
source /workspace/scripts/docker/utilities.sh
source /workspace/scripts/docker/build_and_test.sh
#if [[ "${RUN_SONAR_SCANNER}" = "true" ]]; then
source /workspace/scripts/docker/sonar_scanner.sh
#fi
