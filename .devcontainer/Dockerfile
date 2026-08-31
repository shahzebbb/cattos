# Get x86 BIOS GRUB modules
FROM --platform=linux/amd64 ubuntu:24.04 AS grub_i386

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y --no-install-recommends grub-pc-bin \
 && rm -rf /var/lib/apt/lists/*

# Actual development container — native architecture
FROM --platform=$BUILDPLATFORM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG TARGET=i686-elf
ARG PREFIX=/opt/cross
ARG BINUTILS_VERSION=2.46.0
ARG GCC_VERSION=16.1.0

ENV PATH=${PREFIX}/bin:${PATH}

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    bison \ 
    flex \
    libgmp-dev \
    libmpc-dev \
    libmpfr-dev \
    libisl-dev \
    texinfo \
    grub-common \
    xorriso \
    qemu-system-x86 \
    git \
    wget \
    curl \
    tmux \
    ca-certificates \
    file \
    make \
    gdb \
    mtools \
    nasm \
    python3 \
    pkg-config \
 && rm -rf /var/lib/apt/lists/*

COPY --from=grub_i386 /usr/lib/grub/i386-pc /usr/lib/grub/i386-pc

COPY scripts/build-i686-elf-toolchain.sh /tmp/build-i686-elf-toolchain.sh
RUN chmod +x /tmp/build-i686-elf-toolchain.sh \
 && /tmp/build-i686-elf-toolchain.sh \
 && rm /tmp/build-i686-elf-toolchain.sh

RUN i686-elf-gcc --version \
 && i686-elf-ld --version \
 && grub-mkrescue --version \
 && command -v grub-file \
 && qemu-system-i386 --version \
 && make --version
