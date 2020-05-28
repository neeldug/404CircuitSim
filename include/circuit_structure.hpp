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
    class Parser;
} // namespace Circuit

class Circuit::Schematic
{
private:
    Circuit::Node *ground;

    std::function<int()> createIDGenerator(int &start) const
    {
        return [&]() {
            return start++;
        };
    }
    int start = 0;
    void setupConnectionNode( Circuit::Component *linear, std::string node);
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
    void setupConnections3Node( Circuit::Component *linear, std::string nodeA, std::string nodeB, std::string nodeC );
};

class Circuit::Node
{
private:
    std::string name;

public:
    Node(const std::string& name) : name(name) {
    }
	Node(const std::string &name, float voltage, int id) : id(id), name(name), voltage(voltage) {
    }

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


class Circuit::Component
{
protected:
    float value;
    Schematic *schem;
    Component( std::string name, float value, Schematic* schem ) : name(name), value(value), schem(schem) {}

public:
    std::string name;
    std::vector<Node *> nodes;

    virtual float conductance() const{
        return 0.0;
    };

    Node* getPosNode(){
        return nodes[0];
    }
    Node* getNegNode(){
        return nodes[1];
    }

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
    virtual float getValue(){
        return value;
    };
};


class Circuit::Schematic::Simulation
{
private:
    struct ParamTable
    {
        std::map<std::string, float> lookup;
    };
    std::vector<ParamTable *> tables;

public:
    enum SimulationType{
        OP, TRAN,  DC, SMALL_SIGNAL
    };
    const SimulationType type;

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
    Simulation( SimulationType type ) : type(type){

    }
    void run();

};  

void Circuit::Schematic::setupConnectionNode( Circuit::Component *linear, std::string node ){
    std::map<std::string, Circuit::Node *>::iterator it = this->nodes.find(node);

    if (it == this->nodes.end())
    {
        Circuit::Node *a = new Circuit::Node(node);
        it = this->nodes.insert(std::pair<std::string, Circuit::Node *>(node, a)).first;
    }

    Circuit::Node *a = (*it).second;

    a->comps.push_back(linear);
    this->comps.insert(std::pair<std::string, Circuit::Component *>(linear->name, linear));
    linear->nodes.push_back(a);
}

void Circuit::Schematic::setupConnections2Node( Circuit::Component *linear, std::string nodeA, std::string nodeB )
{
    setupConnectionNode(linear, nodeA);
    setupConnectionNode(linear, nodeB);
}
void Circuit::Schematic::setupConnections3Node( Circuit::Component *linear, std::string nodeA, std::string nodeB, std::string nodeC ){
    setupConnectionNode(linear, nodeA);
    setupConnectionNode(linear, nodeB);
    setupConnectionNode(linear, nodeC);
}

Circuit::Schematic::Schematic() : id(createIDGenerator(start)) {
    ground = new Node("0", 0.0, -1);
    nodes.insert(std::pair< std::string, Node *> (ground->getName(), ground));
}

#endif