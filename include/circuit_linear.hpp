#ifndef GUARD_CIRCUIT_LINEAR_HPP
#define GUARD_CIRCUIT_LINEAR_HPP

#include <string>

#include "circuit_structure.hpp"

class Circuit::Capacitor : public Component
{
public:
    Capacitor(std::string name, float value) : Component(name, value){};
};

class Circuit::Inductor : public Component
{
public:
    Inductor(std::string name, float value) : Component(name, value){};
};

class Circuit::Resistor : public Component
{
public:
    Resistor(std::string name, float value) : Component(name, value){};
    float conductance() const override{
        return 1/value;
    }
};

#endif