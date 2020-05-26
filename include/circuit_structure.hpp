#ifndef GUARD_CIRCUIT_STRUCTURE_HPP
#define GUARD_CIRCUIT_STRUCTURE_HPP

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
#include <functional>
#include <algorithm>
#include <typeinfo>
#include <iostream>

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
    class Current;
    class Voltage;
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
    Schematic();
    std::function<int()> id;
    std::string title;
    std::map<std::string, Node *> nodes;
    std::map<std::string, Component *> comps;
    std::vector<Source *> sources;

    class Simulation;
    std::vector<Simulation *> sims;

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

    void setupConnections2Node( Circuit::Component *linear, std::string nodeA, std::string nodeB );

};

class Circuit::Node
{
private:
    std::string name;

public:
    static Node *ground;
    Node(const std::string& name) : name(name) {}
	Node(const std::string &name, float voltage, int id) : id(id), name(name), voltage(voltage) {}

    int id;
    float voltage;
    std::vector<Component *> comps;
    void print()
    {
        std::cout << "Node" << name << ":\t" << voltage << "V" << std::endl;
    }
    std::string getName(){
        return name;
    }
};

Circuit::Node *Circuit::Node::ground = new Node("ground", 0.0, -1);

class Circuit::Component
{
protected:
    Component(std::string name, float value) : name(name), value(value) {}
    float value;

public:
    std::string name;
    std::vector<Node *> nodes;
    virtual float conductance() const = 0;
    float current()
    {
		return (nodes[0]->voltage - nodes[1]->voltage) * conductance();
        // hardcoding for two nodes
    }
    void print()
    {
        std::cout << typeid(*this).name() << name << ":\t" << current() << "A" << std::endl;
    }
    virtual ~Component()
    {
        //deleteFromSchematicMap();
        //deleteFromAdjacentNodes();
    }
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

class Circuit::Schematic::Simulation
{
private:
    enum Type{
        DC, TRAN, SMALL_SIGNAL
    };
    Type simType;
    struct ParamTable
    {
        std::map<std::string, float> lookup;
    };
    std::vector<ParamTable *> tables;

public:
    ~Simulation()
    {
        std::for_each(tables.begin(), tables.end(),
                      [](ParamTable *&t) {
                          delete t;
                      });
    }

    float getValue(int tableNum, std::string param)
    {
        assert(tables.size() > tableNum && "Attempted value retrieved from non-existant table.");
        return (tables[tableNum])->lookup[param];
    }

};  

void Circuit::Schematic::setupConnections2Node( Circuit::Component *linear, std::string nodeA, std::string nodeB )
{
    std::map<std::string, Circuit::Node *>::iterator itA = this->nodes.find(nodeA);
    std::map<std::string, Circuit::Node *>::iterator itB = this->nodes.find(nodeB);

    if (itA == this->nodes.end())
    {
        Circuit::Node *a = new Circuit::Node(nodeA);
        itA = this->nodes.insert(std::pair<std::string, Circuit::Node *>(nodeA, a)).first;
    }
    if (itB == this->nodes.end())
    {
        Circuit::Node *b = new Circuit::Node(nodeB);
        itB = this->nodes.insert(std::pair<std::string, Circuit::Node *>(nodeB, b)).first;
    }
    Circuit::Node *a = (*itA).second;
    Circuit::Node *b = (*itB).second;

    a->comps.push_back(linear);
    b->comps.push_back(linear);
    this->comps.insert(std::pair<std::string, Circuit::Component *>(linear->name, linear));
    linear->nodes.push_back(a);
    linear->nodes.push_back(b);
}

Circuit::Schematic::Schematic() : id(createIDGenerator(start)) {
    nodes.insert(std::pair< std::string, Node *> (Node::ground->getName(), Node::ground));
}

#endif