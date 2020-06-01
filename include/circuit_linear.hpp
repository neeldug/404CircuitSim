#ifndef GUARD_CIRCUIT_LINEAR_HPP
#define GUARD_CIRCUIT_LINEAR_HPP

#include <limits>

class Circuit::LC : public Circuit::Component
{
protected:
    LC()=default;
    LC(const std::string &name, double value, Circuit::Schematic *schem) : Component(name, value, schem) {}
public:
    virtual double getCurrentSource(ParamTable *param, double timestep) = 0;

    double getCurrent(ParamTable *param, double time, double timestep) const override
    {
        return (getVoltage()) * getConductance(param, timestep) - i_prev;
    }
    virtual ~LC(){};
};
class Circuit::Capacitor : public Circuit::LC
{
protected:
    Capacitor() = default;

public:
    //NOTE DC_init is starting DC voltage for transient analysis
    double DC_init;
    Current *opReplace;

    Capacitor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem) : LC(name, value, schem)
    {
        schem->setupConnections2Node(this, nodeA, nodeB);
        opReplace = new Current(schem);
    }
    Capacitor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem, double DC_init) : Capacitor(name, value, nodeA, nodeB, schem)
    {
        this->DC_init = DC_init;
    }

    virtual double getConductance(ParamTable *param, double timestep) const override
    {
        double min_conductance = 1e-13;
        double max_conductance = 1e13;

        if (timestep == 0)
        {
            return min_conductance;
        }
        return value / timestep;
    }

    virtual double getCurrentSource(ParamTable *param, double timestep) override
    {
        double i_pres = getConductance(param, timestep) * getVoltage();
        i_prev = i_pres;
        return i_pres;
    }
    virtual ~Capacitor(){
        delete opReplace;
    };
};

class Circuit::Inductor : public Circuit::LC
{
public:
    //NOTE I_init is initial current in inductor
    double I_init;
    Voltage *opReplace;
    Inductor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem) : LC(name, value, schem)
    {
        //REVIEW move node connections into compoment constructor
        schem->setupConnections2Node(this, nodeA, nodeB);
        opReplace = new Voltage(schem);
    }
    Inductor(const std::string &name, double value, const std::string &nodeA, const std::string &nodeB, Schematic *schem, double I_init) : Inductor(name, value, nodeA, nodeB, schem)
    {
        this->I_init = I_init;
    }

    //NOTE
    //If timesetp == -1 Simiulation mode op
    //REVIEW should probably change this
    double getConductance(ParamTable *param, double timestep) const override
    {
        double min_conductance = 1e-13;
        double max_conductance = 1e13;

        if (timestep == 0)
        {
            return max_conductance;
        }
        return timestep / value;
    }

    double getCurrentSource(ParamTable *param, double timestep) override
    {
        double i_pres = i_prev - getConductance(param, timestep) * getVoltage();
        i_prev = i_pres;
        return i_pres;
    }
    virtual ~Inductor(){
        delete opReplace;
    };
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
