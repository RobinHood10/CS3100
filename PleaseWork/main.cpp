#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

#include "compute-e.hpp"
#include "compute-fib.hpp"
#include "compute-pi.hpp"

std::string print_error_message();

int main(int argc, char* argv[]) {
	if (argc < 2)
	{
		std::cout << print_error_message();
		exit(0);
	}

	int v1 = 1;
	int v2 = 2;
	//	argc--;

	while (argc - 2 >= 0) {
		std::string input = argv[v1];
		int n = std::atoi(argv[v2]);

		if (input == "-fib") {
			std::cout << fib(n) << std::endl;
			exit(0);
		}
		if (input == "-pi") {
			std::cout << pi(n) << std::endl;
			exit(0);
		}
		if (input == "-e") {
			std::cout << e(n) << std::endl;
			exit(0);
		}
		else {
			std::cout << print_error_message();
			exit(0);
		}
		//		argc -= 2;
		//		v1 += 2;
		//		v2 += 2;
	}
}

std::string print_error_message() {
	//returns help message in case of invalid input
	std::stringstream ss;
	ss << std::endl;
	ss << "--- Assignment 1 Help ---" << std::endl;
	ss << "-fib [n] : Compute the [n]th Fibonacci number" << std::endl;
	ss << "-e [n] : Compute the value of 'e' using [n] iterations" << std::endl;
	ss << "-pi [n] : Compute pi to [n] digits" << std::endl;
	ss << std::endl;
	return ss.str();
}
