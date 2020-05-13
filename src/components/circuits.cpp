#include <iostream>
#include <string>
#include "circuits.hpp"

Circuit::Component::Component( std::string name, float value ){
    this->name = name;
    this->value = value;
}