#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

class Circuit::Math
{
private:
    static void init_vector(Eigen::VectorXd &vec, double val = 0.0)
    {
        for (size_t i = 0; i < vec.cols(); i++)
        {
            vec[i] = val;
        }
    }

    static void init_matrix(Eigen::MatrixXd &mat, double val = 0.0)
    {
        for (size_t i = 0; i < mat.rows(); i++)
        {
            for (size_t j = 0; j < mat.cols(); i++)
            {
                mat(i, j) = val;
            }
        }
    }

    static void addCurrentToVector(Eigen::VectorXd &current, int nodeId, double val)
    {
        if (nodeId != -1)
        {
            current[nodeId] += val;
        }
    }

    static void addConductanceToMatrix(Eigen::MatrixXd &conductance, int i, int j, double val)
    {
        conductance(i, j) += val;
    }

    static void handleCurrentSource(Eigen::VectorXd &current, int posId, int negId, double val)
    {
        addCurrentToVector(current, posId, val);
        addCurrentToVector(current, negId, -val);
    }

    static void handleVoltageSource(Eigen::MatrixXd &conductance, Eigen::VectorXd &current, int posId, int negId, double val)
    {
        Eigen::VectorXd new_conductance(current.cols());
        for (int i = 0; i < current.cols() - 1; i++)
        {
            new_conductance[i] = 0;
        }

        if (posId != -1)
        {
            new_conductance(posId) = 1.0;
            if (negId == -1)
            {
                conductance.row(posId) = new_conductance;
                current[posId] = val;
                return;
            }
            else
            {
                new_conductance(negId) = -1.0;
                conductance.row(negId) += conductance.row(posId);
                conductance.row(posId) = new_conductance;

                current[negId] += current[posId];
                current[posId] = val;
                return;
            }
        }
        else
        {
            assert(negId != -1 && "Both terminal cannot be connected to ground");
            new_conductance(negId) = -1.0;
            current[negId] = val;
            conductance.row(negId) = new_conductance;
            return;
        }
    }

    static void handleConductanceMatrix(const std::pair<std::string, Circuit::Component *> comp_pair, Eigen::MatrixXd &conductance, ParamTable *param, double t, double step)
    {
        if (!comp_pair.second->isSource())
        {
            // add conductance to leading diagonal
            std::for_each(comp_pair.second->nodes.begin(), comp_pair.second->nodes.end(), [&](Node *node) {
                if (node->getId() != -1)
                {
                    addConductanceToMatrix(conductance, node->getId(), node->getId(), comp_pair.second->getConductance(param, step == -1 ? step : t == 0 ? t : step));
                }
            });
            // if not a transistor
            if (comp_pair.second->nodes.size() == 2)
            {
                // if there is no connection to ground - subtract conductance from G12 and G21
                if (comp_pair.second->nodes[0]->getId() != -1 && comp_pair.second->nodes[1]->getId() != -1)
                {
                    addConductanceToMatrix(conductance, comp_pair.second->nodes[0]->getId(), comp_pair.second->nodes[1]->getId(), -comp_pair.second->getConductance(param, step == -1 ? step : t == 0 ? t : step));
                    addConductanceToMatrix(conductance, comp_pair.second->nodes[1]->getId(), comp_pair.second->nodes[0]->getId(), -comp_pair.second->getConductance(param, step == -1 ? step : t == 0 ? t : step));
                }
            }
            else
            {
                // NOTE - if it is a transistor (also worth checking if number of nodes is actually 3)
                assert(false && "Can't handle transistors for now. ABORT");
                exit(1);
            }
        }
    }

public:
    static void getCurrentOP(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param);
    static void getCurrentTRAN(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step);
    static void getConductanceOP(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param);
    static void getConductanceTRAN(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step);
};

void Circuit::Math::getCurrentOP(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param)
{
    init_vector(current);
    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (Circuit::Current *source = dynamic_cast<Circuit::Current *>(comp.second))
        {
            handleCurrentSource(current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->getSourceOutput(param, 0));
        }
        else if (Circuit::Capacitor *source = dynamic_cast<Circuit::Capacitor *>(comp.second))
        {
            handleCurrentSource(current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->opReplace->getSourceOutput(param, 0));
        }
    });

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (Circuit::Voltage *source = dynamic_cast<Circuit::Voltage *>(comp.second))
        {
            handleVoltageSource(conductance, current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->getSourceOutput(param, 0));
        }
        else if (Circuit::Inductor *source = dynamic_cast<Circuit::Inductor *>(comp.second))
        {
            handleVoltageSource(conductance, current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->opReplace->getSourceOutput(param, 0));
        }
    });
}

void Circuit::Math::getCurrentTRAN(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t = 0, double step = 0)
{
    init_vector(current);

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (Circuit::Current *source = dynamic_cast<Circuit::Current *>(comp.second))
        {
            handleCurrentSource(current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->getSourceOutput(param, t));
        }
        else if (Circuit::LC *source = dynamic_cast<Circuit::LC *>(comp.second))
        {
            handleCurrentSource(current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->getCurrentSource(param, step));
        }
    });

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp) {
        if (Circuit::Voltage *source = dynamic_cast<Circuit::Voltage *>(comp.second))
        {
            handleVoltageSource(conductance, current, source->getPosNode()->getId(), source->getNegNode()->getId(), source->getSourceOutput(param, t));
        }
    });
}

void Circuit::Math::getConductanceOP(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param)
{
    init_matrix(conductance);

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp_pair) {
        if (!comp_pair.second->isSource())
        {

            if (Circuit::LC *lc = dynamic_cast<Circuit::LC *>(comp_pair.second))
            {
                return;
            }

            handleConductanceMatrix(comp_pair, conductance, param, 0, -1);
        }
    });
}

void Circuit::Math::getConductanceTRAN(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step)
{
    init_matrix(conductance);

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp_pair) {
        if (!comp_pair.second->isSource())
        {
            handleConductanceMatrix(comp_pair, conductance, param, t, step);
        }
    });
}

#endif
