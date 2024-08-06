FROM quay.io/pypa/manylinux_2_28_x86_64:2024-07-01-8dac23b

WORKDIR /workspace

COPY . .

RUN chmod +x ./dnf_install.sh && ./dnf_install.sh

ENTRYPOINT ["bash", "./main.sh"]
