#ifndef GUARD_CIRCUIT_SIMULATOR_HPP
#define GUARD_CIRCUIT_SIMULATOR_HPP

class Circuit::Simulator
{
private:
    Schematic *schem;

public:
    enum SimulationType
    {
        OP,
        TRAN,
        DC,
        SMALL_SIGNAL
    };

    const SimulationType type;

    Simulator(Schematic *schem, SimulationType type) : type(type), schem(schem) {}

    void run()
    {
        for (ParamTable *param : schem->tables)
        {
            if (type == OP)
            {
                const int NUM_NODES = schem->nodes.size() - 1;
                Vector<double> voltage(NUM_NODES, 0.0);
                Vector<double> current(NUM_NODES, 0.0);
                Matrix<double> conductance(NUM_NODES, NUM_NODES, 0.0);
                Circuit::Math::getConductance(schem, conductance, param);
                Circuit::Math::getCurrent(schem, current, conductance, param, 0.0);
                // std::cout << conductance << std::endl;
                // std::cout << current << std::endl;
                voltage = conductance.inverse() * current;
                // std::cout << voltage << std::endl;

                std::cout << "\t-----Operating Point-----\t\n\n";

                for_each(schem->nodes.begin(), schem->nodes.end(), [&](const auto node_pair) {
                    if (node_pair.second->getId() != -1)
                    {
                        node_pair.second->voltage = voltage[node_pair.second->getId()];
                        printf("V(%s)\t\t%f\tnode_voltage\n", node_pair.first.c_str(), node_pair.second->voltage);   
                    }
                });

                for_each(schem->comps.begin(), schem->comps.end(), [&](const auto comp_pair){
                    printf("I(%s)\t\t%f\tdevice_current\n", comp_pair.first.c_str(), comp_pair.second->current(param));
                });

            }
        }
    }
};
#endif
