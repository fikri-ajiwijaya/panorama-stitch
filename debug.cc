#include <iostream>
#include <vector>
#include <string>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::vector<std::string> args(argv, argv+argc);

	for (auto const& i : args)
		std::cout << i << '\n';

	int w, h;
	unsigned char * data {stbi_load("images/brickwall.jpg", &w, &h, nullptr, 4)};
	if (!data)
		std::cout << stbi_failure_reason();
	else {
		std::cout
			<< w << "x" << h << "\n"
			<< "first: "
			<< static_cast<int>(data[0]) << " "
			<< static_cast<int>(data[1]) << " "
			<< static_cast<int>(data[2]) << " "
			<< static_cast<int>(data[3]) << "\n"
			<< "last : "
			<< static_cast<int>(data[(w*h-1)*4]) << " "
			<< static_cast<int>(data[(w*h-1)*4+1]) << " "
			<< static_cast<int>(data[(w*h-1)*4+2]) << " "
			<< static_cast<int>(data[(w*h-1)*4+3]) << "\n";

		if (!stbi_write_png("images/brickwall.png", w, h, 4, data, 0))
			std::cout << "Fail to write image to file.\n";

		stbi_image_free(data);
	}

	std::cout << "Hello, world!\n";

	return 0;
}
