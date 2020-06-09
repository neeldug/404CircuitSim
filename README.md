```bash
┏━╸┏━┓┏━┓┏━┓┏━┓   ╻ ╻┏━┓╻ ╻   ┏━┓╻┏┳┓╻ ╻╻  ┏━┓╺┳╸┏━┓┏━┓
┣╸ ┣┳┛┣┳┛┃ ┃┣┳┛   ┗━┫┃┃┃┗━┫   ┗━┓┃┃┃┃┃ ┃┃  ┣━┫ ┃ ┃ ┃┣┳┛
┗━╸╹┗╸╹┗╸┗━┛╹┗╸     ╹┗━┛  ╹   ┗━┛╹╹ ╹┗━┛┗━╸╹ ╹ ╹ ┗━┛╹┗╸
```
## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Usage](#usage)
4. [Authors](#authors)

## Introduction

<!-- TODO - Add Project Introduction Aim and Motivation (Probably at the end) -->

## Getting Started

### System Requirements

#### Operating System

 - Linux: Ubuntu 18.04 or higher
 - MacOS: Catalina 10.15 or higher

#### Terminal Shell

 - bash (recommended)
 - sh
 - zsh

### CMake

CMake is used for building and managing dependencies. To install CMake, [click here](). To confirm if CMake is installed correctly on your machine, run:
```bash
cmake --version
```
The version of CMake installed should come up. CMake 3.16.X is required!

### Python (Only for the graphing tool)

To use the in built graphing tool, Python 3 is required. To install python, [click here](https://docs.python-guide.org/starting/installation/). To confirm if Python 3 is installed correctly on your machine, run:
```bash
python3
```
An interactive shell should come up showing the python version. Python 3.X is required!

Once Python 3 has been correctly installed, navigate to the root directory and run:
```bash
python3 -m pip install -r requirements.txt
```
To install the necessary packages required for the graphing tool to work.

### Installing

To install the simulator, navigate to the build directory and run:
```bash
./install.sh
```
And follow the on screen instructions.

## Usage
```
-i      <file>      path to input netlist
-o      <dir>       path to output directory
-f      <format>    specify output format, either csv or space
-p      <list>      plots output, space separated list specifies columns to plot
-c                  shows names of columns in output file, blocks -p and doesn't plot result

Usage: simulator -i filename -p list [-c] [ -o outputdir ] [ -f csv ]

Examples:

Plot Specific Columns:
    simulator -i test.net -p 'V(N001) V(N002)'

Plot All Columns:
    simulator -i test.net -p ''
```

## Authors

 - [Jonah Lehner](https://github.com/jjlehner)
 - [Samuel Adekunle](https://github.com/SamtheSaint)
 - [Neel Dugar](https://github.com/neeldug)
