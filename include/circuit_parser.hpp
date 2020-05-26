// TODO - Implement Circuit Parser
// REVIEW - Maybe at some point make this all just a constructor of Schematic
#ifndef GUARD_CIRCUIT_PARSER_HPP
#define GUARD_CIRCUIT_PARESR_HPP

#include "circuit.hpp"
#include <iostream>
#include <set>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath>

class Parser{
private:
	std::set<std::string> acceptedCommands = {
			".step",
			".tran",
			".dc",
			".model"
	};
	//NOTE converts statements like 12pf to 12e-12
	static float parseVal(const std::string &value ){
		std::string::size_t suffixPos =  value.find_first_not_of("0.123456789");
		std::string unitSuffix = value.substr(suffixPos, string::npos);
		int mult;
		if( suffixPos == std::string::npos ){
			return	std::stof( value );
		}
		else if (unitSuffix == "p")
			mult = -12;
		else if (unitSuffix == "n")
			mult = -9;
		else if (unitSuffix == "u")
			mult = -6;
		else if (unitSuffix == "m")
			mult = -3;
		else if (unitSuffix == "k")
			mult = 3;
		else if (unitSuffix == "Meg")
			mult = 6;
		else if (unitSuffix == "G")
			mult = 9;
		else {
			std::cerr << "Invalid Unit Suffix" << '\n';
			assert(0);
		}
		std::string num = value.substr(0, suffixPos);
		return std::stof(num) * pow(10, mult);
	}
	static void addComponent( const std::string& comp, Circuit::Schematic& schem ){

		std::stringstream ss( comp );
		std::vector<std::string> params;
		std::string param;
		while(ss>>param){
			params.push_back(param);
		}
		if(params.size() == 0 ){
			return;
		}
		std::string name = params[0];

		//NOTE has to be int for switch but basically comparing chars
		int component = (int) std::tolower( name[0] );


		switch( component ){
			case (int) 'r' : {
				//TODO Would be helpful to say which resistor has broken syntax rules
				assert( params.size() >= 4 && "Resistor - too few params" );
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				float value = stof( params[3] );
				Circuit::Resistor *r = new Circuit::Resistor( name.substr( 1, name.size() - 1 ), value, nodeA, nodeB, schem );
				break;
			}
			case (int) 'c' : {
				//TODO Would be helpful to say which capacitor has broken syntax rules
				assert( params.size() >= 4 && "Capacitor - too few params");
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				float value = stof( params[3] );
				if( params.size() >= 5 ){
					float DC_init = stof( params[4] );
					Circuit::Capacitor *r = new Circuit::Capacitor( name.substr( 1, name.size() - 1 ), value, nodeA, nodeB, schem, DC_init );
				}
				else{
					Circuit::Capacitor *c = new Circuit::Capacitor( name.substr( 1, name.size() - 1 ), value, nodeA, nodeB, schem );
				}
				break;
			}
			case (int) 'l' : {
				//TODO Would be helpful to say which indcutor has broken syntax rules
				assert( params.size() >= 4 && "Inductor - too few params");
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				float value = stof( params[3] );
				if( params.size() >= 5 ){
					float I_init = stof( params[4] );
					Circuit::Inductor *l = new Circuit::Inductor( name.substr( 1, name.size() - 1 ), value, nodeA, nodeB, schem, I_init );
				}
				else{
					Circuit::Inductor *l = new Circuit::Inductor( name.substr( 1, name.size() - 1 ), value, nodeA, nodeB, schem );
				}

				break;
			}
			case (int) 'v' : {
				//REVIEW This isn't finished
				assert( params.size() >= 4 && "Voltage - too few params" );
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				std::string value = params[3];

				//Circuit::Source* source = new Source( name, valu);
				break;
			}
			case (int) 'd' : {
				//REVIEW This isn't finished
				assert( params.size() >= 4 && "Diode - too few params" );
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				std::string value = params[3];

				//Circuit::Source* source = new Source( name, valu);
				break;
			}
			case (int) 'q' : {
				std::cout<<"not implemented yet"<<std::endl;
				break;
			}
			case (int) 'm' : {
				std::cout<<"not implemented yet"<<std::endl;
				break;
			}
			case (int) 'e' :{
				std::cout<<"not implemented yet"<<std::endl;
			}
			case (int) 'g' : {
				std::cout<<"not implemented yet"<<std::endl;
				break;
			}
			case (int) 'h' : {
				std::cout<<"not implemented yet"<<std::endl;
				break;
			}
			case (int) 'f' : {
				std::cout<<"not implemented yet"<<std::endl;
				break;
			}
			default : {
				break;
			}
		}

	}
	static void parseCommand( const std::string& comp, Circuit::Schematic& schem ){

	}
public:
	static Circuit::Schematic parse( std::istream& inputStream ){
		//NOTE
		//Refer to
		//https://web.stanford.edu/class/ee133/handouts/general/spice_ref.pdf for
		//sytnax of SPICE files


		std::string inputLine;
		std::vector<std::string> netlist;

		Circuit::Schematic schem;
		if( std::getline( inputStream, inputLine ) ){
			schem.title = inputLine;
		}
		bool endStatement = false;
		while( std::getline( inputStream, inputLine )){
			if( inputLine == ".END" || inputLine == ".end" ){
				endStatement = true;
				break;
			}
			if( inputLine[0] != '*' || inputLine[0] != '.'){
				netlist.push_back( inputLine );
				addComponent( inputLine, schem );
			}
			if( inputLine[0] == '.'){

			}
		}
		assert( endStatement && "No end statement present in netlist");
		return schem;
	}
};


#endif
