#include <iostream>
#include <fstream>
#include <cmath>

int main()
{
	double init = 0.7;
	double vPos;
	std::cin >> vPos;
	double vNew;
	for (size_t i = 0; i < 100; i++) {
		vNew = 25e-3 * log(((vPos - init)/(1e-9)) + 1);
		std::cerr << vNew << '\n';
		init = vNew;
		/* code */
	}
}
