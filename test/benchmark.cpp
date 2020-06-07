#include <hayai.hpp>
#include <circuit.hpp>
#include <vector>
#include <fstream>

class SimulatorFixture
    : public ::hayai::Fixture
{

public:
    SimulatorFixture()
    {
    }
    virtual void SetUp()
    {
        netlist.open("test/SpiceNetlists/transientExample.cir");
        this->schem = Circuit::Parser::parse(netlist);
    }

    virtual void TearDown()
    {
        netlist.close();
        delete this->schem;
    }

    std::ifstream netlist;
    Circuit::Schematic *schem;
    static const size_t iterations = 1;
    static const size_t runs = 1000;
};

BENCHMARK_F(SimulatorFixture, run, runs, iterations)
{
    for (Circuit::Simulator *sim : schem->sims)
    {
        sim->run(std::cerr, Circuit::Simulator::OutputFormat::CSV);
    }
}

int main(int argc, char const *argv[])
{

    hayai::ConsoleOutputter consoleOutputter;

    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}