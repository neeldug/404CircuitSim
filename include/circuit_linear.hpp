#ifndef GUARD_CIRCUIT_LINEAR_HPP
#define GUARD_CIRCUIT_LINEAR_HPP

#include <limits>

//REVIEW possible refractor of extra abstract class between
//component and inductor/capacitor

class Circuit::Capacitor : public Component
{
protected:
    Capacitor()=default;
public:
    //NOTE DC_init is starting DC voltage for transient analysis
    double DC_init;
    Capacitor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem) : Component(name, value, schem)
    {
        schem->setupConnections2Node(this, nodeA, nodeB);
    }
    Capacitor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem, double DC_init) : Capacitor(name, value, nodeA, nodeB, schem)
    {
        this->DC_init = DC_init;
    }

    double getConductance(ParamTable *param, double timestep) const
    {
        double min_conductance = 1e-12;
        double max_conductance = 1e12;

        if (timestep == -1)
        {
            return min_conductance;
        }

        if (timestep == 0)
        {
            return max_conductance;
        }
        return value / timestep;
    }

    double getCurrentSource(ParamTable *param, double timestep)
    {
        double i_pres = getConductance(param, timestep) * getVoltage();
        i_prev = i_pres;
        return i_pres;
    }

    double getCurrent(ParamTable *param, double time, double timestep) const override
    {
        return (getVoltage()) * getConductance(param, timestep) - i_prev;
    }
};

class Circuit::Inductor : public Component
{
public:
    //NOTE I_init is initial current in inductor
    double I_init;
    Inductor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem) : Component(name, value, schem)
    {
        //REVIEW move node connections into compoment constructor
        schem->setupConnections2Node(this, nodeA, nodeB);
    }
    Inductor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem, double I_init) : Inductor(name, value, nodeA, nodeB, schem)
    {
        this->I_init = I_init;
    }

    //NOTE
    //If timesetp == -1 Simiulation mode op
    //REVIEW should probably change this
    double getConductance(ParamTable *param, double timestep) const
    {
        double min_conductance = 1e-12;
        double max_conductance = 1e12;

        if (timestep = -1)
        {
            return max_conductance;
        }
        if (timestep == 0)
        {
            return min_conductance;
        }
        return timestep / value;
    }

    double getCurrentSource(ParamTable *param, double timestep)
    {
        double i_pres = i_prev - getConductance(param, timestep) * getVoltage();
        i_prev = i_pres;
        return i_pres;
    }

    double getCurrent(ParamTable *param, double time, double timestep) const override
    {
        return (getVoltage()) * getConductance(param, timestep) - i_prev;
    }
};

class Circuit::Resistor : public Component
{
public:
    Resistor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem) : Component(name, value, schem)
    {
        schem->setupConnections2Node(this, nodeA, nodeB);
    }

    double getConductance(ParamTable *param, double time = 0) const override
    {
        return 1.0 / getValue(param);
    }
};

#endif
