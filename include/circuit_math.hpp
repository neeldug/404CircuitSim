#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

#include <iostream>

#include "circuit_structure.hpp"
#include "symbolic.hpp"

void getCurrentVector(const Circuit::Schematic &circuit, Symbolic &currentVector)
{
    std::cout << "Getting Current Vector" << std::endl;
    // std::map<std::string, Circuit::CurrentSource *>::iterator it = circuit.sources.begin();
}

void generateMatrices(const Circuit::Schematic &circuit)
{
    const int NUM_NODES = circuit.nodes.size();
    Symbolic currentVector("currentVector", NUM_NODES);
    Symbolic voltageVector("voltageVector", NUM_NODES);
    Symbolic conductanceMatrix("conductanceMatrix", NUM_NODES, NUM_NODES);
    getCurrentVector(circuit, currentVector);
}

#endif