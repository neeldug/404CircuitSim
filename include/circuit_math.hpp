#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

class Circuit::Math
{
public:
    static void getCurrent(Circuit::Schematic *schem, Vector<double> &current, Matrix<double> &conductance, Circuit::ParamTable *param, double t, double step);
    static void getConductance(Circuit::Schematic *schem, Matrix<double> &conductance, Circuit::ParamTable *param, double t, double step);
};

void Circuit::Math::getCurrent(Circuit::Schematic *schem, Vector<double> &current, Matrix<double> &conductance, Circuit::ParamTable *param, double t=0, double step=0)
{
    // helper function
    auto addCurrentToNode = [&](int nodeId, double cur) {
        if (nodeId != -1)
        {
            current[nodeId] += cur;
        }
    };
    // current sources
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (Circuit::Current *source = dynamic_cast<Circuit::Current *>(comp.second))
        {
            addCurrentToNode(source->getPosNode()->getId(), source->getSourceOutput(param, t));
            addCurrentToNode(source->getNegNode()->getId(), -source->getSourceOutput(param, t));
        }

        else if (Circuit::Capacitor *source = dynamic_cast<Circuit::Capacitor *>(comp.second))
        {
            addCurrentToNode(source->getPosNode()->getId(), source->getCurrentSource(param, step));
            addCurrentToNode(source->getNegNode()->getId(), -source->getCurrentSource(param, step));
        }

        else if (Circuit::Inductor *source = dynamic_cast<Circuit::Inductor *>(comp.second))
        {
            addCurrentToNode(source->getPosNode()->getId(), source->getCurrentSource(param, step));
            addCurrentToNode(source->getNegNode()->getId(), -source->getCurrentSource(param, step));
        }
    });

    // voltage - sources
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        // new conductance row
        if (Circuit::Voltage *source = dynamic_cast<Circuit::Voltage *>(comp.second))
        {
            Vector<double> new_conductance(schem->nodes.size() - 1, 0.0);
            if (source->getPosNode()->getId() != -1)
            {
                new_conductance[source->getPosNode()->getId()] = 1.0;
                if (source->getNegNode()->getId() == -1)
                {
                    conductance[source->getPosNode()->getId()] = new_conductance;
                    current[source->getPosNode()->getId()] = source->getSourceOutput(param, t);
                    return;
                }
                else
                {
                    new_conductance[source->getNegNode()->getId()] = -1.0;
                    conductance[source->getNegNode()->getId()] += conductance[source->getPosNode()->getId()];
                    conductance[source->getPosNode()->getId()] = new_conductance;

                    current[source->getNegNode()->getId()] += current[source->getPosNode()->getId()];
                    current[source->getPosNode()->getId()] = source->getSourceOutput(param, t);
                    return;
                }
            }
            else
            {
                assert(source->getNegNode()->getId() != -1 && "Both terminal cannot be connected to ground");
                new_conductance[source->getNegNode()->getId()] = -1.0;
                current[source->getNegNode()->getId()] = source->getSourceOutput(param, t);
                conductance[source->getNegNode()->getId()] = new_conductance;
                return;
            }
        }
    });
}

void Circuit::Math::getConductance(Circuit::Schematic *schem, Matrix<double> &conductance, Circuit::ParamTable *param, double t=0, double step=0)
{
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](const std::pair<std::string, Circuit::Component *> comp_pair) {
        if (!comp_pair.second->isSource())
        {
            // add conductance to leading diagonal
            std::for_each(comp_pair.second->nodes.begin(), comp_pair.second->nodes.end(), [&](const Node* node) {
                if (node->getId() != -1)
                {
                    conductance[node->getId()][node->getId()] += comp_pair.second->getConductance(param, step == -1 ? step : t == 0 ? t : step);
                }
            });
            // if not a transistor
            if (comp_pair.second->nodes.size() == 2)
            {
                // if there is no connection to ground - subtract conductance from G12 and G21
                if (comp_pair.second->nodes[0]->getId() != -1 && comp_pair.second->nodes[1]->getId() != -1)
                {
                    conductance[comp_pair.second->nodes[0]->getId()][comp_pair.second->nodes[1]->getId()] -= comp_pair.second->getConductance(param, step == -1 ? step : t == 0 ? t : step);
                    conductance[comp_pair.second->nodes[1]->getId()][comp_pair.second->nodes[0]->getId()] -= comp_pair.second->getConductance(param, step == -1 ? step : t == 0 ? t : step);
                }
            }
            else
            {
                // NOTE - if it is a transistor (also worth checking if number of nodes is actually 3)
                assert(false && "Can't handle transistors for now. ABORT");
                exit(1);
            }
        }
    });
}

#endif