#include <iostream>
#include <fstream>
#include <circuit.hpp>

int main(){

	std::ifstream netlist;
	netlist.open("test/SpiceNetlists/inductorExample.cir");
	if(netlist.fail()){
		std::cout<<"File Not Found!"<<std::endl;
		return 1;
	}
	Circuit::Schematic* schem = Circuit::Parser::parse( netlist );
    Circuit::Simulator* sim = new Circuit::Simulator(schem, Circuit::Simulator::SimulationType::TRAN, 1, 0, 1e-4);
    schem->out(NULL);
	schem->sims.push_back(sim);
	// for(auto comps : schem->comps){
	// 	comps.second->print( NULL );
	// }
    for(auto sims : schem->sims){
        sims->run(std::cout);
    }

	netlist.close();
	delete schem;
}
