#include <iostream>

int main(int const argc, char const* const* const argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	for (int i {0}; i < argc; ++i)
		std::cout << argv[i] << "\n";

	std::cout << "Hello, world!\n";
	return 0;
}
