#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

class Circuit::Math
{
public:
    static void getCurrent(Circuit::Schematic *schem, Vector<double> &current, Matrix<double> &conductance, Circuit::ParamTable *param, double t);
    static void getConductance(Circuit::Schematic *schem, Matrix<double> &conductance, Circuit::ParamTable *param);
};

void Circuit::Math::getCurrent(Circuit::Schematic *schem, Vector<double> &current, Matrix<double> &conductance, Circuit::ParamTable *param, double t)
{
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (comp.second->isSource())
        {
            Circuit::Source *source = static_cast<Circuit::Source *>(comp.second);
            if (source->isCurrent())
            {
                if (source->getPosNode()->getId() != -1)
                    current[source->getPosNode()->getId()] += source->getSourceOutput(param, t);
                if (source->getNegNode()->getId() != -1)
                    current[source->getNegNode()->getId()] -= source->getSourceOutput(param, t);
            }
        }
    });

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (comp.second->isSource())
        {
            Circuit::Source *source = static_cast<Circuit::Source *>(comp.second);
            Vector<double> new_conductance(schem->nodes.size()-1, 0.0);

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
        }
    });
}

void Circuit::Math::getConductance(Circuit::Schematic *schem, Matrix<double> &conductance, Circuit::ParamTable *param)
{
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](const auto comp) {
        if (!comp.second->isSource())
        {
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
        }
    });
}

#endif
