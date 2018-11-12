#!/bin/bash
set -e

mkdir build && cd build
cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ..
make -j8
