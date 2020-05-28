#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>

class Circuit::Source : public Circuit::Component
{
protected:
	std::string name;
	Symbolic timeVar{"t"};
	Symbolic function = 0;
	
	float DC = 0;
	float smallSignalAmp = 0;

	float SINE_DC_offset = 0;
	float SINE_amplitude = 0;
	float SINE_frequency = 0;
	Source(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg, Schematic* schem) :  Component( name, value, schem ){
		DC = value;
	}
	Source( std::string name, float DC, float smallSignalAmp, float SINE_DC_offset ,float SINE_amplitude, float SINE_frequency, Schematic* schem ) :  Component( name,  DC, schem ){

		this->name = name;
		this->DC = DC;
		this->smallSignalAmp = smallSignalAmp;
		this->SINE_DC_offset = SINE_DC_offset;
		this->SINE_amplitude = SINE_amplitude;
		this->SINE_frequency = SINE_frequency;

		function =  (double)SINE_DC_offset + ( (double)  SINE_amplitude ) * sin( (double) SINE_frequency * timeVar );

	}

public:
	virtual float getValue() override{
		return DC;
	}
	virtual bool isCurrent() const = 0;
};

// NOTE - Value could be a function
class Circuit::Current : public Circuit::Source
{
public:

	Current( std::string name, float DC, std::string nodePos, std::string nodeNeg, float smallSignalAmp, float SINE_DC_offset ,float SINE_amplitude, float SINE_frequency, Schematic* schem ) :  Source( name,  DC, smallSignalAmp, SINE_DC_offset, SINE_amplitude, SINE_frequency, schem ){

		schem->setupConnections2Node( this, nodePos, nodeNeg );
	}
	Current( std::string name, float DC, std::string nodePos, std::string nodeNeg, Schematic *schem ) : Current(name, DC, nodePos,  nodeNeg, 0, 0 , 0, 0, schem ) {

	}
	bool isCurrent() const override
	{
		return true;
	}
};

class Circuit::Voltage : public Circuit::Source
{
public:
	
	Voltage( std::string name, float DC, std::string nodePos, std::string nodeNeg, float smallSignalAmp, float SINE_DC_offset ,float SINE_amplitude, float SINE_frequency, Schematic* schem ) :  Source( name,  DC, smallSignalAmp, SINE_DC_offset, SINE_amplitude, SINE_frequency, schem ){
		schem->setupConnections2Node( this, nodePos, nodeNeg );
	}
	Voltage( std::string name, float DC, std::string nodePos, std::string nodeNeg, Schematic *schem ) : Voltage(name, DC, nodePos,  nodeNeg, 0, 0 , 0, 0, schem ) {

	}
	bool isCurrent() const override
	{
		return false;
	}
};

#endif