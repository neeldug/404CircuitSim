// TODO - Implement Circuit Parser
// REVIEW - Maybe at some point make this all just a constructor of Schematic
#ifndef GUARD_CIRCUIT_PARSER_HPP
#define GUARD_CIRCUIT_PARSER_HPP

#include <iostream>
#include <set>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <regex>

class Circuit::Parser{
private:

	std::set<std::string> acceptedCommands{
			".step",
			".tran",
			".dc",
			".op"
			".model"
	};
	
	static double parseVal(const std::string &value ){
		
		std::size_t suffixPos =  value.find_first_not_of("0.123456789");
		if( suffixPos == std::string::npos ){
			return	std::stod( value );
		}
		std::string unitSuffix = value.substr(suffixPos, std::string::npos);
		int mult;

		if (unitSuffix == "p")
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
		return std::stod(num) * pow(10, mult);
	}

	template <class SourceType>
	static SourceType* sourceFactory( const std::string& line, Schematic *schem ) {
		static_assert(std::is_base_of<Circuit::Source, SourceType>::value, "Only derivates of source type maybe passed into this function"); 
		
		std::regex nameNodes(R"(^(\w+) (\w+) (\w+))");
		std::smatch nameNodesM;

		std::string name;
		std::string nodePos;
		std::string nodeNeg;
		if( regex_search( line, nameNodesM, nameNodes ) ){
			name = nameNodesM.str(1);
			nodePos = nameNodesM.str(2);
			nodeNeg = nameNodesM.str(3);
		}

		
		Symbolic timeVar{"t"};
		Symbolic function = 0;

		double DC = 0;
		double smallSignalAmp = 0;

		double SINE_DC_offset = 0;
		double SINE_amplitude = 0;
		double SINE_frequency = 0;

		//NOTE Small Signal value
		std::regex ac("AC (\\w+)");
		std::smatch acM;
		if( regex_search( line, acM, ac ) ){
			smallSignalAmp = parseVal( acM.str(1) );	
		}
		
		//DC value already variable safe although not implemented
		std::regex dc("(?:^(?:(?:\\w+ ){3}))(\\d+)");
		std::smatch dcM;
		if( regex_search( line, dcM, dc ) ){
			if( std::isdigit(dcM.str(1)[0])){
				DC = parseVal( dcM.str(1) );	
			}
		}

		//sine function variable safe although not implemented
		std::regex sine(R"(^(?:(?:\w+ ?){3}) (?:sine\s?\(|SINE\s?\()(?:\s?)(\d+) (\d+) (\d+))");
		std::smatch sineFunc;
		std::cout<<line<<std::endl;
		if( regex_search( line, sineFunc, sine ) ){
			if( std::isdigit(sineFunc.str(1)[0])){
				SINE_DC_offset = parseVal( sineFunc.str(1) );	
			}
			if( std::isdigit(sineFunc.str(2)[0])){
				SINE_amplitude = parseVal( sineFunc.str(2) );	
			}
			if( std::isdigit(sineFunc.str(3)[0])){
				SINE_frequency = parseVal( sineFunc.str(3) );	
			}
		}

		return new SourceType(name, DC, nodePos, nodeNeg, smallSignalAmp, SINE_DC_offset , SINE_amplitude,  SINE_frequency, schem );
	}

	static void addComponent( const std::string& comp, Circuit::Schematic* schem ){

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
				double value = stod( params[3] );
				Circuit::Resistor *r = new Circuit::Resistor( name, value, nodeA, nodeB, schem );
				break;
			}
			case (int) 'c' : {
				//TODO Would be helpful to say which capacitor has broken syntax rules
				assert( params.size() >= 4 && "Capacitor - too few params");
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				double value = stod( params[3] );
				if( params.size() >= 5 ){
					double DC_init = stod( params[4] );
					Circuit::Capacitor *r = new Circuit::Capacitor( name, value, nodeA, nodeB, schem, DC_init );
				}
				else{
					Circuit::Capacitor *c = new Circuit::Capacitor( name, value, nodeA, nodeB, schem );
				}
				break;
			}
			case (int) 'l' : {
				//TODO Would be helpful to say which indcutor has broken syntax rules
				assert( params.size() >= 4 && "Inductor - too few params");
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				double value = stod( params[3] );
				if( params.size() >= 5 ){
					double I_init = stod( params[4] );
					Circuit::Inductor *l = new Circuit::Inductor( name, value, nodeA, nodeB, schem, I_init );
				}
				else{
					Circuit::Inductor *l = new Circuit::Inductor( name, value, nodeA, nodeB, schem );
				}

				break;
			}
			case (int) 'v' : {
				assert( params.size() >= 4 && "Voltage - too few params" );
				Circuit::Voltage* volt = sourceFactory<Circuit::Voltage>( comp, schem );
				

				break;
			}
			case (int) 'i' : {	
				assert( params.size() >= 4 && "Voltage - too few params" );
				Circuit::Current* curr = sourceFactory<Circuit::Current>( comp, schem );

				break;
			}
			case (int) 'd' : {
				assert( params.size() >= 4 && "Diode - too few params" );
				std::string nodeA = params[1];
				std::string nodeB = params[2];
				std::string modName = params[3];

				Circuit::Diode* diode = new Circuit::Diode( name, nodeA, nodeB, modName, schem );
				break;
			}
			case (int) 'q' : {
				// Qname C B E BJT_modelName

				assert( params.size() >= 5 );
				std::string nodeCollector = params[1];
				std::string nodeBase = params[2];
				std::string nodeEmitter = params[3];
				std::string modelName = params[4];

				Circuit::Transistor* tran = new Circuit::Transistor( name, nodeCollector, nodeBase, nodeEmitter, modelName, schem );
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
	static void parseCommand( const std::string& comp, Circuit::Schematic* schem ){

	}
public:
	
	static Circuit::Schematic* parse( std::istream& inputStream ){
		//NOTE
		//Refer to
		//https://web.stanford.edu/class/ee133/handouts/general/spice_ref.pdf for
		//sytnax of SPICE files


		std::string inputLine;
		std::vector<std::string> netlist;

		Circuit::Schematic *schem = new Schematic();
		if( std::getline( inputStream, inputLine ) ){
			schem->title = inputLine;
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
