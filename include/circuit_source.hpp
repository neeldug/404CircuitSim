#ifndef GUARD_CIRCUIT_SOURCE_HPP
#define GUARD_CIRCUIT_SOURCE_HPP

#include <string>

#include "circuit_structure.hpp"

class Circuit::CurrentSource
{
// private:
//     const std::string name;
//     float value;
//     const Circuit::Node *pos;
//     const Circuit::Node *neg;

// NOTE - Made Everything public for now to avoid having to write getters and setters

public:
    const std::string name;
    float value;
    const Circuit::Node *pos;
    const Circuit::Node *neg;
    CurrentSource(const std::string &name, float value) : name(name), value(value), pos(nullptr), neg(nullptr) {}
    CurrentSource(const std::string &name, float value, const Circuit::Node *pos, const Circuit::Node *neg) : name(name), value(value), pos(pos), neg(neg) {}
};

#endif