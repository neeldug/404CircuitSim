#include "../../include/circuits.hpp"
//TODO 
//Need to setup build correctly such that we don't need
//to include header files with relative paths

class Node {
private:
    std::string name;
    float voltage;
    std::vector< Component * > comps;
//TODO
// Needs to ad 
};

class Component {
private:
protected:
    // making constructor virutal makes
    // it obvious this is an abstract
    // base class
    Component();

    //either two or three connecting nodes in here
    std::vector< Node * > nodes;
    std::string name;

public:
    virtual ~Component();
};

class Circuit::Schematic {
    std::map< std::string, Node *> nodes;
    std::map< std::string, Component *> comps;

    // TODO 
    // Future development could be to serialise the
    // Schematic class to provide load/save functionality
    // with associated plots graphs and simulations, as well
    // as simulation setup parameters.

};