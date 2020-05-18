#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

#include <iostream>
#include <algorithm>

#include "circuit_structure.hpp"
#include "symbolic.hpp"

void getCurrentVector(const Circuit::Schematic &circuit, Symbolic &currentVector)
{
    std::cout << "Getting Current Vector" << std::endl;
    std::for_each(circuit.sources.begin(), circuit.sources.end(), [=](const auto source){
        if (source->pos->id != -1) std::cout << currentVector(source->pos->id);
        if (source->neg->id != -1) std::cout << currentVector(source->neg->id);
    });
}

void generateMatrices(const Circuit::Schematic &circuit)
{
    const int NUM_NODES = circuit.nodes.size();
    Symbolic currentVector("currentVector", NUM_NODES);
    Symbolic voltageVector("voltageVector", NUM_NODES);
    Symbolic conductanceMatrix("conductanceMatrix", NUM_NODES, NUM_NODES);
    // std::cout << int(expand(det(conductanceMatrix))) << std::endl;
    getCurrentVector(circuit, currentVector);
}

#endif