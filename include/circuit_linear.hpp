#ifndef GUARD_CIRCUIT_LINEAR_HPP
#define GUARD_CIRCUIT_LINEAR_HPP

#include <string>
#include "circuit_structure.hpp"

void generateLinear(Circuit::Component *linear, float value, std::string nodeA, std::string nodeB, Circuit::Schematic &schem)
{
    std::map<std::string, Circuit::Node *>::iterator itA = schem.nodes.find(nodeA);
    std::map<std::string, Circuit::Node *>::iterator itB = schem.nodes.find(nodeB);

    if (itA == schem.nodes.end())
    {
        Circuit::Node *a = new Circuit::Node(nodeA);
        itA = schem.nodes.insert(std::pair<std::string, Circuit::Node *>(nodeA, a)).first;
    }
    if (itB == schem.nodes.end())
    {
        Circuit::Node *b = new Circuit::Node(nodeB);
        itB = schem.nodes.insert(std::pair<std::string, Circuit::Node *>(nodeA, b)).first;
    }
    Circuit::Node *a = (*itA).second;
    Circuit::Node *b = (*itB).second;

    a->comps.push_back(linear);
    b->comps.push_back(linear);
    schem.comps.insert(std::pair<std::string, Circuit::Component *>(linear->name, linear));
    schem.nodes.insert(std::pair<std::string, Circuit::Node *>(linear->name, a));
    schem.nodes.insert(std::pair<std::string, Circuit::Node *>(linear->name, b));
}
class Circuit::Capacitor : public Component
{
public:
    //NOTE DC_init is starting DC voltage for transient analysis
    float DC_init;
    Capacitor(std::string name, float value) : Component(name, value){};
    Capacitor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem) : Component(name, value)
    {
        generateLinear(this, value, nodeA, nodeB, schem);
    };
    Capacitor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem, float DC_init) : Capacitor(name, value, nodeA, nodeB, schem)
    {
        this->DC_init = DC_init;
    };
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
    Inductor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem) : Component(name, value)
    {
        generateLinear(this, value, nodeA, nodeB, schem);
    };
    Inductor(std::string name, float value, std::string nodeA, std::string nodeB, Schematic &schem, float I_init) : Inductor(name, value, nodeA, nodeB, schem)
    {
        this->I_init = I_init;
    };
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
        generateLinear(this, value, nodeA, nodeB, schem);
    };
    float conductance() const override
    {
        return 1 / value;
    }
};

#endif