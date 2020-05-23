#ifndef GUARD_CIRCUIT_STRUCTURE_HPP
#define GUARD_CIRCUIT_STRUCTURE_HPP

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
#include <functional>

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
    class Source;
    Schematic parse();
} // namespace Circuit

class Circuit::Schematic
{
private:
    std::function<int()> createIDGenerator(int &start) const
    {
        return [&]() {
            return start++;
        };
    }
    int start = 0;

public:
    Schematic() : id(createIDGenerator(start)) {}
    std::function<int()> id;
    std::map<std::string, Node *> nodes;
    std::map<std::string, Component *> comps;
    std::vector<Source *> sources;
    // void out();
    // TODO
    // Future development could be to serialise the
    // Schematic class to provide load/save functionality
    // with associated plots graphs and simulations, as well
    // as simulation setup parameters.
};

class Circuit::Node
{
private:
    std::string name;

public:
    Node(const std::string &name, float voltage, int id)
        : id(id), name(name), voltage(voltage) {}
    int id;
    float voltage;
    std::vector<Component *> comps;
    void print(){
        std::cout << name << ":\t" << voltage << std::endl;
    }
};

class Circuit::Component
{
protected:
    Component(std::string name, float value)
    {
        this->name = name;
        this->value = value;

        assert(value >= 0 && "Value of component can not be negative");
    }
    std::string name;
    float value;

public:
    //either two or three connecting nodes in here
    std::vector<Node *> nodes;
    virtual float conductance() const = 0;
    virtual ~Component() {}
};

#endif