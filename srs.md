# Error 404 Circuit Simulator

## Table of Contents

[1 Introduction](#introduction)

[1.1 Purpose](#purpose)

[1.2 Intended Audience](#intended-audience)

[1.3 Intended Use](#intended-use)

[1.4 Scope](#scope)

[1.5 Definitions and Acronyms](#definitions-and-acronyms)

[2 Overall Description](#overall-description)

[2.1 User Needs](#user-needs)

[2.2 Assumptions and Dependencies](#assumptions-and-dependencies)

[3 System Features and Requirements](#system-features-and-requirements)

[3.1 Functional Requirements](#functional-requirements)

[3.2 External Interface Requirements](#external-interface-requirements)

[3.3 System Features](#system-features)

[3.4 Nonfunctional Requirements](#nonfunctional-requirements)

## 1 Introduction

### 1.1 Purpose

The purpose of this software is to simulate circuits and solve voltages at every node and currents of every component by using nodal analysis. This software emulates LT Spice, however, using a command line interface.

### 1.2 Intended Audience

Our intended audience would be electrical engineers who would prefer a less bloated and cross platform solution than LT Spice. This would also include other developers who would like to fork this project, or testers to ensure our program is bug-free.

### 1.3 Intended Use

Intended use would be to generate output data files using our simulator that can be viewed using our python script, specifically we accept net lists in the exact same format as LT Spice, however, we have yet to implement specific models.

### 1.4 Scope

Our circuit simulator uses conductance matrices to calculate voltages and currents using methods such as basic nodal analysis, iteration and equivalent models to certain components. Our goals while pursuing this project, was to create a much more performant alternative, with a nicer UI, compared to LT Spice. The benefits to using our program, is speed, <Insert benchmarks here>, simplicity, control of output format.

### 1.5 Definitions and Acronyms

Our simulator uses theoretical principles, which may not hold to precision when compared to practical examples, hence we'd recommend not using this to calculate to such a high precision, especially when working with non-linear components.

## 2 Overall Description

### 2.1 User Needs

The user would typically have an existing netlist file, this should start with lines containing component names, connected nodes, and their intrinsic properties, i.e. resistance or capacitance.

### 2.2 Assumptions and Dependencies

We assume ideal characteristics of components, in that for most components there is no parasitic capacitance or inductance, moreover, we also assume that nodal analysis is always true, which also assumes wires and connectors are essentially perfect conductors.

Dependencies: `Python3` & install packages from `requirements.txt`

## 3 System Features and Requirements

### 3.1 Functional Requirements

* Parse netlist into various data structures
* Generate conductance matrix from data structures
* Invert/Solve conductance matrix to find voltages and currents
* Print results into CSV or space separated formats and plot

### 3.2 External Interface Requirements

* The CSV produced is of standard formatting and can be used in conjunction with any software that accepts CSV formats.
* Anything else to go here << REMOVE

### 3.3 System Features

* We require the Eigen library to process our matrices and use LU decomposition, we also considered using Cholesky Decomposition, however, << need to do this

### 3.4  Nonfunctional requirements

* Ensure memory safety
* Create a functioning API that is legible and usable for others
* Create a performant solution which can be energy efficient and much quicker than LT SPICE
* << Anything else here
