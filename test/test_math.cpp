#include <circuit.hpp>

// NOTE - I'm ignoring garbage collection for some reason

void voltage_sources()
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
}

void current_sources()
{
    Circuit::Schematic circuit;

    //should make ground node a special node included in parsing
    Circuit::Node *node1 = new Circuit::Node("N1", 0.0, circuit.id());
    Circuit::Node *node2 = new Circuit::Node("N2", 0.0, circuit.id());
    Circuit::Node *node3 = new Circuit::Node("N3", 0.0, circuit.id());

    Circuit::Resistor *r1 = new Circuit::Resistor("r1", 4);
    Circuit::Resistor *r2 = new Circuit::Resistor("r2", 3);
    Circuit::Resistor *r3 = new Circuit::Resistor("r3", 6);
    Circuit::Resistor *r4 = new Circuit::Resistor("r4", 5);
    Circuit::Resistor *r5 = new Circuit::Resistor("r5", 8);
    Circuit::Resistor *r6 = new Circuit::Resistor("r6", 6);

    Circuit::Current *cs1 = new Circuit::Current("cs1", 6, node1, Circuit::Node::ground);
    Circuit::Current *cs2 = new Circuit::Current("cs2", 4, node3, node1);

    node1->comps.push_back(r1);
    node1->comps.push_back(r2);
    node1->comps.push_back(r6);

    node2->comps.push_back(r2);
    node2->comps.push_back(r3);
    node2->comps.push_back(r4);

    node3->comps.push_back(r4);
    node3->comps.push_back(r5);
    node3->comps.push_back(r6);

    r1->nodes = {Circuit::Node::ground, node1};
    r2->nodes = {node1, node2};
    r3->nodes = {Circuit::Node::ground, node2};
    r4->nodes = {node2, node3};
    r5->nodes = {Circuit::Node::ground, node3};
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

    dc(circuit);
    circuit.out();
}

void neg_voltage_source()
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

    Circuit::Voltage *v1 = new Circuit::Voltage("V1", 5, Circuit::Node::ground, node1);
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
}

void mult_current_voltage()
{
    // Problem Sheet 2 - Question Five
    // 60V connected to Node 4
    // Middle Node is Node 3
    // Vx is Node 2
    // 240V connected to Node 1
    Circuit::Schematic circuit;

    Circuit::Node *node1 = new Circuit::Node("N001", 0.0, circuit.id());
    Circuit::Node *node2 = new Circuit::Node("X", 0.0, circuit.id());
    Circuit::Node *node3 = new Circuit::Node("N003", 0.0, circuit.id());
    Circuit::Node *node4 = new Circuit::Node("N004", 0.0, circuit.id());

    Circuit::Resistor *r1 = new Circuit::Resistor("R1", 3e3);
    Circuit::Resistor *r2 = new Circuit::Resistor("R2", 6e3);
    Circuit::Resistor *r3 = new Circuit::Resistor("R3", 24e3);
    Circuit::Resistor *r4 = new Circuit::Resistor("R4", 12e3);

    Circuit::Current *i1 = new Circuit::Current("I1", 10e-3, node4, node2);
    Circuit::Voltage *v2 = new Circuit::Voltage("V2", 60, node4, Circuit::Node::ground);
    Circuit::Voltage *v1 = new Circuit::Voltage("V1", 240, node1, Circuit::Node::ground);

    node1->comps.push_back(r1);

    node2->comps.push_back(r1);
    node2->comps.push_back(r2);

    node4->comps.push_back(r4);

    r1->nodes = {node1, node2};
    r2->nodes = {node2, node3};
    r3->nodes = {node3, Circuit::Node::ground};
    r4->nodes = {node3, node4};

    // need to separate voltage and current sources
    circuit.sources.push_back(v1);
    circuit.sources.push_back(v2);
    circuit.sources.push_back(i1);

    circuit.nodes["N1"] = node1;
    circuit.nodes["N2"] = node2;
    circuit.nodes["N3"] = node3;
    circuit.nodes["N4"] = node4;

    circuit.comps["R1"] = r1;
    circuit.comps["R2"] = r2;
    circuit.comps["R3"] = r3;
    circuit.comps["R4"] = r4;

    dc(circuit);
    circuit.out();

    // NOTE - use double, float not accurate enough, got wrong answers using float
}

void connected_voltage_sources()
{
    
}

int main()
{
    // current_sources();
    // voltage_sources();
    // neg_voltage_source();
    mult_current_voltage();

    return 0;
}