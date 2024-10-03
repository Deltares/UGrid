FROM quay.io/pypa/manylinux_2_28_x86_64:latest

WORKDIR /workspace

COPY . /workspace

ENV CLEAN_BUILD="false"
ENV BUILD_TYPE="Release"
ENV VERBOSE_BUILD="false"

ENV RUN_SONAR_SCANNER="false"
ENV SONAR_TOKEN=""
ENV BRANCH_NAME=""

RUN chmod +x /workspace/scripts/docker/dnf_install.sh
RUN /workspace/scripts/docker/dnf_install.sh

ARG SONAR_SAVE_DIR=/sonar
ENV SONAR_SAVE_DIR=${SONAR_SAVE_DIR}
RUN chmod +x /workspace/scripts/docker/downlaod_sonar_tools.sh
RUN /workspace/scripts/docker/downlaod_sonar_tools.sh

ARG THIRD_PARTY_WORK_DIR=/third_party/work
ARG THIRD_PARTY_INSTALL_DIR=/third_party/install
ENV THIRD_PARTY_INSTALL_DIR=${THIRD_PARTY_INSTALL_DIR}
RUN chmod +x /workspace/scripts/install_netcdf_cxx4.sh
RUN /workspace/scripts/install_netcdf_cxx4.sh \
    --work_dir ${THIRD_PARTY_WORK_DIR} \
    --install_dir ${THIRD_PARTY_INSTALL_DIR} \
    --clean

ENTRYPOINT ["bash", "/workspace/scripts/docker/main.sh"]
