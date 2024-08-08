#!/usr/local/bin/bash

error() {
  local exit_code="$?"
  echo "$(basename "$0")": "$1"
  exit $exit_code
}
