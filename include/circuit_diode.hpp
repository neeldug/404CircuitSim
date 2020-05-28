#ifndef GUARD_DIODE_HPP
#define GUARD_DIODE_HPP

class Circuit::Diode : public Component {
private:
	std::string modelName;
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
		IS = stof( params[0] );
		RS = stof( params[1] );
		CJO = stof( params[2] );
		TT = stof( params[3] );
		BV = stof( params[4] );
		IBV = stof( params[5] );

		value = IS;
	}
	double conductance() const override
    {
        assert( false && "No conductance for diode yet");
    }
	std::string getModelName(){
		return modelName;
	}
};

#endif
