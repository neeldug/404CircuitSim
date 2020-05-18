#include <circuit.hpp>
int main()
{
    Circuit::Schematic circuit;
    int start = 0;
    auto id = Circuit::Node::createIncrementer(start);

    //should make ground node a special node included in parsing
    Circuit::Node *ground = new Circuit::Node("ground", 0.0, -1);
    Circuit::Node *node1 = new Circuit::Node("node1", 0.0, id());
    Circuit::Node *node2 = new Circuit::Node("node2", 0.0, id());
    Circuit::Node *node3 = new Circuit::Node("node3", 0.0, id());

    Circuit::Resistor *r1 = new Circuit::Resistor("r1", 4);
    Circuit::Resistor *r2 = new Circuit::Resistor("r2", 3);
    Circuit::Resistor *r3 = new Circuit::Resistor("r3", 6);
    Circuit::Resistor *r4 = new Circuit::Resistor("r4", 5);
    Circuit::Resistor *r5 = new Circuit::Resistor("r5", 8);
    Circuit::Resistor *r6 = new Circuit::Resistor("r6", 6);

    Circuit::CurrentSource *cs1 = new Circuit::CurrentSource("cs1", 6, node1, ground);
    Circuit::CurrentSource *cs2 = new Circuit::CurrentSource("cs2", 4, node3, node1);

    node1->comps.push_back(r1);
    node1->comps.push_back(r2);
    node1->comps.push_back(r6);

    node2->comps.push_back(r2);
    node2->comps.push_back(r3);
    node2->comps.push_back(r4);

    node3->comps.push_back(r4);
    node3->comps.push_back(r5);
    node3->comps.push_back(r6);

    r1->nodes = {ground, node1};
    r2->nodes = {node1, node2};
    r3->nodes = {ground, node2};
    r4->nodes = {node2, node3};
    r5->nodes = {ground, node3};
    r6->nodes = {node1, node3};

    circuit.sources.push_back(cs1);
    circuit.sources.push_back(cs2);

    circuit.nodes["node1"] = node1;
    circuit.nodes["node2"] = node2;
    circuit.nodes["node3"] = node3;

    circuit.comps["r1"] = r1;
    circuit.comps["r2"] = r2;
    circuit.comps["r3"] = r3;
    circuit.comps["r4"] = r4;
    circuit.comps["r5"] = r5;
    circuit.comps["r6"] = r6;

    generateMatrices(circuit);

    return 0;
}