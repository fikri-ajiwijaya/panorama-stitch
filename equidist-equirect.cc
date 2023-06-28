#include <exception>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

struct image_t {
	int w;
	int h;
	unsigned char* data;

	image_t(std::string const& filename) {
		data = stbi_load(filename.c_str(), &w, &h, nullptr, STBI_rgb_alpha);
		if (!data)
			throw std::runtime_error(stbi_failure_reason());
	}

	image_t(int _w, int _h)
		: w {_w}, h {_h}, data {new unsigned char[h*w*4]}
	{}

	~image_t() {
		stbi_image_free(data);
	}

	void write_png(std::string const& filename) const {
		stbi_write_png(filename.c_str(), w, h, STBI_rgb_alpha, data, 0);
	}

	struct pixel_t {
		unsigned char& r;
		unsigned char& g;
		unsigned char& b;
		unsigned char& a;

		pixel_t(unsigned char* loc)
			: r {loc[0]}, g {loc[1]}, b {loc[2]}, a {loc[3]}
		{}

		pixel_t& operator=(pixel_t const& p) {
			r = p.r;
			g = p.g;
			b = p.b;
			a = p.a;
			return *this;
		}

		pixel_t& operator=(std::initializer_list<unsigned char> p) {
			r = *(p.begin()+0);
			g = *(p.begin()+1);
			b = *(p.begin()+2);
			a = *(p.begin()+3);
			return *this;
		}
	};

	pixel_t pixel(int x, int y) const {
		return pixel_t(data + (y * w + x) * 4);
	}
};

float constexpr pi {3.14159265358979323846f};

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::vector<std::string> const args {argv, argv+argc};

	if (args.size() != 12) {
		std::cout
			<< "Incorrect number of arguments.\n"
			<< "\n"
			<< "usage:\n"
			<< "\tTARGET width height SOURCE x0 y0 radius fov yaw pitch roll\n";
		return -1;
	}

	std::string const dst_filename {args[1]};
	float const dst_w {static_cast<float>(std::stoi(args[2]))};
	float const dst_h {static_cast<float>(std::stoi(args[3]))};

	std::string const src_filename {args[4]};
	float const src_x0 {std::stof(args[5])};
	float const src_y0 {std::stof(args[6])};
	float const src_r {std::stof(args[7])};
	float const src_fov {std::stof(args[8]) * pi / 180.0f};
	float const src_xy {std::stof(args[9]) * pi / 180.0f};
	float const src_xz {std::stof(args[10]) * pi / 180.0f};
	float const src_yz {std::stof(args[11]) * pi / 180.0f};

	image_t dst_img {static_cast<int>(dst_w), static_cast<int>(dst_h)};
	image_t const src_img {src_filename};

	float const src_w {static_cast<float>(src_img.w)};
	float const src_h {static_cast<float>(src_img.h)};

	float min_dst_x {std::numeric_limits<float>::infinity()}, max_dst_x {-std::numeric_limits<float>::infinity()};
	float min_dst_y {std::numeric_limits<float>::infinity()}, max_dst_y {-std::numeric_limits<float>::infinity()};
	float min_p {std::numeric_limits<float>::infinity()}, max_p {-std::numeric_limits<float>::infinity()};
	float min_l {std::numeric_limits<float>::infinity()}, max_l {-std::numeric_limits<float>::infinity()};
	float min_x {std::numeric_limits<float>::infinity()}, max_x {-std::numeric_limits<float>::infinity()};
	float min_y {std::numeric_limits<float>::infinity()}, max_y {-std::numeric_limits<float>::infinity()};
	float min_z {std::numeric_limits<float>::infinity()}, max_z {-std::numeric_limits<float>::infinity()};
	float min_r {std::numeric_limits<float>::infinity()}, max_r {-std::numeric_limits<float>::infinity()};
	float min_t {std::numeric_limits<float>::infinity()}, max_t {-std::numeric_limits<float>::infinity()};
	float min_src_x {std::numeric_limits<float>::infinity()}, max_src_x {-std::numeric_limits<float>::infinity()};
	float min_src_y {std::numeric_limits<float>::infinity()}, max_src_y {-std::numeric_limits<float>::infinity()};

	for (float dst_x {0}; dst_x < dst_w; ++dst_x) {
	for (float dst_y {0}; dst_y < dst_h; ++dst_y) {
		float const p {pi * (dst_y / dst_h - 0.5f)};
		float const l {2.0f * pi * (dst_x / dst_w - 0.5f)};

		float x {std::cos(p) * std::cos(l)};
		float y {std::cos(p) * std::sin(l)};
		float z {std::sin(p)};

		y = std::cos(src_yz) * y - std::sin(src_yz) * z;
		z = std::sin(src_yz) * y + std::cos(src_yz) * z;

		x = std::cos(src_xz) * x - std::sin(src_xz) * z;
		z = std::sin(src_xz) * x + std::cos(src_xz) * z;

		x = std::cos(src_xy) * x - std::sin(src_xy) * y;
		y = std::sin(src_xy) * x + std::cos(src_xy) * y;

		float const r {std::acos(x) / src_fov * 2.0f};
		float const t {std::atan2(z, y)};

		float const src_x {src_x0 + src_r * r * std::cos(t)};
		float const src_y {src_y0 + src_r * r * std::sin(t)};

		if (
			src_x > 0 && src_x < src_w && src_y > 0 && src_y < src_h
			&& std::sqrt((src_x - src_x0) * (src_x - src_x0) + (src_y - src_y0) * (src_y - src_y0)) <= src_r
		)
			dst_img.pixel(dst_x, dst_y) = src_img.pixel(src_x, src_y);
		else
			dst_img.pixel(dst_x, dst_y) = {0x00, 0x00, 0x00, 0x00};

		if (dst_x < min_dst_x) min_dst_x = dst_x; if (dst_x > max_dst_x) max_dst_x = dst_x;
		if (dst_y < min_dst_y) min_dst_y = dst_y; if (dst_y > max_dst_y) max_dst_y = dst_y;
		if (p < min_p) min_p = p; if (p > max_p) max_p = p;
		if (l < min_l) min_l = l; if (l > max_l) max_l = l;
		if (x < min_x) min_x = x; if (x > max_x) max_x = x;
		if (y < min_y) min_y = y; if (y > max_y) max_y = y;
		if (z < min_z) min_z = z; if (z > max_z) max_z = z;
		if (r < min_r) min_r = r; if (r > max_r) max_r = r;
		if (t < min_t) min_t = t; if (t > max_t) max_t = t;
		if (src_x < min_src_x) min_src_x = src_x; if (src_x > max_src_x) max_src_x = src_x;
		if (src_y < min_src_y) min_src_y = src_y; if (src_y > max_src_y) max_src_y = src_y;
	}}

	std::cout << "dst_x: " << min_dst_x << " " << max_dst_x << "\n";
	std::cout << "dst_y: " << min_dst_y << " " << max_dst_y << "\n";
	std::cout << "p: " << min_p << " " << max_p << "\n";
	std::cout << "l: " << min_l << " " << max_l << "\n";
	std::cout << "x: " << min_x << " " << max_x << "\n";
	std::cout << "y: " << min_y << " " << max_y << "\n";
	std::cout << "z: " << min_z << " " << max_z << "\n";
	std::cout << "r: " << min_r << " " << max_r << "\n";
	std::cout << "t: " << min_t << " " << max_t << "\n";
	std::cout << "src_x: " << min_src_x << " " << max_src_x << "\n";
	std::cout << "src_y: " << min_src_y << " " << max_src_y << "\n";

	dst_img.write_png(dst_filename);

	return 0;
}
