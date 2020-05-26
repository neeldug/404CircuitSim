#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>

#include "circuit_structure.hpp"

class Circuit::Source
{
protected:
    const std::string name;
    Source(const std::string &name, float value)
        : name(name), value(value), pos(nullptr), neg(nullptr) {}
    Source(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg)
        : name(name), value(value), pos(pos), neg(neg) {}

public:
    float value;
    const Circuit::Node *pos;
    const Circuit::Node *neg;
    virtual bool isCurrent() const = 0;
};

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