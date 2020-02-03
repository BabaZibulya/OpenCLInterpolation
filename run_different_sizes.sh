#!/bin/bash

for SIZE in 160 192 224 256 288 320 384 416 448 480
do
    ./run.sh $SIZE 60 60 60
done
