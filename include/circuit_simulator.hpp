#ifndef GUARD_CIRCUIT_SIMULATOR_HPP
#define GUARD_CIRCUIT_SIMULATOR_HPP

class Circuit::Simulator
{
private:
    enum SimulationType
    {
        OP,
        TRAN,
        DC,
        SMALL_SIGNAL
    };

public:
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
