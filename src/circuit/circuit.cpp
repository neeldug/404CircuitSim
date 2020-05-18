#include <iostream>
#include <string>
#include <cassert>
#include <circuit.hpp>

Circuit::Component::Component( std::string name, float value ){
    this->name = name;
    this->value = value;

    assert(value >= 0 && "Value of component can not be negative" );
}

Circuit::Component::~Component(){
    
}