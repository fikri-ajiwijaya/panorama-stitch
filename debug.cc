#include <iostream>
#include "image.hh"

bool str_cmp(char const* const a, char const* const b) {
	for (int i {0}, j {0}; a[i] != '\0' && b[j] != '\0'; ++i, ++j)
		if (a[i] != b[j])
			return false;
	return true;
}

int main(int const argc, char const* const* const argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	if (argc > 1 && str_cmp(argv[1], "white")) {
		std::cout << "Generating images/white.png.\n" << std::flush;
		image_t img {2048, 1024};
		for (int x {0}; x < img.w; ++x) {
		for (int y {0}; y < img.h; ++y) {
			img(x, y) = {1.0f, 1.0f, 1.0f, 1.0f};
		}}
		img.write_png("images/white.png");
	}

	for (int i {0}; i < argc; ++i)
		std::cout << argv[i] << "\n";

	std::cout << "Hello, world!\n";
	return 0;
}
