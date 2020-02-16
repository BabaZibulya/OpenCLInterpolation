#!/bin/bash

set -e

TARGET=hw
DEVICE=xilinx_u200_xdma_201830_1
KERNEL=convolution

xocc -c  --target $TARGET --platform $DEVICE ${KERNEL}.cl -o $TARGET/${KERNEL}.xo
xocc -l  --target $TARGET --platform $DEVICE $TARGET/${KERNEL}.xo -o $TARGET/${KERNEL}.xclbin

