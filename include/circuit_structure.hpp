#ifndef GUARD_CIRCUIT_STRUCTURE_HPP
#define GUARD_CIRCUIT_STRUCTURE_HPP

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
#include <functional>
#include <algorithm>
#include <typeinfo>
#include <iostream>

namespace Circuit
{
	class Component;
	class Capacitor;
	class Inductor;
	class Resistor;
	class Transistor;
	class Mosfet;
	class Diode;
	class Schematic;
	class Node;
	class Source;
	class Current;
	class Voltage;
	class Parser;
	class Simulator;
	class Math;
	struct ParamTable;
} // namespace Circuit

struct Circuit::ParamTable
{
	std::map<std::string, double> lookup;
};

class Circuit::Schematic
{
	friend class Simulator;

private:
	std::vector<ParamTable *> tables;
	std::function<int()> createIDGenerator(int &start) const
	{
		return [&]() {
			return start++;
		};
	}
	int start = -1;
	void setupConnectionNode( Circuit::Component *linear, const std::string &node);
	// std::map<std::string, Component*> tranMask;
	// void applyMask(){
	// 	for(std::pair<std::string, Component *> comp_pair : comps){
	// 		if(Resistor* res = dynamic_cast<Resistor *>(comp_pair.second)){
	// 			tranMask.insert(comp_pair);
	// 		}
	// 		else if(Capacitor *cap = dynamic_cast<Capacitor *>(comp_pair.second)){
	// 			// resistor
	// 			Resistor *res = new Resistor(comp_pair.first + "_R", , cap->getNegNode()->getName(), cap->getNegNode()->getName(), )
	// 			// current source
	// 		}
	// 	}
	// }
public:
	Schematic();
	std::function<int()> id;
	std::string title;
	std::map<std::string, Node *> nodes;
	std::map<std::string, Component *> comps;
	std::vector<Simulator *> sims = {};

	void out(ParamTable *param) const
	{
		for_each(nodes.begin(), nodes.end(), [&](const auto node) {
			node.second->print();
		});
		for_each(comps.begin(), comps.end(), [&](const auto comp) {
			comp.second->print(param);
		});
	}

	void setupConnections2Node( Circuit::Component *linear, const std::string& nodeA, const std::string& nodeB );
	void setupConnections3Node( Circuit::Component *linear, const std::string& nodeA, const std::string& nodeB, const std::string& nodeC );
	~Schematic();
};

class Circuit::Node
{
private:
	Schematic *schem;
	std::string name;
	int id;
public:
	double voltage;
	std::vector<Component *> comps;
	Node(const std::string& name, Schematic *schem) : name(name) {
		id = schem->id();
		voltage = 0.0;
		this->schem = schem;
	}

	void print() const
	{
		std::cout << "Node" << name << ":\t" << voltage << "V" << std::endl;
	}
	std::string getName() const{
		return name;
	}
	int getId() const{
		return id;
	}

	~Node(){
		assert(comps.size() == 0 && "Can't delete a Node with components");
		schem->nodes.erase(name);
	}
};


class Circuit::Component
{
protected:
	double i_prev = 0.0;
	double value;
	Schematic *schem;
	Component( const std::string& name, double value, Schematic* schem ) : name(name), value(value), schem(schem) {}
	bool variableDefined = false;
	std::string variableName; 
public:
	std::string name;
	std::vector<Node *> nodes;
	virtual double conductance(ParamTable * param) const{
		return 0.0;
	};

	Node* getPosNode() const{
		return nodes[0];
	}
	Node* getNegNode() const{
		return nodes[1];
	}
	virtual double getVoltage() const{
		return getPosNode()->voltage - getNegNode()->voltage;
	}
	virtual double current(ParamTable * param, double time=0, double timestep=0) const 
	{
		return (nodes[0]->voltage - nodes[1]->voltage) * conductance(param);
		// Direction of current
		// assuming node[0] is more positiveW
	}
	virtual void print(ParamTable * param) const
	{
		std::cout << typeid(*this).name() << name << ":\t" << current(param) << "A" << std::endl;
	}
	virtual ~Component()
	{
		//deleteFromSchematicMap();
		this->schem->comps.erase(name);
		
		//deleteFromAdjacentNodes();
		for(Node *n : nodes){
			n->comps.erase(std::find(n->comps.begin(), n->comps.end(), this));
			if(n->comps.size() == 0) delete n;
		}
		
	}
	virtual double getValue( ParamTable * param ) const {
		if( variableDefined ){
			return param->lookup[variableName];
		}
		else{
			return value;
		}
	};
	virtual bool isSource() const{
		return false;
	}
};

void Circuit::Schematic::setupConnectionNode( Circuit::Component *linear, const std::string &node ){


	//end add ground support

	std::map<std::string, Circuit::Node *>::iterator it = this->nodes.find(node);

	if (it == this->nodes.end())
	{
		Circuit::Node *a = new Circuit::Node(node, this);
		it = this->nodes.insert(std::pair<std::string, Circuit::Node *>(node, a)).first;
	}

	Circuit::Node *a = (*it).second;

	// into the node
	a->comps.push_back(linear);

	// this goes into the schematic
	this->comps.insert(std::pair<std::string, Circuit::Component *>(linear->name, linear));
	linear->nodes.push_back(a);
}

void Circuit::Schematic::setupConnections2Node( Circuit::Component *linear, const std::string& nodeA, const std::string& nodeB )
{
	setupConnectionNode(linear, nodeA);
	setupConnectionNode(linear, nodeB);
}
void Circuit::Schematic::setupConnections3Node( Circuit::Component *linear, const std::string& nodeA, const std::string& nodeB, const std::string& nodeC ){
	setupConnectionNode(linear, nodeA);
	setupConnectionNode(linear, nodeB);
	setupConnectionNode(linear, nodeC);
}

// don't need to insert ground into nodes
Circuit::Schematic::Schematic() : id(createIDGenerator(start)) {
	Node *ground = new Node("0", this );
	nodes.insert(std::pair< std::string, Node *> (ground->getName(), ground));
	// temp need a table to simulate
	ParamTable *param = nullptr;
	tables.push_back(param);
	// REVIEW - remove this
}

Circuit::Schematic::~Schematic(){
		std::for_each(tables.begin(), tables.end(),
					  [](ParamTable *&t) {
						  delete t;
					  });
		// std::for_each(nodes.begin(), nodes.end(), [](auto kv){
		// 	delete kv.second;
		// });
		
		


		//NOTE David thomas approved
		while(comps.size() != 0 ){
			delete comps.begin()->second;
		}

		std::for_each(sims.begin(), sims.end(), [](auto kv){
			delete kv;
		});
	}
#endif
