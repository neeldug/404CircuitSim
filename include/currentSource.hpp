#ifndef GUARD_CURRENT_SOURCE_HPP
#define GUARD_CURRENT_SOURCE_HPP
#include <string>
#include "circuitStructure.hpp"

class Circuit::CurrentSource : public Circuit::Component {
public:
    CurrentSource( std::string name, float value ) : Component( name, value ) {};
};

#endif