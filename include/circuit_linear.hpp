#ifndef GUARD_CIRCUIT_LINEAR_HPP
#define GUARD_CIRCUIT_LINEAR_HPP

#include <string>


class Circuit::Capacitor : public Component
{
public:
    //NOTE DC_init is starting DC voltage for transient analysis
    double DC_init;
    Capacitor(const std::string& name, double value, const std::string& nodeA, const std::string& nodeB, Schematic* schem) : Component(name, value, schem)
    {
        schem->setupConnections2Node( this, nodeA, nodeB );
    }
    Capacitor(const std::string& name, double value, const std::string& nodeA, const std::string& nodeB, Schematic* schem, double DC_init) : Capacitor(name, value, nodeA, nodeB, schem)
    {
        this->DC_init = DC_init;
    }
    double conductance() const override
    {
        // TODO
        return 1 / value;
    }
};

class Circuit::Inductor : public Component
{
public:
    //NOTE I_init is initial current in inductor
    double I_init;
    Inductor(const std::string& name, double value, const std::string& nodeA, const std::string& nodeB, Schematic* schem) : Component( name, value, schem )
    {
        //REVIEW move node connections into compoment constructor
        schem->setupConnections2Node(this, nodeA, nodeB);
    }
    Inductor(const std::string& name, double value, const std::string& nodeA, const std::string& nodeB, Schematic *schem, double I_init) : Inductor(name, value, nodeA, nodeB, schem)
    {
        this->I_init = I_init;
    }
    double conductance() const override
    {
        // TODO
        return 1 / value;
    }
};

class Circuit::Resistor : public Component
{
public:
    Resistor(const std::string& name, double value, const std::string& nodeA, const std::string& nodeB, Schematic* schem) : Component(name, value, schem)
    {
        schem->setupConnections2Node( this, nodeA, nodeB );
    };
    double conductance(ParamTable * param) const override
    {
        return 1 / getValue(param);
    }
};

#endif
