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
 - Circuit::Simulator       # controls output
 - Circuit::Math            # provides math functions

 - Circuit::Component       # component abstraction
 - Circuit::Resistor        # resistor abstraction
 - Circuit::Capacitor       # capacitor abstraction
 - Circuit::Inductor        # inductor abstraction
 - Circuit::Diode           # diode abstraction
 - Circuit::Transistor      # transistor abstraction
 - Circuit::Mosfet          # mosfet abstraction

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
    std::vector<Simulation *> sims;
    void out() const;
    void setupConnections2Node( Circuit::Component *linear, const std::string& nodeA, const std::string& nodeB );
    void setupConnections3Node( Circuit::Component *linear, const std::string& nodeA, const std::string& nodeB, const std::string& nodeC );

``` 

#### Circuit::Node

```c++

	Node(const std::string &name, Circuit::Schematic * schem);
    double voltage;
    std::vector<Component *> comps;
    void print() const;
    std::string getName() const;
    int getId() const;

```

#### Circuit::Parser

```c++

	static Circuit::Schematic* parse( std::istream& inputStream );

``` 

#### Circuit::Simulator

```c++

    static enum SimulationType;
    Simulator( Circuit::Schematic *schem, SimulationType type );
    const SimulationType type;
    void run();

```

#### Circuit::Math

```c++

TODO - Add Math definitions

``` 

#### Circuit::Component

```c++

    std::string name;
    std::vector<Node *> nodes;
    virtual double conductance(ParamTable *param) const
    Node* getPosNode() const
    Node* getNegNode() const
    double current() const
    void print() const
    virtual double getValue(ParamTable *param) const
    virtual bool isSource() const

```

#### Circuit::Resistor

```c++

    Resistor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic* schem)
    double conductance() const override

``` 

#### Circuit::Capacitor

```c++

    double DC_init;
    Capacitor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic* schem)
    Capacitor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic* schem, double DC_init)
    double conductance() const override

```

#### Circuit::Inductor

```c++

    double I_init;
    Inductor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic* schem)
    Inductor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem, double I_init)
    double conductance() const override

``` 

#### Circuit::Diode

```c++

	double IS=0.1;
	double RS=16;
	double CJO=2e-12;
	double TT=12e-9;
	double BV=100;
	double IBV=0.1e-12;
	Diode( const std::string &name, const std::string &nodeA, const std::string &nodeB, const std::string &model, Schematic* schem)
	void assignModel( std::vector<std::string> params )
	double conductance() const override
	std::string getModelName()
	virtual double getValue() override

```

#### Circuit::Transistor

```c++

	double BF=100;
	double IS=1e-16;
	double VAF=std::numeric_limits<double>::max();
	bool model_CE_resistance = false;
	TType transistorType;
	Transistor( const std::string &name, const std::string &nodeCollector, const std::string &nodeBase, const std::string &nodeEmitter, const std::string &model, Schematic* schem)
	void assignModel( std::vector<std::string> params )
	double conductance() const override
	std::string getModelName()
	virtual double getValue() override

``` 

#### Circuit::Source

```c++

	double getSourceOutput( ParamTable* param, double t )
	virtual bool isSource() const override
	virtual bool isCurrent() const = 0;

```

#### Circuit::Voltage

```c++

	Voltage( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, double smallSignalAmp, double SINE_DC_offset ,double SINE_amplitude, double SINE_frequency, Schematic* schem )
	Voltage( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, Schematic *schem )
	bool isCurrent() const override

``` 

#### Circuit::Current

```c++

	Current( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, double smallSignalAmp, double SINE_DC_offset, double SINE_amplitude, double SINE_frequency, Schematic* schem )
	Current( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, Schematic *schem )
	bool isCurrent() const override

```

## Authors

 - [Jonah Lehner](https://github.com/jjlehner)
 - [Samuel Adekunle](https://github.com/SamtheSaint)
 - [Neel Dugar](https://github.com/neeldug)
