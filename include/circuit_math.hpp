// #ifndef GUARD_CIRCUIT_MATH_HPP
// #define GUARD_CIRCUIT_MATH_HPP


// #include <iostream>
// #include <algorithm>

// #include "circuit_structure.hpp"
// #include "symbolic.hpp"

// class Circuit::Math
// {
// public:
//     void getCurrent(const Circuit::Schematic &circuit, Vector<double> &current, Matrix<double> &conductance);
//     void getConductance(const Circuit::Schematic &circuit, Matrix<double> &conductance);
// };

// void getCurrent(const Circuit::Schematic &circuit, Vector<double> &current, Matrix<double> &conductance)
// {
//     std::for_each(circuit.sources.begin(), circuit.sources.end(), [&](const auto source) {
//         if (source->isCurrent())
//         {
//             if (source->getPosNode()->getId() != -1)
//                 current[source->getPosNode()->getId()] += source->getValue();
//             if (source->getNegNode()->getId() != -1)
//                 current[source->getNegNode()->getId()] -= source->getValue();
//         }
//     });

//     std::for_each(circuit.sources.begin(), circuit.sources.end(), [&](const auto source) {
//         Vector<double> new_conductance(circuit.nodes.size(), 0.0);

//         if (!source->isCurrent())
//         {
//             if (source->getPosNode()->getId() != -1)
//                 if (source->getPosNode()->getId() != -1)
//                 {
//                     current[source->getPosNode()->getId()] = source->getValue();
//                     new_conductance[source->getPosNode()->getId()] = 1.0;
//                 }

//             if (source->getNegNode()->getId() != -1)
//             {
//                 new_conductance[source->getNegNode()->getId()] = -1.0;
//                 if (source->getPosNode()->getId() == -1)
//                 {
//                     current[source->getNegNode()->getId()] = source->getValue();
//                     conductance[source->getNegNode()->getId()] = new_conductance;
//                     return;
//                 }

//                 conductance[source->getNegNode()->getId()] = conductance[source->getPosNode()->getId()] + conductance[source->getNegNode()->getId()];
//             }

//             conductance[source->getPosNode()->getId()] = new_conductance;
//         }
//     });
// }

// void getConductance(const Circuit::Schematic &circuit, Matrix<double> &conductance)
// {
//     std::for_each(circuit.comps.begin(), circuit.comps.end(), [&](const auto comp) {
//         std::for_each(comp.second->nodes.begin(), comp.second->nodes.end(), [&](const auto node) {
//             if (node->getId() != -1)
//                 conductance[node->getId()][node->getId()] += comp.second->conductance();
//         });
//         if (comp.second->nodes.size() == 2)
//         {
//             if (comp.second->nodes[0]->getId() != -1 && comp.second->nodes[1]->getId() != -1)
//             {
//                 conductance[comp.second->nodes[0]->getId()][comp.second->nodes[1]->getId()] -= comp.second->conductance();
//                 conductance[comp.second->nodes[1]->getId()][comp.second->nodes[0]->getId()] -= comp.second->conductance();
//             }
//         }
//     });
// }

// #endif
