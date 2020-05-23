#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

#include <iostream>
#include <algorithm>

#include "circuit_structure.hpp"
#include "symbolic.hpp"

void getCurrent(const Circuit::Schematic &circuit, Vector<float> &current, Matrix<float> &conductance)
{
    // std::cout << "Current Vector: " << std::endl;
    std::for_each(circuit.sources.begin(), circuit.sources.end(), [&](const auto source) {
        if (source->isCurrent())
        {
            if (source->pos->id != -1)
                current[source->pos->id] += source->value;
            if (source->neg->id != -1)
                current[source->neg->id] -= source->value;
        }
        else
        {
            Vector<float> new_conductance(circuit.nodes.size(), 0.0);
            if (source->pos->id != -1)
            {
                current[source->pos->id] = source->value;
                new_conductance[source->pos->id] = 1.0;
            }

            if (source->neg->id != -1)
            {
                if (source->pos->id == -1)
                {
                    // handle odd case
                }

                new_conductance[source->neg->id] = -1.0;
                conductance[source->neg->id] = conductance[source->pos->id] + conductance[source->neg->id];
            }

            conductance[source->pos->id] = new_conductance;
        }
    });
    // std::cout << current << std::endl;
}

void getConductance(const Circuit::Schematic &circuit, Matrix<float> &conductance)
{
    // std::cout << "Conductance Matrix: " << std::endl;
    std::for_each(circuit.comps.begin(), circuit.comps.end(), [&](const auto comp) {
        std::for_each(comp.second->nodes.begin(), comp.second->nodes.end(), [&](const auto node) {
            if (node->id != -1)
                conductance[node->id][node->id] += comp.second->conductance();
        });
        if (comp.second->nodes.size() == 2)
        {
            if (comp.second->nodes[0]->id != -1 && comp.second->nodes[1]->id != -1)
            {
                conductance[comp.second->nodes[0]->id][comp.second->nodes[1]->id] -= comp.second->conductance();
                conductance[comp.second->nodes[1]->id][comp.second->nodes[0]->id] -= comp.second->conductance();
            }
        }
    });
    // std::cout << conductance << std::endl;
}

void dc(const Circuit::Schematic &circuit)
{
    const int NUM_NODES = circuit.nodes.size();
    Vector<float> voltage;
    Vector<float> current(NUM_NODES, 0.0);
    Matrix<float> conductance(NUM_NODES, NUM_NODES, 0.0);

    getConductance(circuit, conductance);

    getCurrent(circuit, current, conductance);

    voltage = conductance.inverse() * current;

    for_each(circuit.nodes.begin(), circuit.nodes.end(), [&](const auto node_pair){
        node_pair.second->voltage = voltage[node_pair.second->id];
    });
    
    // std::cout << "Voltage Vector: " << std::endl;
    // std::cout << voltage << std::endl;
}

#endif