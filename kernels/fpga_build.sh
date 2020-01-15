#!/bin/bash

set -e

TARGET=hw && DEVICE=xilinx_u200_xdma_201830_1 && xocc -c  --target $TARGET --platform $DEVICE interpol.cl -o interpol.xo

TARGET=hw && DEVICE=xilinx_u200_xdma_201830_1 && xocc -l  --target $TARGET --platform $DEVICE interpol.xo -o interpol.xclbin

