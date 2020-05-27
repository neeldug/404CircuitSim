#ifndef GUARD_CIRCUIT_LINEAR_HPP
#define GUARD_CIRCUIT_LINEAR_HPP

#include <string>
#include "circuit_structure.hpp"


class Circuit::Capacitor : public Component
{
public:
    //NOTE DC_init is starting DC voltage for transient analysis
    float DC_init;
    Capacitor(std::string name, float value) : Component(name, value){};
    Capacitor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem) : Component(name, value)
    {
        schem.setupConnections2Node( this, nodeA, nodeB );
    }
    Capacitor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem, float DC_init) : Capacitor(name, value, nodeA, nodeB, schem)
    {
        this->DC_init = DC_init;
    }
    float conductance() const override
    {
        // TODO
        return 1 / value;
    }
};

class Circuit::Inductor : public Component
{
public:
    //NOTE I_init is initial current in inductor
    float I_init;
    Inductor(std::string name, float value) : Component(name, value){};
    Inductor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem) : Component( name, value )
    {
        //REVIEW move node connections into compoment constructor
        schem.setupConnections2Node(this, nodeA, nodeB);
    }
    Inductor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem, float I_init) : Inductor(name, value, nodeA, nodeB, schem)
    {
        this->I_init = I_init;
    }
    float conductance() const override
    {
        // TODO
        return 1 / value;
    }
};

class Circuit::Resistor : public Component
{
public:
    Resistor(std::string name, float value) : Component(name, value){};
    Resistor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem) : Component(name, value)
    {
        schem.setupConnections2Node( this, nodeA, nodeB );
    };
    float conductance() const override
    {
        return 1 / value;
    }
};

#endif
