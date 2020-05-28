#include <iostream>
#include <circuit.hpp>

int main(){
	Circuit::Schematic* schem = Circuit::Parser::parse( std::cin );
	schem->out();
}
