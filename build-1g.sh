#!/bin/bash

export WORKDIR="$(pwd)"

unset ARCH
unset CROSS_COMPILE

if [ ! -f "${WORKDIR}/../renesas-u-boot-cip/deploy/u-boot-elf.srec" ]; then
    echo "Please build u-boot first!"
    exit 1
fi

cd "${WORKDIR}/tools/bootparameter"
gcc -O bootparameter bootparameter.c

cd "${WORKDIR}"

export ARCH="arm64"
export CORES=`getconf _NPROCESSORS_ONLN`
export CROSS_COMPILE="aarch64-none-elf-"
export PATH="${WORKDIR}/../gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin:${WORKDIR}/tools/fiptool:${WORKDIR}/tools/bootparameter:$PATH"

if [ x"$1" = x"mrprober" ]; then
   rm -rf build 2>/dev/null || true
   make distclean
fi

if [ ! -d build ]; then
    mkdir -p build
fi

make O=build PLAT=g2l BOARD=novotech bl2 bl31 fiptool

rm -rf deploy/g2l 2>/dev/null || true
mkdir -p deploy/g2l

"${WORKDIR}/tools/bootparameter/bootparameter" \
    "${WORKDIR}/build/g2l/release/bl2.bin" \
    "${WORKDIR}/deploy/g2l/bl2_bp.bin"
cat "${WORKDIR}/build/g2l/release/bl2.bin" >> \
    "${WORKDIR}/deploy/g2l/bl2_bp.bin"

fiptool create --align 16 --soc-fw "${WORKDIR}/build/g2l/release/bl31.bin" \
    --nt-fw "${WORKDIR}/../renesas-u-boot-cip/deploy/u-boot.bin" \
    "${WORKDIR}/deploy/g2l/fip.bin"

objcopy -O srec --adjust-vma=0x00011E00 --srec-forceS3 -I binary \
    "${WORKDIR}/deploy/g2l/bl2_bp.bin" \
    "${WORKDIR}/deploy/g2l/bl2_bp.srec"
objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 \
    "${WORKDIR}/deploy/g2l/fip.bin" \
    "${WORKDIR}/deploy/g2l/fip.srec"

#cp -v build/rcar/release/bl2.srec deploy/g2l/bl2.srec
#cp -v build/rcar/release/bl31.srec deploy/g2l/bl31.srec
#cp -v tools/dummy_create/bootparam_sa0.srec deploy/g2l/bootparam_sa0.srec
#cp -v tools/dummy_create/cert_header_sa6.srec deploy/g2l/cert_header_sa6.srec
