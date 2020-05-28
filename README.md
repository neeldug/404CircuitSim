# 404 Circuit Simulator

## Table of Contents

1. [Introduction](#introduction)
2. [Folder Structure](#folder-structure)
3. [Getting Started](#getting-started)
4. [Technical Documentation](#technical-documentation)
5. [Authors](#authors)

## Introduction

TODO - Add Project Introduction Aim and Motivation (Probably at the end)

## Folder Structure

``` 
├── bin
├── build
│   └── build.sh
├── CMakeLists.txt
├── include
│   ├── circuit_diode.hpp
│   ├── circuit.hpp
│   ├── circuit_linear.hpp
│   ├── circuit_math.hpp
│   ├── circuit_mosfet.hpp
│   ├── circuit_parser.hpp
│   ├── circuit_source.hpp
│   ├── circuit_structure.hpp
│   ├── circuit_transistor.hpp
│   └── symbolic.hpp
├── lib
│   └── symbolic
├── src
│   ├── main.cpp
│   ├── math
│   │   └── math.cpp
│   └── parser
│       └── parser.cpp
├── test
│   ├── parser_test.cpp
│   ├── SpiceNetlists
│   │   └── test.cir
│   └── test_math.cpp
```

## Getting Started

TODO - Add Getting Started Steps (Probably Last Thing) also add command line option etc

## Technical Documentation

### Project Specification

 - Input Format: The circuit simulator should read an input file that defines the circuit and the simulation. The format of the input file is compatible with SPICE, but you only need to support a subset of the features of SPICE, which are described in [this document](spec.pdf).

 - Output Format: The output format will be a ?? (TODO)

### API Documentation

The API of the Circuit Simulator is a header only library, where each header file defines one or more classes that are used in the cpp files. All classes are namespaced under circuit to avoid conflicts.

#### Classes

``` 
 - Circuit::Schematic       # circuit abstraction
 - Circuit::Node            # circuit node abstraction
 - Circuit::Parser          # controls input
 - Circuit::Math            # controls output

 - Circuit::Component       # component abstraction
 - Circuit::Resistor        # resistor abstraction
 - Circuit::Capacitor       # capacitor abstraction
 - Circuit::Inductor        # inductor abstraction
 - Circuit::Diode           # diode abstraction
 - Circuit::Transistor      # transistor abstraction

 - Circuit::Source          # power source abstraction
 - Circuit::Voltage         # voltage source abstraction
 - Circuit::Current         # current source abstraction
 ```

#### Circuit::Schematic

```c++

    Schematic();
    std::function<int()> id;
    std::string title;
    std::map<std::string, Node *> nodes;
    std::map<std::string, Component *> comps;
    std::vector<Source *> sources;
    class Simulation;
    std::vector<Simulation *> sims;
    void out();
    void setupConnections2Node( Circuit::Component *linear, std::string nodeA, std::string nodeB );
    void setupConnections3Node( Circuit::Component *linear, std::string nodeA, std::string nodeB, std::string nodeC );

``` 

#### Circuit::Node

```c++

    Node(const std::string& name)
	Node(const std::string &name, float voltage, int id)
    int id;
    float voltage;
    std::vector<Component *> comps;
    void print()
    std::string getName()

```

#### Circuit::Parser

```c++

	static Circuit::Schematic* parse( std::istream& inputStream );

``` 

#### Circuit::Math

```c++

TODO

``` 

#### Circuit::Component

```c++

    std::string name;
    std::vector<Node *> nodes;
    virtual float conductance() const
    Node* getPosNode()
    Node* getNegNode()
    float current()
    void print()
    virtual ~Component()
    virtual float getValue()

```

#### Circuit::Resistor

```c++

    Resistor(std::string name, float value, Schematic* schem)
    Resistor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic* schem)
    float conductance() const override

``` 

#### Circuit::Capacitor

```c++

    float DC_init;
    Capacitor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic* schem)
    Capacitor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic* schem, float DC_init)
    float conductance() const override

```

#### Circuit::Inductor

```c++

    float I_init;
    Inductor(std::string name, float value, Schematic* schem) : Component(name, value, schem )
    Inductor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic* schem)
    Inductor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic *schem, float I_init)
    float conductance() const override

``` 

#### Circuit::Diode

```c++

	float IS=0.1;
	float RS=16;
	float CJO=2e-12;
	float TT=12e-9;
	float BV=100;
	float IBV=0.1e-12;
	Diode( std::string name, std::string nodeA, std::string nodeB, std::string model, Schematic* schem)
	void assignModel( std::vector<std::string> params )
	float conductance() const override
	std::string getModelName()
	virtual float getValue() override

```

#### Circuit::Transistor

```c++

	float BF=100;
	float IS=1e-16;
	float VAF=std::numeric_limits<float>::max();
	bool model_CE_resistance = false;
	TType transistorType;
	Transistor( std::string name, std::string nodeCollector, std::string nodeBase, std::string nodeEmitter, std::string model, Schematic* schem)
	void assignModel( std::vector<std::string> params )
	float conductance() const override
	std::string getModelName()
	virtual float getValue() override

``` 

#### Circuit::Source

```c++

	virtual float getValue() override
	virtual bool isCurrent() const = 0;

```

#### Circuit::Voltage

```c++

	Voltage( std::string name, float DC, std::string nodePos, std::string nodeNeg, float smallSignalAmp, float SINE_DC_offset ,float SINE_amplitude, float SINE_frequency, Schematic* schem )
	Voltage( std::string name, float DC, std::string nodePos, std::string nodeNeg, Schematic *schem )
	bool isCurrent() const override

``` 

#### Circuit::Current

```c++

	Current( std::string name, float DC, std::string nodePos, std::string nodeNeg, float smallSignalAmp, float SINE_DC_offset, float SINE_amplitude, float SINE_frequency, Schematic* schem )
	Current( std::string name, float DC, std::string nodePos, std::string nodeNeg, Schematic *schem )
	bool isCurrent() const override

```

## Authors

 - [Jonah Lehner](https://github.com/jjlehner)
 - [Samuel Adekunle](https://github.com/SamtheSaint)
 - [Neel Dugar](https://github.com/neeldug)
