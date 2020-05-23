#include <circuit.hpp>
int main()
{
    Circuit::Schematic circuit;

    Circuit::Node *node1 = new Circuit::Node("N1", 0.0, circuit.id());
    Circuit::Node *node2 = new Circuit::Node("N2", 0.0, circuit.id());
    Circuit::Node *node3 = new Circuit::Node("N3", 0.0, circuit.id());
    Circuit::Node *node4 = new Circuit::Node("N4", 0.0, circuit.id());


    Circuit::Resistor *r1 = new Circuit::Resistor("R1", 1);
    Circuit::Resistor *r2 = new Circuit::Resistor("R2", 1);
    Circuit::Resistor *r3 = new Circuit::Resistor("R3", 1);
    Circuit::Resistor *r4 = new Circuit::Resistor("R4", 1);
    Circuit::Resistor *r5 = new Circuit::Resistor("R5", 1);
    Circuit::Resistor *r6 = new Circuit::Resistor("R6", 1);

    Circuit::Voltage *v1 = new Circuit::Voltage("V1", 5, node1, Circuit::Node::ground);
    Circuit::Voltage *v2 = new Circuit::Voltage("V2", 5, node3, node4);

    node1->comps.push_back(r1);
    node1->comps.push_back(r2);

    node2->comps.push_back(r2);
    node2->comps.push_back(r3);
    node2->comps.push_back(r4);

    node3->comps.push_back(r1);
    node3->comps.push_back(r3);
    node3->comps.push_back(r4);
    node3->comps.push_back(r5);

    node4->comps.push_back(r6);


    r1->nodes = {node1, node3};
    r2->nodes = {node1, node2};
    r3->nodes = {node2, node3};
    r4->nodes = {node2, node3};
    r5->nodes = {node3, Circuit::Node::ground};
    r6->nodes = {node4, Circuit::Node::ground};

    circuit.sources.push_back(v1);
    circuit.sources.push_back(v2);

    circuit.nodes["N1"] = node1;
    circuit.nodes["N2"] = node2;
    circuit.nodes["N3"] = node3;
    circuit.nodes["N4"] = node4;

    circuit.comps["R1"] = r1;
    circuit.comps["R2"] = r2;
    circuit.comps["R3"] = r3;
    circuit.comps["R4"] = r4;
    circuit.comps["R5"] = r5;
    circuit.comps["R6"] = r6;

    dc(circuit);
    circuit.out();
    return 0;
}