#include <iostream>

#include <circuit.hpp>

int main()
{
    Symbolic x("x");
    std::cout << integrate(x + 1, x) << std::endl; // => 1/2*x^(2)+x
    Symbolic y("y");
    std::cout << df(y, x) << std::endl;              // => 0
    std::cout << df(y[x], x) << std::endl;           // => df(y[x],x)
    std::cout << df(exp(cos(y[x])), x) << std::endl; // => -sin(y[x])*df(y[x],x)*e^cos(y[x])

    return 0;
}