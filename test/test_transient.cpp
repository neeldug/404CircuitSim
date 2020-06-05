#include <iostream>
#include <fstream>
#include <circuit.hpp>

#include <filesystem>
namespace fs = std::filesystem;
int main()
{
	std::ifstream netlist;
	netlist.open("test/SpiceNetlists/phase.cir");
	if (netlist.fail())
	{
		std::cerr << "File Not Found! " << fs::current_path()<<" out"<< std::endl;
		return 1;
	}
	Circuit::Schematic *schem = Circuit::Parser::parse(netlist);
	schem->nonLinear = true;
	std::cerr<<"Time,Verr"<<std::endl;
	for (auto sims : schem->sims)
	{
		sims->run(std::cout, Circuit::Simulator::OutputFormat::CSV);
	}
	netlist.close();
	delete schem;
}
