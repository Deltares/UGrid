#!/usr/bin/env bash

set -e

source /workspace/scripts/docker/utilities.sh

print_text_box "Download sonar tools"
python3 /workspace/scripts/download_sonar_tools.py --save_dir ${SONAR_SAVE_DIR}
chmod +x ${SONAR_SAVE_DIR}/build-wrapper-linux-x86/build-wrapper-linux-x86-64
chmod +x ${SONAR_SAVE_DIR}/sonar-scanner/bin/sonar-scanner
chmod +x ${SONAR_SAVE_DIR}/sonar-scanner/jre/bin/java
chmod +x ${SONAR_SAVE_DIR}/sonar-scanner/jre/lib/jspawnhelper
