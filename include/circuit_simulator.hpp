#ifndef GUARD_CIRCUIT_SIMULATOR_HPP
#define GUARD_CIRCUIT_SIMULATOR_HPP

class Circuit::Simulator
{
public:
    enum SimulationType
    {
        OP,
        TRAN,
        DC,
        SMALL_SIGNAL
    };

    const SimulationType type;

    Simulator(Schematic *schem, SimulationType type) : type(type)
    {
    }

    void run()
    {
        if (type == OP)
        {
            const int NUM_NODES = schem->nodes.size() - 1;
            Vector<double> voltage;
            Vector<double> current(NUM_NODES, 0.0);
            Matrix<double> conductance(NUM_NODES, NUM_NODES, 0.0);
            getConductance(circuit, conductance);

            getCurrent(circuit, current, conductance);
            voltage = conductance.inverse() * current;

            for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
                node_pair.second->voltage = voltage[node_pair.second->id];
            });
        }
    }
};
#endif
