#include <cmath>
#include <iostream>

int main(int argc, char const *argv[])
{
    double V_POS = atof(argv[1]);
    double vGuess = 0;
    double vNew;
    double V_T = 25e-3;
    double RS = 1e-2;
    double IS = 1e-10;
    double GMIN = 1e-10;
    while (true)
    {
        vNew = V_T * std::log(((V_POS - vGuess * (1 + RS * GMIN)) / (RS * IS)) + 1);
        double error = std::pow(vGuess - vNew, 2);
        if (error < 1e-15 || std::isnan(vNew))
            break;
        vGuess = vNew;
        std::cerr << vGuess << std::endl;
    }

    if (std::isnan(vNew))
    {
        vNew = ((V_POS / RS) + IS) / ((1 / RS) + GMIN);
        std::cerr << "NAN: " << vNew << std::endl;
    }

    double iNew = IS * (exp(vNew / V_T) - 1) + vNew * GMIN;
    std::cerr << iNew << std::endl;

    return 0;
}
