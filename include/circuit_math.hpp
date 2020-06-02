#ifndef GUARD_CIRCUIT_MATH_HPP
#define GUARD_CIRCUIT_MATH_HPP

class Circuit::Math
{
private:
    static void init_matrix(Eigen::MatrixXd &mat, double val = 0.0)
    {
        for (size_t i = 0; i < mat.rows(); i++)
        {
            for (size_t j = 0; j < mat.cols(); j++)
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
        if (i != -1 && j != -1)
        {
            conductance(i, j) += val;
        }
    }

    static void handleCurrentSource(Eigen::VectorXd &current, int posId, int negId, double val)
    {
        addCurrentToVector(current, posId, val);
        addCurrentToVector(current, negId, -val);
    }

    static void handleVoltageSource(Eigen::MatrixXd &conductance, Eigen::VectorXd &current, int posId, int negId, double val)
    {
        Eigen::VectorXd new_conductance(current.rows());
        for (int i = 0; i < current.rows(); i++)
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
            assert(negId != -1 && "Both terminals cannot be connected to ground");
            new_conductance(negId) = -1.0;
            current[negId] = val;
            conductance.row(negId) = new_conductance;
            return;
        }
    }
    static void handleConductanceMatrixTwoNodes(Eigen::MatrixXd &conductance, int i, int j, double value)
    {
        addConductanceToMatrix(conductance, i, i, value);
        addConductanceToMatrix(conductance, j, j, value);
        addConductanceToMatrix(conductance, i, j, -value);
        addConductanceToMatrix(conductance, j, i, -value);
    }

public:
    static void getCurrentOP(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param);
    static void getCurrentTRAN(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step);
    static void getConductanceOP(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param);
    static void getConductanceTRAN(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step);
    static void init_vector(Eigen::VectorXd &vec, double val = 0.0)
    {
        for (size_t i = 0; i < vec.rows(); i++)
        {
            vec[i] = val;
        }
    }
    static double MSE(Eigen::VectorXd X, Eigen::VectorXd Y)
    {
        // double sum = 0;
        // assert(X.rows() == Y.rows() && "Old vector size not the same as new vector size");
        // size_t size = X.rows();
        // for (size_t i = 0; i < size; i++)
        // {
        //     sum += abs(X[i] - Y[i]);
        // }
        // double res = sum / (double)size;
        // return res;
        double dist;
        dist = (X - Y).norm();
        return dist;
    }
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

void Circuit::Math::getCurrentTRAN(Circuit::Schematic *schem, Eigen::VectorXd &current, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step)
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
        else if (Circuit::Diode *source = dynamic_cast<Circuit::Diode *>(comp.second))
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
            double value = comp_pair.second->getConductance(param, -1);
            handleConductanceMatrixTwoNodes(conductance, comp_pair.second->nodes[0]->getId(), comp_pair.second->nodes[1]->getId(), value);
        }
    });
}

void Circuit::Math::getConductanceTRAN(Circuit::Schematic *schem, Eigen::MatrixXd &conductance, Circuit::ParamTable *param, double t, double step)
{
    init_matrix(conductance);

    std::for_each(schem->comps.begin(), schem->comps.end(), [&](std::pair<std::string, Circuit::Component *> comp_pair) {
        if (!comp_pair.second->isSource())
        {
            if (t == 0)
            {
                step = 0;
            }
            double value = comp_pair.second->getConductance(param, step);
            handleConductanceMatrixTwoNodes(conductance, comp_pair.second->nodes[0]->getId(), comp_pair.second->nodes[1]->getId(), value);
        }
    });
}

#endif
