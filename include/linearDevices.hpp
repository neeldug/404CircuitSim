#ifndef GUARD_LINEAR_COMPS_HPP
#define GUARD_LINEAR_COMPS_HPP
#include <string>
#include "circuitStructure.hpp"

class Circuit::Capacitor : public Component{
public:
    Capacitor( std::string name, float value ) : Component( name, value ) {};
};

class Circuit::Inductor : public Component{
public:
    Inductor( std::string name, float value ) : Component( name, value ) {};
};

class Circuit::Resistor : public Component{
public:
    Resistor( std::string name, float value ) : Component( name, value ){};
};


#endif