/*
#ifndef GUARD_CIRCUIT_SOURCE_DEPENDENT_HPP
#define GUARD_CIRCUIT_SOURCE_DEPENDENT_HPP

//NOTE Voltage/Current Dependent Current source
class Circuit::D_Current : public Circuit::Current{
public:

	enum ControlType{
		current,voltage
	};
	ControlType dependencyType;

	Component* CControl;
	Node* VControlA;
	Node* VControlB;

	//NOTE constructor for current controlled current source
	D_Current( const std::string& name, double factor, const std::string& nodeA, const std::string& nodeB, Component* ref, Schematic* schem ) : Current( name, 0, nodeA, nodeB, schem ){

		dependencyType = current;
		
	}
	//NOTE constructor for voltage controlled current source
	D_Current( const std::string& name, double factor, const std::string& nodeA, const std::string& nodeB, const std::string& refNodeA, const std::string refNodeB , Schematic* schem ) : Current( name, 0, nodeA, nodeB, schem ){

		dependencyType = voltage;
		
	}
	double getSourceOutput( ParamTable* param, double t ) override {
		if ( dependencyType == voltage ){
			VControlA.
		}
		else if ( dependencyType == current ){

		}
		else{
			assert( false && "Dependency Type not set");
		}
	}
};
#endif*/