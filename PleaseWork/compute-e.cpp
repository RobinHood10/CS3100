#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

#include "compute-e.hpp"

double e(int n) {

	if (n < 0 || n > 30) {
		std::cout << n << " is out of range for e. Please enter a number from 0 to 30.\n";
		return -1;
	}

	double e = 0;
	for (int i = n; i >= 0; i--) {
		e += (1.0 / factorial(i));
	}
	return e;
}

int factorial(int n) {
	//return n!
	//	if (n == 0)
	//		return 1;
	int factorial = 1;
	for (int i = 1; i <= n; i++) {
		factorial *= i;
	}
	return factorial;
}