#include <vector>
#include <iostream>
#include "image.hh"

int main(int const argc, char const* const* const argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	if (argc < 3) {
		std::cout
			<< "Incorrect number of arguments.\n"
			<< "\n"
			<< "usage:\n"
			<< "\tTARGET SOURCE ...\n";
		return -1;
	}

	char const* const dst_file {argv[1]};

	std::vector<image_t> src_img;
	src_img.reserve(argc - 2);
	for (int i {2}; i < argc; ++i)
		src_img.push_back(image_t{argv[i]});

	float const w {static_cast<float>(src_img[0].w)};
	float const h {static_cast<float>(src_img[0].h)};

	image_t dst_img {static_cast<int>(w), static_cast<int>(h)};

	for (int x {0}; x < w; ++x) {
	for (int y {0}; y < h; ++y) {
		auto px {dst_img(x, y)};
		px = {0.0f, 0.0f, 0.0f, 0.0f};
		for (auto const& i : src_img) {
			auto i_px {i(x, y)};
			px.r += i_px.r;
			px.g += i_px.g;
			px.b += i_px.b;
			px.a += i_px.a;
		}
		if (px.a > 0.0f) {
			px.r /= px.a;
			px.g /= px.a;
			px.b /= px.a;
			px.a = 1.0f;
		}
	}}

	dst_img.write_png(dst_file);

	return 0;
}
