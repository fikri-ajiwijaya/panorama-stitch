#include <exception>
#include <cmath>
#include <iostream>
#include <stdexcept>
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

	unsigned char* pixel(int x, int y) const {
		return data + y * w * 4 + x * 4;
	}
};

constexpr float pi = 3.14159265358979323846f;

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

	auto const target_filename {args[1]};
	auto const target_width {std::stoi(args[2])};
	auto const target_height {std::stoi(args[3])};

	auto const source_filename {args[4]};
	auto const source_x0 {std::stoi(args[5])};
	auto const source_y0 {std::stoi(args[6])};
	auto const source_radius {std::stoi(args[7])};
	auto const source_fov {std::stof(args[8])};
	auto const source_yaw {std::stof(args[9])};
	auto const source_pitch {std::stof(args[10])};
	auto const source_roll {std::stof(args[11])};

	image_t target_image {target_width, target_height};
	image_t const source_image {source_filename};

	std::cout
		<< "Target filename: " << target_filename << "\n"
		<< "Target width: " << target_width << "\n"
		<< "Target height: " << target_height << "\n"
		<< "\n"
		<< "Source filename: " << source_filename << "\n"
		<< "Source x0: " << source_x0 << "\n"
		<< "Source y0: " << source_y0 << "\n"
		<< "Source radius: " << source_radius << "\n"
		<< "Source FOV: " << source_fov << "\n"
		<< "Source yaw: " << source_yaw << "\n"
		<< "Source pitch: " << source_pitch << "\n"
		<< "Source roll: " << source_roll << "\n"
		<< "\n"
		<< "Target image width: " << target_image.w << "\n"
		<< "Target image height: " << target_image.h << "\n"
		<< "\n"
		<< "Source image width: " << source_image.w << "\n"
		<< "Source image height: " << source_image.h << "\n";

	int max_target_i {0};
	int min_target_i {0};
	int max_target_j {0};
	int min_target_j {0};

	int max_source_i {0};
	int min_source_i {0};
	int max_source_j {0};
	int min_source_j {0};

	float max_target_x {0.0f};
	float min_target_x {0.0f};
	float max_target_y {0.0f};
	float min_target_y {0.0f};

	float max_source_x {0.0f};
	float min_source_x {0.0f};
	float max_source_y {0.0f};
	float min_source_y {0.0f};

	float min_r {0.0f};
	float max_r {0.0f};

	for (int target_i {0}; target_i < target_image.h; ++target_i) {
		float const target_y {target_i * 2.0f / static_cast<float>(target_image.h) - 1.0f};
	for (int target_j {0}; target_j < target_image.w; ++target_j) {
		float const target_x {target_j * 2.0f / static_cast<float>(target_image.w) - 1.0f};

		// std::cout << target_i << "x" << target_j << "\n" << std::flush;

		auto const target_pixel {target_image.pixel(target_j, target_i)};

		// target_pixel[0] = target_pixel[1] = target_pixel[2] = 0x00;

		// if (target_y > 0)
		// 	if (target_x > 0)
		// 		target_pixel[0] = 0xff;
		// 	else
		// 		target_pixel[1] = 0xff;
		// else
		// 	if (target_x < 0)
		// 		target_pixel[2] = 0xff;
		// 	else
		// 		target_pixel[0] = target_pixel[1] = 0xff;

		// target_pixel[3] = 0xff;

		// target_pixel[0] = 0x00;
		// target_pixel[1] = 0x00;
		// target_pixel[2] = 0xff;
		// target_pixel[3] = 0xff;

		float const latitude {target_y * pi / 2.0f};
		float const longtitude {target_x * pi};

		float const Px {std::cos(latitude) * std::cos(longtitude)};
		float const Py {std::cos(latitude) * std::sin(longtitude)};
		float const Pz {std::sin(latitude)};

		float const r {2 * std::atan2(std::sqrt(Px * Px + Pz * Pz), Py) / (source_fov * pi / 180)};
		float const theta {std::atan2(Pz, Px)};

		float const source_x {r * std::cos(theta)};
		float const source_y {r * std::sin(theta)};

		int const source_i {static_cast<int>(source_y * source_radius + source_y0)};
		int const source_j {static_cast<int>(source_x * source_radius + source_x0)};

		if (target_i > max_target_i) max_target_i = target_i;
		if (target_i < min_target_i) min_target_i = target_i;
		if (target_j > max_target_j) max_target_j = target_j;
		if (target_j < min_target_j) min_target_j = target_j;

		if (source_i > max_source_i) max_source_i = source_i;
		if (source_i < min_source_i) min_source_i = source_i;
		if (source_j > max_source_j) max_source_j = source_j;
		if (source_j < min_source_j) min_source_j = source_j;

		if (target_x > max_target_x) max_target_x = target_x;
		if (target_x < min_target_x) min_target_x = target_x;
		if (target_y > max_target_y) max_target_y = target_y;
		if (target_y < min_target_y) min_target_y = target_y;

		if (source_x > max_source_x) max_source_x = source_x;
		if (source_x < min_source_x) min_source_x = source_x;
		if (source_y > max_source_y) max_source_y = source_y;
		if (source_y < min_source_y) min_source_y = source_y;

		if (r > max_r) max_r = r;
		if (r < min_r) min_r = r;

		if (source_i > 0 && source_i < source_image.h && source_j > 0 && source_j < source_image.w) {
			auto const source_pixel {source_image.pixel(source_j, source_i)};
			target_pixel[0] = source_pixel[0];
			target_pixel[1] = source_pixel[1];
			target_pixel[2] = source_pixel[2];
			target_pixel[3] = source_pixel[3];
		} else {
			target_pixel[0] = 0x00;
			target_pixel[1] = 0x00;
			target_pixel[2] = 0x00;
			target_pixel[3] = 0xff;
		}
	}}

	std::cout << "\n";
	std::cout << "max_target_i: " << max_target_i << "\n";
	std::cout << "min_target_i: " << min_target_i << "\n";
	std::cout << "max_target_j: " << max_target_j << "\n";
	std::cout << "min_target_j: " << min_target_j << "\n";

	std::cout << "\n";
	std::cout << "max_source_i: " << max_source_i << "\n";
	std::cout << "min_source_i: " << min_source_i << "\n";
	std::cout << "max_source_j: " << max_source_j << "\n";
	std::cout << "min_source_j: " << min_source_j << "\n";

	std::cout << "\n";
	std::cout << "max_target_x: " << max_target_x << "\n";
	std::cout << "min_target_x: " << min_target_x << "\n";
	std::cout << "max_target_y: " << max_target_y << "\n";
	std::cout << "min_target_y: " << min_target_y << "\n";

	std::cout << "\n";
	std::cout << "max_source_x: " << max_source_x << "\n";
	std::cout << "min_source_x: " << min_source_x << "\n";
	std::cout << "max_source_y: " << max_source_y << "\n";
	std::cout << "min_source_y: " << min_source_y << "\n";

	std::cout << "\n";
	std::cout << "max_r: " << max_r << "\n";
	std::cout << "min_r: " << min_r << "\n";

	target_image.write_png(target_filename);

	return 0;
}
