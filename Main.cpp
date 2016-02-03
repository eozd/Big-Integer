#include "Big_integer.hpp"

int main()
{
	Big_integer b{1};
	for (int i = 1; i <= 40; ++i) {
		b *= Big_integer{i};
	}
	std::cout << "Initial b: " << b << std::endl;
	for (int i = 1; i <= 20; ++i) {
		Big_integer divisor{Big_integer{i} * Big_integer{i}};
		std::cout << "Divisor: " << divisor << std::endl;
		b /= divisor;
		std::cout << "b after division: " << b << std::endl;
	}
}
