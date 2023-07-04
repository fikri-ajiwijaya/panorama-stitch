#include <cmath>
#include <iostream>
#include "image.hh"
#include "angle.hh"
#include "constant.hh"

int main(int const argc, char const* const* const argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	if (argc != 7) {
		std::cout
			<< "Incorrect number of arguments.\n"
			<< "\n"
			<< "usage:\n"
			<< "\tTARGET SOURCE latitude latitude longtitude longtitude\n";
		return -1;
	}

	char const* const dst_file {argv[1]};
	char const* const src_file {argv[2]};

	auto const [p1, p2] {ang_ran(deg_rad(std::stof(argv[3])), deg_rad(std::stof(argv[4])))};
	auto const [l1, l2] {ang_ran(deg_rad(std::stof(argv[5])), deg_rad(std::stof(argv[6])))};

	image_t src_img {src_file};

	float const w {static_cast<float>(src_img.w)};
	float const h {static_cast<float>(src_img.h)};

	image_t dst_img {static_cast<int>(w), static_cast<int>(h)};

	for (float x {0}; x < w; ++x) {
	for (float y {0}; y < h; ++y) {
		float const p {ang_wrap(pi * (0.5f - y / h))};
		float const l {ang_wrap(2.0f * pi * (x / w - 0.5f))};

		if (ang_in(p, p1, p2) && ang_in(l, l1, l2))
			dst_img(x, y) = src_img(x, y);
		else
			dst_img(x, y) = {0.0f, 0.0f, 0.0f, 0.0f};
	}}

	dst_img.write_png(dst_file);

	return 0;
}
