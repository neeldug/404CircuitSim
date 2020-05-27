#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>
#include <complex>
#include <regex>
#include "circuit.hpp"

class Circuit::Source : public Circuit::Component
{
protected:
	const std::string name;
	Symbolic timeVar{"t"};
	Symbolic function = 0;

	float DC = 0;
	std::complex<float> AC_Phasor = 0;
	float smallSignalAmp = 0;

	float SINE_DC_offset = 0;
	float SINE_amplitude = 0;
	float SINE_frequency = 0;
	Source(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg, Schematic& schem) :  Component( name, value ){

	}
	Source( const std::string &name, const Circuit::Node *pos, const Circuit::Node *neg, std::string line, Schematic& schem ) :  Component( name,  0.0 ){
		while(line[0] == ' '){
			line.erase( 0,1 );
		}
		while( line.back() == ' '){
			line.pop_back();
		}
		//NOTE Small Signal value
		std::regex ac("AC (\\w+)");
		std::smatch acM;
		if( regex_search( line, acM, ac ) ){
			assert(m.size() <= 2 && "Syntax Error in Spice file - too many SMALL SIGNAL values");
			smallSignalAmp = Parser::parseVal( m.str(1) );
		}

		//DC value already variable safe
		std::regex dc("^(?:(?:\w+ ?){3}) (?:{?)([0-9]*|[a-z]*)+(?:}?)");
		std::smatch dcM;
		if( regex_search( line, dcM, ac ) ){
			if( std::isdigit(dcM.str(1)[0])){
				DC = Parser::parseVal( dcM.str(1) );
			}
		}

		//sine function
		std::regex sine(R"(^(?:(?:\w+ ?){3}(?:(?: sine)|(?: SINE))\(\s?)(\d+ )(\d+ )(\d+ ))");
		std::smatch sineFunc;
		if( regex_search( line, dcM, ac ) ){
			if( std::isdigit(dcM.str(1)[0])){
				SINE_DC_offset = Parser::parseVal( dcM.str(1) );
			}
			if( std::isdigit(dcM.str(2)[0])){
				SINE_DC_offset = Parser::parseVal( dcM.str(1) );
			}
			if( std::isdigit(dcM.str(3)[0])){
				SINE_DC_offset = Parser::parseVal( dcM.str(1) );
			}
		}

public:
	virtual bool isCurrent() const = 0;
};

/*
	Need a source value function of time
*/

// NOTE - Value could be a function
class Circuit::Current : public Circuit::Source
{
public:
	// Current(const std::string &name, float value) : Source(name, value) {}
	Current(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg)
		: Source(name, value, pos, neg) {}
	bool isCurrent() const override
	{
		return true;
	}
};

class Circuit::Voltage : public Circuit::Source
{
public:
	// Voltage(const std::string &name, float value) : Source(name, value) {}
	Voltage(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg)
		: Source(name, value, pos, neg) {}
	bool isCurrent() const override
	{
		return false;
	}
};

#endif
