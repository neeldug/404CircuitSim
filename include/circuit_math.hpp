#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP


#include <iostream>
#include <algorithm>

#include "circuit_structure.hpp"
#include "symbolic.hpp"

class Circuit::Math
{
public:
    void getCurrent(Circuit::Schematic *schem, Vector<double> &current, Matrix<double> &conductance, Circuit::ParamTable *param, double t);
    void getConductance(Circuit::Schematic *schem, Matrix<double> &conductance);
};

    void getCurrent(Circuit::Schematic *schem, Vector<double> &current, Matrix<double> &conductance, Circuit::ParamTable *param, double t)
{
    std::for_each(schem->sources.begin(), schem->sources.end(), [&](const auto source) {
        if (source->isCurrent())
        {
            if (source->getPosNode()->getId() != -1)
                current[source->getPosNode()->getId()] += source->getSourceOutput(param, t);
            if (source->getNegNode()->getId() != -1)
                current[source->getNegNode()->getId()] -= source->getSourceOutput(param, t);
        }
    });

    std::for_each(schem->sources.begin(), schem->sources.end(), [&](const auto source) {
        Vector<double> new_conductance(schem->nodes.size(), 0.0);

        if (!source->isCurrent())
        {
            if (source->getPosNode()->getId() != -1)
                if (source->getPosNode()->getId() != -1)
                {
                    current[source->getPosNode()->getId()] = source->getSourceOutput(param, t);
                    new_conductance[source->getPosNode()->getId()] = 1.0;
                }

            if (source->getNegNode()->getId() != -1)
            {
                new_conductance[source->getNegNode()->getId()] = -1.0;
                if (source->getPosNode()->getId() == -1)
                {
                    current[source->getNegNode()->getId()] = source->getSourceOutput(param, t);
                    conductance[source->getNegNode()->getId()] = new_conductance;
                    return;
                }

                conductance[source->getNegNode()->getId()] = conductance[source->getPosNode()->getId()] + conductance[source->getNegNode()->getId()];
            }

            conductance[source->getPosNode()->getId()] = new_conductance;
        }
    });
}

void getConductance(Circuit::Schematic *schem, Matrix<double> &conductance, ParamTable * param)
{
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](const auto comp) {
        std::for_each(comp.second->nodes.begin(), comp.second->nodes.end(), [&](const auto node) {
            if (node->getId() != -1)
                conductance[node->getId()][node->getId()] += comp.second->conductance(param);
        });
        if (comp.second->nodes.size() == 2)
        {
            if (comp.second->nodes[0]->getId() != -1 && comp.second->nodes[1]->getId() != -1)
            {
                conductance[comp.second->nodes[0]->getId()][comp.second->nodes[1]->getId()] -= comp.second->conductance(param);
                conductance[comp.second->nodes[1]->getId()][comp.second->nodes[0]->getId()] -= comp.second->conductance(param);
            }
        }
    });
}

#endif
