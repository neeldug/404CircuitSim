#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

/*
#include <iostream>
#include <algorithm>

#include "circuit_structure.hpp"
#include "symbolic.hpp"

void getCurrent(const Circuit::Schematic &circuit, Vector<double> &current, Matrix<double> &conductance)
{
    std::for_each(circuit.sources.begin(), circuit.sources.end(), [&](const auto source) {
        if (source->isCurrent())
        {
            if (source->getPosNode()->id != -1)
                current[source->getPosNode()->id] += source->getValue();
            if (source->getNegNode()->id != -1)
                current[source->getNegNode()->id] -= source->getValue();
        }
    });

    std::for_each(circuit.sources.begin(), circuit.sources.end(), [&](const auto source) {
        Vector<double> new_conductance(circuit.nodes.size(), 0.0);

        if (!source->isCurrent())
        {
            if (source->getPosNode()->id != -1)
                if (source->getPosNode()->id != -1)
                {
                    current[source->getPosNode()->id] = source->getValue();
                    new_conductance[source->getPosNode()->id] = 1.0;
                }

            if (source->getNegNode()->id != -1)
            {
                new_conductance[source->getNegNode()->id] = -1.0;
                if (source->getPosNode()->id == -1)
                {
                    current[source->getNegNode()->id] = source->getValue();
                    conductance[source->getNegNode()->id] = new_conductance;
                    return;
                }

                conductance[source->getNegNode()->id] = conductance[source->getPosNode()->id] + conductance[source->getNegNode()->id];
            }

            conductance[source->getPosNode()->id] = new_conductance;
        }
    });
}

void getConductance(const Circuit::Schematic &circuit, Matrix<double> &conductance)
{
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
}

void dc(const Circuit::Schematic &circuit)
{
    const int NUM_NODES = circuit.nodes.size();
    Vector<double> voltage;
    Vector<double> current(NUM_NODES, 0.0);
    Matrix<double> conductance(NUM_NODES, NUM_NODES, 0.0);

    getConductance(circuit, conductance);

    getCurrent(circuit, current, conductance);

    // std::cout << current << endl;
    // std::cout << conductance << endl;
    voltage = conductance.inverse() * current;

    for_each(circuit.nodes.begin(), circuit.nodes.end(), [&](const auto node_pair) {
        node_pair.second->voltage = voltage[node_pair.second->id];
    });

    // std::cout << "Voltage Vector: " << std::endl;
    // std::cout << voltage << std::endl;

}
*/
#endif