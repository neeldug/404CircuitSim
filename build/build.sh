#!/bin/bash

#to build executables, just run this file

cd "$(dirname "$0")"
cmake ..

cmake --build .