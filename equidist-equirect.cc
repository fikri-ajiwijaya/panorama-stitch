#include <cmath>
#include <iostream>
#include "image.hh"
#include "angle.hh"
#include "constant.hh"
#include "matrix.hh"

int main(int const argc, char const* const* const argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	if (argc != 12) {
		std::cout
			<< "Incorrect number of arguments.\n"
			<< "\n"
			<< "usage:\n"
			<< "\tTARGET width height SOURCE x0 y0 radius fov yaw pitch roll\n";
		return -1;
	}

	char const* const dst_file {argv[1]};
	float const dst_w {static_cast<float>(std::stoi(argv[2]))};
	float const dst_h {static_cast<float>(std::stoi(argv[3]))};

	char const* const src_file {argv[4]};
	float const src_x0 {std::stof(argv[5])};
	float const src_y0 {std::stof(argv[6])};
	float const src_r {std::stof(argv[7])};
	float const src_fov {deg_rad(std::stof(argv[8]))};
	float const src_ry {deg_rad(std::stof(argv[9]))};
	float const src_rp {deg_rad(std::stof(argv[10]))};
	float const src_rr {deg_rad(std::stof(argv[11]))};

	image_t dst_img {static_cast<int>(dst_w), static_cast<int>(dst_h)};
	image_t src_img {src_file};

	float const src_w {static_cast<float>(src_img.w)};
	float const src_h {static_cast<float>(src_img.h)};

	std::vector<float> const my {
		std::cos(src_ry), -std::sin(src_ry), 0.0f,
		std::sin(src_ry),  std::cos(src_ry), 0.0f,
		            0.0f,              0.0f, 1.0f
	};
	std::vector<float> const mp {
		std::cos(src_rp), 0.0f, -std::sin(src_rp),
		            0.0f, 1.0f,              0.0f,
		std::sin(src_rp), 0.0f,  std::cos(src_rp)
	};
	std::vector<float> const mr {
		 1.0f,             0.0f,              0.0f,
		 0.0f, std::cos(src_rr), -std::sin(src_rr),
		 0.0f, std::sin(src_rr),  std::cos(src_rr)
	};
	auto const m {mat_mul(my, 3, 3, mat_mul(mp, 3, 3, mr, 3, 3), 3, 3)};
	auto const m_ {mat_inv(m, 3)};

	for (float dst_x {0}; dst_x < dst_w; ++dst_x) {
	for (float dst_y {0}; dst_y < dst_h; ++dst_y) {
		float const p {pi * (0.5f - dst_y / dst_h)};
		float const l {2.0f * pi * (dst_x / dst_w - 0.5f)};

		std::vector<float> const g {
			std::cos(p) * std::cos(l),
			std::cos(p) * std::sin(l),
			std::sin(p)
		};
		std::vector<float> const g_ {mat_mul(m_, 3, 3, g, 3, 1)};

		float const r {2.0f * std::acos(g_[0]) / src_fov};
		float const t {std::atan2(g_[2], g_[1])};

		float const src_x {src_x0 + src_r * r * std::cos(t)};
		float const src_y {src_y0 - src_r * r * std::sin(t)};

		if (r <= 1.0f && src_x > 0 && src_x < src_w && src_y > 0 && src_y < src_h)
			dst_img(dst_x, dst_y) = src_img(src_x, src_y);
		else
			dst_img(dst_x, dst_y) = {0.0f, 0.0f, 0.0f, 0.0f};
	}}

	dst_img.write_png(dst_file);

	return 0;
}
