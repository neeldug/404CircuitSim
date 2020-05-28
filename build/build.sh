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
<<<<<<< HEAD

cmake --build .
=======
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ --build .
>>>>>>> 2b4512ecd555df6aab71fcf77afb5a45d192a3f4
