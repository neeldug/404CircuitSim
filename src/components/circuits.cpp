#include <iostream>
#include <string>
#include "circuits.hpp"
#include <maths/symbolic/symbolicc++.h>

Circuit::Component::Component( std::string name, float value ){
    this->name = name;
    this->value = value;
}
