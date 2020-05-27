// #ifndef GUARD_CIRCUIT_TRANSISTOR_HPP
// #define GUARD_CIRCUIT_TRANSISTOR_HPP
//
// #include "circuit_structure.hpp"
//
// class Circuit::Transistor : public Component {
// private:
// 	std::string modelName;
// public:
// 	//REVIEW will probably have to make these doubles
// 	float BF=0.1; //also stored in value (Component base class)
// 	float IS=16;
// 	float VAF=2e-12;
//
// 	Diode( std::string name, std::string nodeA, std::string nodeB, std::string model, Schematic& schem) : Component( name, IS ){
// 		schem.setupConnections2Node( this, nodeA, nodeB );
// 	}
// 	void assignModel( std::vector<std::string> params ){
// 		//NOTE Remember to update component value!
// 		//REVIEW Maybe allow params to be variable dependent
//
// 		assert( params.size() == 6 && "Incorrect number of diode params" );
// 		IS = stof( params[0] );
// 		RS = stof( params[1] );
// 		CJO = stof( params[2] );
// 		TT = stof( params[3] );
// 		BV = stof( params[4] );
// 		IBV = stof( params[5] );
//
// 		value = IS;
// 	}
// 	float conductance() const override
//     {
//         assert( false && "No conductance for diode yet");
//     }
// 	std::string getModelName(){
// 		return modelName;
// 	}
// };
//
// #endif
