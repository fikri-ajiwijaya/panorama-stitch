#include <cmath>
#include <iostream>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

struct image_t {
	int w, h;
	unsigned char* data;

	image_t(char const* const filename) {
		data = stbi_load(filename, &w, &h, nullptr, STBI_rgb_alpha);
		if (!data)
			throw std::runtime_error(stbi_failure_reason());
	}

	image_t(int const _w, int const _h) : w {_w}, h {_h}, data {new unsigned char[h*w*4]} {}

	~image_t() { stbi_image_free(data); }

	void write_png(char const* filename) const {
		auto const success {stbi_write_png(filename, w, h, STBI_rgb_alpha, data, 0)};
		if (!success)
			throw std::runtime_error(filename);
	}

	unsigned char* px(int const x, int const y) const { return data + (y * w + x) * 4; }
};

float constexpr pi {3.14159265358979323846f};

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

	char const* const dst_filename {argv[1]};
	float const dst_w {static_cast<float>(std::stoi(argv[2]))};
	float const dst_h {static_cast<float>(std::stoi(argv[3]))};

	char const* const src_filename {argv[4]};
	float const src_x0 {std::stof(argv[5])};
	float const src_y0 {std::stof(argv[6])};
	float const src_r {std::stof(argv[7])};
	float const src_fov {std::stof(argv[8]) * pi / 180.0f};
	float const src_ry {std::stof(argv[9]) * pi / 180.0f};
	float const src_rp {std::stof(argv[10]) * pi / 180.0f};
	float const src_rr {std::stof(argv[11]) * pi / 180.0f};

	image_t dst_img {static_cast<int>(dst_w), static_cast<int>(dst_h)};
	image_t src_img {src_filename};

	float const src_w {static_cast<float>(src_img.w)};
	float const src_h {static_cast<float>(src_img.h)};

	auto const t {([](float const y, float const p, float const r) {
		float const cy {std::cos(y)}; float const sy {std::sin(y)};
		float const cp {std::cos(p)}; float const sp {std::sin(p)};
		float const cr {std::cos(r)}; float const sr {std::sin(r)};
		return new float const[] {
			cy*cp, -cy*sp*sr-sy*cr, -cy*sp*cr+sy*sr,
			sy*cp, -sy*sp*sr+cy*cr, -sy*sp*cr-cy*sr,
			   sp,           cp*sr,           cp*cr
		};
	})(-src_ry, -src_rp, -src_rr)};

	for (float dst_x {0}; dst_x < dst_w; ++dst_x) {
	for (float dst_y {0}; dst_y < dst_h; ++dst_y) {
		float const p {(0.5f - dst_y / dst_h) * pi};
		float const l {2.0f * pi * (dst_x / dst_w - 0.5f)};

		float const cp {std::cos(p)}; float const sp {std::sin(p)};
		float const cl {std::cos(l)}; float const sl {std::sin(l)};

		float const x {t[0]*cp*cl+t[1]*cp*sl+t[2]*sp};
		float const y {t[3]*cp*cl+t[4]*cp*sl+t[5]*sp};
		float const z {t[6]*cp*cl+t[7]*cp*sl+t[8]*sp};

		float const r {2.0f * std::acos(x) / src_fov};
		float const t {std::atan2(z, y)};

		float const src_x {src_x0 + src_r * r * std::cos(t)};
		float const src_y {src_y0 - src_r * r * std::sin(t)};

		auto dst_px {dst_img.px(dst_x, dst_y)};
		auto src_px {src_img.px(src_x, src_y)};

		if (r <= 1.0f && src_x > 0 && src_x < src_w && src_y > 0 && src_y < src_h) {
			dst_px[0] = src_px[0];
			dst_px[1] = src_px[1];
			dst_px[2] = src_px[2];
			dst_px[3] = src_px[3];
		} else {
			dst_px[0] = 0x00;
			dst_px[1] = 0x00;
			dst_px[2] = 0x00;
			dst_px[3] = 0x00;
		}
	}}

	dst_img.write_png(dst_filename);

	return 0;
}
