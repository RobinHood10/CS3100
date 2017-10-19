#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

#include "compute-pi.hpp"

std::string pi(int n) {
	//creturns pi to n digits--truncating, not rounding
	if (n < 0 || n > 10) {
		std::cout << n << " is out of range for pi. Please enter a number from 0 to 10.\n";
		return "-1";
	}
	char pi10[10] = { '1','4','1','5','9','2','6','5','3','5' };
	std::string pi = "3.";
	for (int i = 0; i < n; i++) {
		pi += pi10[i];
	}

	return pi;
}