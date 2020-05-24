#ifndef GUARD_CIRCUIT_STRUCTURE_HPP
#define GUARD_CIRCUIT_STRUCTURE_HPP

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
#include <functional>
#include <algorithm>

namespace Circuit
{
    // Base Classes
    class Schematic;
    class Node;
    class Component;
    class Source;

    // Components
    class Resistor;

    // Sources
    class Current;
    class Voltage;

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
    void out()
    {
        for_each(nodes.begin(), nodes.end(), [](const auto node) {
            node.second->print();
        });
        for_each(comps.begin(), comps.end(), [](const auto comp) {
            comp.second->print();
        });
        std::cout << std::endl;
        std::cout << std::endl;
    }
};

class Circuit::Node
{
private:
    std::string name;

public:
    static Node *ground;
    Node(const std::string &name, float voltage, int id)
        : id(id), name(name), voltage(voltage) {}
    int id;
    float voltage;
    std::vector<Component *> comps;
    void print()
    {
        std::cout << name << ":\t" << voltage << "V" << std::endl;
    }
};

Circuit::Node *Circuit::Node::ground = new Node("ground", 0.0, -1);
class Circuit::Component
{
protected:
    Component(std::string name, float value) : name(name), value(value) {}
    std::string name;
    float value;

public:
    std::vector<Node *> nodes;
    virtual float conductance() const = 0;
    float current()
    {
        // hardcoding for two nodes
        return (nodes[0]->voltage - nodes[1]->voltage) * conductance();
    }
    void print()
    {
        std::cout << name << ":\t" << current() << "A" << std::endl;
    }
    virtual ~Component() {}
};

// Value should be some type of time varying function
class Circuit::Source
{
protected:
    const std::string name;
    Source(const std::string &name, float value)
        : name(name), value(value), pos(nullptr), neg(nullptr) {}
    Source(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg)
        : name(name), value(value), pos(pos), neg(neg) {}

public:
    float value;
    const Circuit::Node *pos;
    const Circuit::Node *neg;
    virtual bool isCurrent() const = 0;
};

#endif