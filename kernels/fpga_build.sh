#!/bin/bash
set -e

TARGET=hw && DEVICE=xilinx_u200_xdma_201830_2 && xocc -c  --target $TARGET --platform $DEVICE intelpol.cl -o interpol.xo

TARGET=hw && DEVICE=xilinx_u200_xdma_201830_2 && xocc -l  --target $TARGET --platform $DEVICE intelpol.xo -o interpol.xclbin