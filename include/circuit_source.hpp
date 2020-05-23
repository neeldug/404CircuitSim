#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>

#include "circuit_structure.hpp"

// NOTE - Value could be a function

class Circuit::Current : public Circuit::Source
{
public:
    Current(const std::string &name, float value) : Source(name, value) {}
    Current(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg)
        : Source(name, value, pos, neg) {}
    bool isCurrent() const override
    {
        return true;
    }
};

class Circuit::Voltage : public Circuit::Source
{
public:
    Voltage(const std::string &name, float value) : Source(name, value) {}
    Voltage(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg)
        : Source(name, value, pos, neg) {}
    bool isCurrent() const override
    {
        return false;
    }
};

#endif