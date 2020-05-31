#ifndef GUARD_DIODE_HPP
#define GUARD_DIODE_HPP
#include <cmath>
class Circuit::Diode : public Component {
private:
	std::string modelName = "D";
	const double GMIN = 1e-10;
	const double V_T = 25e-3;
public:
	
	//REVIEW will probably have to make these doubles and might make this a nested class

	double IS=0.1; //also stored in value (Component base class)
	double RS=16;
	double CJO=2e-12;
	double TT=12e-9;
	double BV=100;
	double IBV=0.1e-12;

	Diode( std::string name, std::string nodeA, std::string nodeB, std::string model, Schematic* schem) : Component( name, IS, schem ){
		schem->setupConnections2Node( this, nodeA, nodeB );
	}
	void assignModel( std::vector<std::string> params ){
		//NOTE Remember to update component value!
		//REVIEW Maybe allow params to be variable dependent

		assert( params.size() == 6 && "Incorrect number of diode params" );
		IS = stod( params[0] );
		RS = stod( params[1] );
		CJO = stod( params[2] );
		TT = stod( params[3] );
		BV = stod( params[4] );
		IBV = stod( params[5] );

		value = IS;
	}
	double getConductance( ParamTable * param, double timestep ) const override
    {
		return GMIN;
    }
	double getCurrentSource( ParamTable *param, double time, double timestep, double vGuess ){
		double shockley;
		double exponentialBreakdown;
		shockley = IS*( exp(vGuess/V_T) - 1 );
		exponentialBreakdown = -IS*(exp(-(BV+vGuess)/V_T)-1 ) + BV/V_T;

		i_prev = ( shockley + exponentialBreakdown );
		return ( shockley + exponentialBreakdown );
	}
	double getCurrent( ParamTable *param, double time, double timestep) const override{
		return ( i_prev +  getVoltage()*getConductance( param, timestep ) );
	}
	std::string getModelName(){
		return modelName;
	}
};

#endif