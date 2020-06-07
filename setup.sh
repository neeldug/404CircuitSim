#!/usr/bin/env bash
set -e
base64 -d <<< "4pSP4pSB4pW44pSP4pSB4pST4pSP4pSB4pST4pSP4pSB4pST4pSP4pSB4pSTICAg4pW7IOKVu+KUj+KUgeKUk+KVuyDilbsgICDilI/ilIHilJPilbvilI/ilLPilJPilbsg4pW74pW7ICDilI/ilIHilJPilbrilLPilbjilI/ilIHilJPilI/ilIHilJMK4pSj4pW4IOKUo+KUs+KUm+KUo+KUs+KUm+KUgyDilIPilKPilLPilJsgICDilJfilIHilKvilIPilIPilIPilJfilIHilKsgICDilJfilIHilJPilIPilIPilIPilIPilIMg4pSD4pSDICDilKPilIHilKsg4pSDIOKUgyDilIPilKPilLPilJsK4pSX4pSB4pW44pW54pSX4pW44pW54pSX4pW44pSX4pSB4pSb4pW54pSX4pW4ICAgICDilbnilJfilIHilJsgIOKVuSAgIOKUl+KUgeKUm+KVueKVuSDilbnilJfilIHilJvilJfilIHilbjilbkg4pW5IOKVuSDilJfilIHilJvilbnilJfilbgK
"
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
