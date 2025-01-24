#!/bin/bash
export $(grep -v '^#' .env | xargs)
rm -rf build
mkdir build
cd build && cmake .. && make
./digital_cluster