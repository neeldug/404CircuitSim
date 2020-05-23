#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

#include <iostream>
#include <algorithm>

#include "circuit_structure.hpp"
#include "symbolic.hpp"

void getCurrent(const Circuit::Schematic &circuit, Vector<float> &current)
{
    std::cout << "Current Vector: " << std::endl;
    std::for_each(circuit.sources.begin(), circuit.sources.end(), [&](const auto source){
        if (source->pos->id != -1) current[source->pos->id] += source->value;
        if (source->neg->id != -1) current[source->neg->id] -= source->value;
    });
    std::cout << current << std::endl;
}

void getConductance(const Circuit::Schematic &circuit, Matrix<float> &conductance){
    std::cout << "Conductance Matrix: " << std::endl;
    std::for_each(circuit.comps.begin(), circuit.comps.end(), [&](const auto comp){
        std::for_each(comp.second->nodes.begin(), comp.second->nodes.end(), [&](const auto node){
            if (node->id != -1) conductance[node->id][node->id] +=comp.second->conductance();
        });
        if(comp.second->nodes.size() == 2){
            if(comp.second->nodes[0]->id != -1 && comp.second->nodes[1]->id != -1){
                conductance[comp.second->nodes[0]->id][comp.second->nodes[1]->id] -= comp.second->conductance();
                conductance[comp.second->nodes[1]->id][comp.second->nodes[0]->id] -= comp.second->conductance();
                // std::cout << comp.second->nodes[0]->id << std::endl;
                // std::cout << comp.second->nodes[1]->id << std::endl;

            }
        }
    });
    std::cout << conductance << std::endl;
}

void generateMatrices(const Circuit::Schematic &circuit)
{
    const int NUM_NODES = circuit.nodes.size();
    Vector<float> current(NUM_NODES, 0.0);
    Matrix<float> conductance(NUM_NODES, NUM_NODES, 0.0);
    getCurrent(circuit, current);
    getConductance(circuit, conductance);

    Vector<float> voltage = conductance.inverse() * current;
    std::cout << "Voltage Vector: " << std::endl;
    std::cout << voltage  << std::endl;
    int i=0;
    for(auto node : circuit.nodes){
        node.second->voltage = voltage[i];
        i++;
    }
}

#endif