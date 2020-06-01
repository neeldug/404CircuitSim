#include <iostream>
#include <fstream>
#include <circuit.hpp>

int main()
{
	std::ifstream netlist;
	netlist.open("test/SpiceNetlists/specExample.cir");
	if (netlist.fail())
	{
		std::cout << "File Not Found!" << std::endl;
		return 1;
	}
	Circuit::Schematic *schem = Circuit::Parser::parse(netlist);
	for (auto sims : schem->sims)
	{
		sims->run(std::cout, Circuit::Simulator::OutputFormat::CSV);
	}
	netlist.close();
	delete schem;
}
