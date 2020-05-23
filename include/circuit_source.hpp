#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>

#include "circuit_structure.hpp"

class Circuit::Source
{

    const std::string name;
    const std::string type;

public:
    static std::string CURRENT_SOURCE;
    static std::string VOLTAGE_SOURCE;
    float value;
    const Circuit::Node *pos;
    const Circuit::Node *neg;
    bool isCurrent()
    {
        return type == CURRENT_SOURCE;
    }
    bool isVoltage()
    {
        return type == VOLTAGE_SOURCE;
    }
    Source(const std::string &name, const std::string &type, float value) : name(name), value(value), type(type), pos(nullptr), neg(nullptr) {}
    Source(const std::string &name, const std::string &type, float value, const Circuit::Node *pos, const Circuit::Node *neg) : name(name), type(type), value(value), pos(pos), neg(neg) {}
};
std::string Circuit::Source::CURRENT_SOURCE = "current source";
std::string Circuit::Source::VOLTAGE_SOURCE = "voltage source";
#endif