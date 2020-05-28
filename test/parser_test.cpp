#include <iostream>
#include <circuit.hpp>

int main(){
	std::cout<<"fish"<<std::endl;
	Circuit::Schematic* schem = Circuit::Parser::parse( std::cin );
	schem->out();
}
