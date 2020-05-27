#ifndef GUARD_CIRCUIT_TRANSISTOR_HPP
#define GUARD_CIRCUIT_TRANSISTOR_HPP

#include "circuit_structure.hpp"

class Circuit::Transistor : public Component {
private:
	std::string modelName;
	enum TType{
		NPN, PNP
	};

public:
	//REVIEW will probably have to make these doubles
	float BF=100;
	float IS=1e-16;
	float VAF=std::numeric_limits<float>::max();
	bool model_CE_resistance = false;


	TType transistorType;
	Transistor( std::string name, std::string nodeCollector, std::string nodeBase, std::string nodeEmitter, std::string model, Schematic& schem) : Component( name, IS ){
		if( modelName == "NPN" ){
			transistorType = NPN;
		}
		if( modelName == "PNP" ){
			transistorType = NPN;
		}
		this->modelName = modelName;
		schem.setupConnections3Node( this, nodeCollector, nodeBase, nodeEmitter );
	}

	//REVIEW Model not setup
	void assignModel( std::vector<std::string> params ){
		//NOTE Remember to update component value!
		//REVIEW Maybe allow params to be variable dependent

		assert( params.size() == 6 && "Incorrect number of diode params" );
		value = IS;
	}
	float conductance() const override
    {
        assert( false && "No conductance for diode yet");
    }

	std::string getModelName(){
		return modelName;
	}
};

#endif
