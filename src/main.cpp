#include <iostream>
#include <circuitSuper.hpp>
#include <maths/symbolic/symbolicc++.h>
int main()
{
    Symbolic x("x");
    std::cout << integrate(x+1, x);     // => 1/2*x^(2)+x
    Symbolic y("y");
    std::cout << df(y, x);              // => 0
    std::cout << df(y[x], x);           // => df(y[x],x)
    std::cout << df(exp(cos(y[x])), x); // => -sin(y[x])*df(y[x],x)*e^cos(y[x])

    Circuit::Capacitor * c = new Circuit::Capacitor("fish", -2.3 );
    return 0;
}