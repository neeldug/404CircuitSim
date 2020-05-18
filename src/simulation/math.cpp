#include <circuitSuper.hpp>
#include <math/symbolicc++.h>

void generateMatrices(const Ciruit::Schematic &circuit){
    const int NUM_NODES = circuit.nodes.size();
    Symbolic currentVector('currentVector', NUM_NODES);
    Symbolic voltageVector('voltageVector', NUM_NODES);
    Symbolic conductanceMatrix('conductanceMatrix', NUM_NODES, NUM_NODES);
}