#include "circuit_structure.hpp"
#include "circuit_math.hpp"

class Circuit::Simulator
{
private:
public:
    static void op(const Circuit::Schematic *schem)
    {
        const int NUM_NODES = schem->nodes.size();
        Vector<double> voltage;
        Vector<double> current(NUM_NODES, 0.0);
        Matrix<double> conductance(NUM_NODES, NUM_NODES, 0.0);

        Circuit::Math::getConductance(circuit, conductance);

        Circuit::Math::getCurrent(circuit, current, conductance);

        voltage = conductance.inverse() * current;

        for_each(circuit.nodes.begin(), circuit.nodes.end(), [&](const auto node_pair) {
            node_pair.second->voltage = voltage[node_pair.second->id];
        });

        schem->out();
    }
    static void tran(const Circuit::Schematic *schem)
    {
    }
};