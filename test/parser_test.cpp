#include <iostream>
#include <circuit.hpp>

int main(){
	Circuit::Schematic schem = Parser::parse( std::cin );
	schem.out();
}
