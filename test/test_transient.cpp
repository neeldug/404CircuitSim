#include <iostream>
#include <fstream>
#include <circuit.hpp>

int main(){

	std::ifstream netlist;
	netlist.open("test/SpiceNetlists/transientExample.cir");
	if(netlist.fail()){
		std::cout<<"File Not Found!"<<std::endl;
		return 1;
	}
	Circuit::Schematic* schem = Circuit::Parser::parse( netlist );
    Circuit::Simulator* sim = new Circuit::Simulator(schem, Circuit::Simulator::SimulationType::TRAN, 5e-3, 0, 0.01e-3);
    schem->sims.push_back(sim);

    for(auto sims : schem->sims){
        sims->run();
    }

	netlist.close();
	delete schem;
}
