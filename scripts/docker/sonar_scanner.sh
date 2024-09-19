#!/usr/bin/env bash

set -e

export netCDFCxx_DIR=${THIRD_PARTY_INSTALL_DIR}/netcdf_cxx4/lib64/cmake/netCDF/

sonar_build_dir="/workspace/build_sonar"

if [[ "${CLEAN_BUILD}" = "true" && -d ${sonar_build_dir} ]]; then
    print_text_box "Clean build"
    # cmake --build ${sonar_build_dir} --target clean ||
    #   error "[cmake] Failed to clean project"
    rm -fr ${sonar_build_dir}
fi

# download sonar tools
rm -fr .sonar
print_text_box "Download sonar tools"
python3 /workspace/scripts/download_sonar_tools.py
chmod +x .sonar/build-wrapper-linux-x86/build-wrapper-linux-x86-64
ls -ltr .sonar
ls -ltr .sonar/build-wrapper-linux-x86

# configure
print_text_box "Configure Sonar build"
cmake \
    -S . \
    -B ${sonar_build_dir} \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_PREFIX_PATH="${THIRD_PARTY_INSTALL_DIR}" \
    -DENABLE_CODE_COVERAGE:BOOL=ON ||
    error "[cmake (sonar)] Failed to configure project"

# build
print_text_box "Sonar Build"
sonar_out_dir="/workspace/sonar_analysis"
.sonar/build-wrapper-linux-x86/build-wrapper-linux-x86-64 \
    --out-dir ${sonar_out_dir} \
    cmake \
    --build ${sonar_build_dir} \
    --config ${BUILD_TYPE} \
    --parallel ||
    error "[sonar build-wrapper-linux-x86-64] Failed to build project"

# run tests
print_text_box "Test"
${sonar_build_dir}/tests/api/UGridAPITests

# Generate coverage report
print_text_box "Generate and collect coverage report"
gcov_reports_dir="/workspace/gcov_reports_dir"
# run gcov on all .gcda files found
find ${sonar_build_dir} -iname '*.gcda' -exec gcov -p {} ';' >/dev/null
# make gcov_reports_dir directory to store all coverage results
rm -rf ${gcov_reports_dir}
mkdir ${gcov_reports_dir}
# move the coverage results to gcov_reports_dir
find . -maxdepth 1 -iname '*.gcov' -exec mv {} ${gcov_reports_dir} ';'

# Analyse
print_text_box "Analyse coverage reports"
chmod +x .sonar/sonar-scanner/bin/sonar-scanner
chmod +x .sonar/sonar-scanner/jre/bin/java
.sonar/sonar-scanner/bin/sonar-scanner \
    -X \
    -Dsonar.verbose=false \
    -Dsonar.token=${SONAR_TOKEN} \
    -Dsonar.organization=deltares \
    -Dsonar.projectKey=Deltares_UGrid \
    -Dsonar.sources=libs \
    -Dsonar.cfamily.build-wrapper-output=${sonar_out_dir} \
    -Dsonar.host.url=https://sonarcloud.io \
    -Dsonar.branch.name=%teamcity.build.branch% \
    -Dsonar.cpp.std=c++20 \
    -Dsonar.cfamily.gcov.reportsPath=${gcov_reports_dir} \
    -Dsonar.cpd.exclusions=libs/UGridNET/**/*
