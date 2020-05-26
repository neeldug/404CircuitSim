#!/bin/bash

set -e

#to build executables, just run this file
if [[ $(uname) == "Linux" ]]; then
    cd "$(dirname "$0")"
    cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..

elif [[ $(uname) == "Darwin" ]]; then
    cd "$(dirname "$0")"
    cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
else
    echo "Windows?"
    exit
fi
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ --build .
