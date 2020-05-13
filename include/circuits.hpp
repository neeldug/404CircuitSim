#ifndef GUARD_CIRCUITS_HPP
#define GUARD_CIRCUITS_HPP

#include <vector>
#include <map>
#include <string>

namespace Circuit
{
    class Component;
    class Capacitor;
    class Inductor;
    class Resistor;
    class Transistor;
    class Mosfet;
    class Diode;
    class Schematic;
    class Node;
    Schematic parse();
} // namespace Circuit

class Circuit::Schematic
{

public:
    std::map<std::string, Node *> nodes;
    std::map<std::string, Component *> comps;
    void out();
    // TODO
    // Future development could be to serialise the
    // Schematic class to provide load/save functionality
    // with associated plots graphs and simulations, as well
    // as simulation setup parameters.
};

class Circuit::Node{
public:
    std::string name;
    float voltage;
    std::vector<Component *> comps;
};

class Circuit::Component
{
protected:
    // making constructor virutal makes
    // it obvious this is an abstract
    // base class
    Component(std::string name, float value);

public:
    //either two or three connecting nodes in here
    std::vector<Node *> nodes;
    std::string name;
    float value;
    virtual ~Component();
};

#endif