#ifndef GUARD_DIODE_HPP
#define GUARD_DIODE_HPP

#include "circuit_structure.hpp"
class Circuit::Diode : public Component {
public:
	//REVIEW will probably have to make these doubles
	float IS=0.1; //also stored in value (Component base class)
	float RS=16;
	float CJO=2e-12;
	float TT=12e-9;
	float BV=100;
	float IBV=0.1e-12;

	Diode( std::string name, std::string nodeA, std::string nodeB ) : Component( name, IS ){
		
	}
	void assignModel( std::vector<std::string> params ){
		//NOTE Remember to update component value!
		//REVIEW Maybe allow params to be variable dependent

		assert( params.size() == 6 && "Incorrect number of diode params" );
		IS = stof( params[0] );
		RS = stof( params[1] );
		CJO = stof( params[2] );
		TT = stof( params[3] );
		BV = stof( params[4] );
		IBV = stof( params[5] );

		value = IS;
	}
	
};

#endif