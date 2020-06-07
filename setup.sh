#!/usr/bin/env bash
set -e
# Dependencies:
if [[ $(uname) == "Linux" ]]; then
    sudo apt install -y cmake python3
elif [[ $(uname) == "Darwin" ]]; then
    brew install python3 gcc cmake
fi

# Python Virtual Environment
if [[ ! -d "$env" ]]; then
    python3 -m venv env
fi
source env/bin/activate
pip install --upgrade pip
pip install -r "requirements.txt"
deactivate

./build/build.sh Release
