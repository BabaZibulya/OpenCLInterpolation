#!/bin/bash

set -e

TARGET=sw_emu
DEVICE=xilinx_u200_xdma_201830_1
KERNEL=interpol

xocc -c  --target $TARGET --platform $DEVICE ${KERNEL}.cl -o $TARGET/${KERNEL}.xo
xocc -l  --target $TARGET --platform $DEVICE $TARGET/${KERNEL}.xo -o $TARGET/${KERNEL}.xclbin

