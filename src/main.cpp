#include <iostream>
#include <circuit.hpp>
#include <circuit_math.hpp>

void example()
{
    Symbolic x("x");
    std::cout << integrate(x + 1, x) << std::endl; // => 1/2*x^(2)+x
    Symbolic y("y");
    std::cout << df(y, x) << std::endl;              // => 0
    std::cout << df(y[x], x) << std::endl;           // => df(y[x],x)
    std::cout << df(exp(cos(y[x])), x) << std::endl; // => -sin(y[x])*df(y[x],x)*e^cos(y[x])
}

int main()
{
    example();

    Circuit::Schematic circuit;

    Circuit::CurrentSource *cs1 = new Circuit::CurrentSource("cs1", 6);
    Circuit::CurrentSource *cs2 = new Circuit::CurrentSource("cs2", 4);

    //should make ground node a special node included in parsing
    Circuit::Node *ground = new Circuit::Node("ground", 0.0);
    Circuit::Node *node1 = new Circuit::Node("node1", 0.0);
    Circuit::Node *node2 = new Circuit::Node("node2", 0.0);
    Circuit::Node *node3 = new Circuit::Node("node3", 0.0);

    Circuit::Resistor *r1 = new Circuit::Resistor("r1", 4);
    Circuit::Resistor *r2 = new Circuit::Resistor("r2", 3);
    Circuit::Resistor *r3 = new Circuit::Resistor("r3", 6);
    Circuit::Resistor *r4 = new Circuit::Resistor("r4", 5);
    Circuit::Resistor *r5 = new Circuit::Resistor("r5", 8);
    Circuit::Resistor *r6 = new Circuit::Resistor("r6", 6);

    node1->comps.push_back(r1);
    node1->comps.push_back(r2);
    node1->comps.push_back(r6);

    node2->comps.push_back(r2);
    node2->comps.push_back(r3);
    node2->comps.push_back(r4);

    node3->comps.push_back(r4);
    node3->comps.push_back(r5);
    node3->comps.push_back(r6);

    cs1->nodes = {ground, node1};
    cs2->nodes = {node1, node3};

    r1->nodes = {ground, node1};
    r2->nodes = {node1, node2};
    r3->nodes = {ground, node2};
    r4->nodes = {node2, node3};
    r5->nodes = {ground, node3};
    r6->nodes = {node1, node3};

    circuit.sources["cs1"] = cs1;
    circuit.sources["cs2"] = cs2;

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