#include <circuit.hpp>
#include <circuit_math.hpp>
#include <circuit_linear.hpp>
#include <circuit_source.hpp>
#include <algorithm>
#include <iostream>

class Math {
private:
  Circuit::Schematic schem;

public:
  Math(const Circuit::Schematic &schem) : schem(schem){}
  void run(){
    for_each(schem.sims.begin(), schem.sims.end(), [&](Circuit::Schematic::Simulation *sim){
      switch (sim.type) {
        case sim.SimulationType.OP:
          std::cerr << "OP type" << std::endl;
          break;
        case sim.SimulationType.TRAN:
          std::cerr << "TRAN" << '\n';
          break;
        case sim.SimulationType.DC:
          std::cerr << "DC" << '\n';
          break;
        case sim.SimulationType.SMALL_SIGNAL:
          std::cerr << "SMALL_SIGNAL" << '\n';
          break;
      }
    });
  }

};

int main(int argc, char const *argv[]) {
  Circuit::Schematic schem();
  Circuit::Resistor *R1 = new Circuit::Resistor("R1", 1, "N001", "0", schem);
  return 0;
}
