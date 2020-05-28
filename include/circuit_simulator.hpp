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
        }
    }
};

#endif
