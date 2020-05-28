#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>

class Circuit::Source : public Circuit::Component
{
protected:
	std::string name;
	Symbolic timeVar{"t"};
	Symbolic function = 0;

	double DC = 0;
	double smallSignalAmp = 0;

	double SINE_DC_offset = 0;
	double SINE_amplitude = 0;
	double SINE_frequency = 0;

	Source( const std::string &name, double DC, double smallSignalAmp, double SINE_DC_offset ,double SINE_amplitude, double SINE_frequency, Schematic* schem ) :  Component( name,  DC, schem ){

		this->name = name;
		this->DC = DC;
		this->smallSignalAmp = smallSignalAmp;
		this->SINE_DC_offset = SINE_DC_offset;
		this->SINE_amplitude = SINE_amplitude;
		this->SINE_frequency = SINE_frequency;

		function =  (double) DC + (double)SINE_DC_offset + ( (double)  SINE_amplitude ) * sin( (double) SINE_frequency * timeVar );

	}
	Source(const std::string &name, double value, const Circuit::Node *pos, const Circuit::Node *neg, Schematic* schem) :  Source( name, value, 0, 0, 0, 0, schem ){
		
	}

public:
	double getSourceOutput( ParamTable* param, double t ){
		return function[timeVar==t];
	}
	virtual bool isSource() const override{
		return true;
	}
	virtual bool isCurrent() const = 0;
	virtual void print()const override {
		printf ("DC - %f | Small signal Amp - %f | ( %f + %f * sin( %f t ) ) \n", DC, smallSignalAmp, SINE_DC_offset, SINE_amplitude, SINE_frequency );
	}
};

/*
	Need a source value function of time
*/

// NOTE - Value could be a function
class Circuit::Current : public Circuit::Source
{
public:

	Current( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, double smallSignalAmp, double SINE_DC_offset ,double SINE_amplitude, double SINE_frequency, Schematic* schem ) :  Source( name,  DC, smallSignalAmp, SINE_DC_offset, SINE_amplitude, SINE_frequency, schem ){

		schem->setupConnections2Node( this, nodePos, nodeNeg );
	}
	Current( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, Schematic *schem ) : Current(name, DC, nodePos,  nodeNeg, 0, 0 , 0, 0, schem ) {

	}
	bool isCurrent() const override
	{
		return true;
	}
};

class Circuit::Voltage : public Circuit::Source
{
public:

	Voltage( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, double smallSignalAmp, double SINE_DC_offset ,double SINE_amplitude, double SINE_frequency, Schematic* schem ) :  Source( name,  DC, smallSignalAmp, SINE_DC_offset, SINE_amplitude, SINE_frequency, schem ){
		schem->setupConnections2Node( this, nodePos, nodeNeg );
	}
	Voltage( const std::string &name, double DC, const std::string &nodePos, const std::string &nodeNeg, Schematic *schem ) : Voltage(name, DC, nodePos,  nodeNeg, 0, 0 , 0, 0, schem ) {

	}
	bool isCurrent() const override
	{
		return false;
	}

};

#endif
