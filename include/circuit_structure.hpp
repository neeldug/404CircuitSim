#ifndef GUARD_CIRCUIT_STRUCTURE_HPP
#define GUARD_CIRCUIT_STRUCTURE_HPP

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <functional>
#include <typeinfo>
#include <iostream>

// namespace Circut
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
	class CurrentSource;
	Schematic parse();
} // namespace Circuit


class Circuit::Schematic
{
private:
	class Simulation;
	std::vector<Simulation *> sims;

public:
	std::string title;
	std::map<std::string, Node *> nodes;
	std::map<std::string, Component *> comps;
	std::vector<CurrentSource *> sources;
	
	//NOTE Testing function to output contents of Schematic
	void dumpContents(){
		std::cout<<nodes.size()<<std::endl<<comps.size()<<std::endl;
		for( auto x : nodes ){
			std::cout<<typeid(x.second).name()<<" "<<x.first<<std::endl;
		}
		for( auto x : comps ){
			std::cout<<typeid(x.second).name()<<" "<<x.first<<std::endl;
		}
	}

	// TODO
	// Future development could be to serialise the
	// Schematic class to provide load/save functionality
	// with associated plots graphs and simulations, as well
	// as simulation setup parameters.
};

class Circuit::Node
{
public:
	static std::function<int()> createIncrementer(int &start)
	{
		return [&]() {
			return start++;
		};
	}

	
	Node(const std::string& name) : name(name) {}
	Node(const std::string &name, float voltage, int id) : id(id), name(name), voltage(voltage) {}
	int id;
	std::string name;
	float voltage;
	std::vector<Component *> comps;
};

class Circuit::Component
{
protected:
	// making constructor virutal makes
	// it obvious this is an abstract
	// base class

	//maybe value
	Component(std::string name, float value)
	{
		this->name = name;
		this->value = value;

		assert(value >= 0 && "Value of component cannot be negative");
	}


public:
	//either two or three connecting nodes in here
	std::vector<Node *> nodes;
	std::string name;
	float value;

	virtual ~Component() {}
};

class Circuit::Schematic::Simulation{
private:
	struct ParamTable{
		std::map<std::string, float> lookup;
	};
	std::vector<ParamTable*> tables;

public:
	~Simulation(){
		std::for_each(tables.begin(), tables.end(), 
			[](ParamTable* &t){
				delete t;
			}
		);
	}
	
	float getValue( int tableNum, std::string param ){
		assert( tables.size() > tableNum && "Attempted value retrieved from non-existant table.");
		return (tables[tableNum])->lookup[param];
	}
};


#endif