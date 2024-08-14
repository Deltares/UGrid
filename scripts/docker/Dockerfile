FROM quay.io/pypa/manylinux_2_28_x86_64:2024-07-01-8dac23b

WORKDIR /workspace

COPY . /workspace/

RUN chmod +x /workspace/dnf_install.sh
RUN /workspace/dnf_install.sh

ARG THIRD_PARTY_WORK_DIR=/third_party/work
ARG THIRD_PARTY_INSTALL_DIR=/third_party/install
ENV THIRD_PARTY_INSTALL_DIR=${THIRD_PARTY_INSTALL_DIR}

RUN chmod +x /workspace/install_netcdf_cxx4.sh
RUN /workspace/install_netcdf_cxx4.sh \
    --work_dir ${THIRD_PARTY_WORK_DIR} \
    --install_dir ${THIRD_PARTY_INSTALL_DIR} \
    --clean

ENV CLEAN_BUILD=false

ENV BUILD_TYPE=Release

ENV VERBOSE=false

ENTRYPOINT ["bash", "/workspace/scripts/docker/main.sh"]
