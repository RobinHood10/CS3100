#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

#include "compute-fib.hpp"

int fib(int n) {
	//returns the nth fibonacci number
	int fib1 = 1;
	int fib2 = 1;

	if (n < 0 || n > 40) {
		std::cout << n << " is out of range for fib. Please enter a number from 0 to 40.\n";
		return -1;
	}
	//	if (n == 0)
	//		return 0;
	if (n == 1 || n == 0)
		return 1;
	else {
		int next_fib = 0;
		for (int i = 1; i < n; i++) {
			next_fib = fib1 + fib2;
			fib1 = fib2;
			fib2 = next_fib;
		}
		return next_fib;
	}
}