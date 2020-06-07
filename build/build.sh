#!/bin/bash

set -e

cd "$(dirname "$0")"
cmake -DCMAKE_BUILD_TYPE="$1" -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..

cmake --build .
