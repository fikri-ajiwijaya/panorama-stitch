#include "image.hh"
#include <stdexcept>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

image_t::image_t(char const* const file) {
	auto const img {stbi_load(file, &w, &h, nullptr, STBI_rgb_alpha)};
	if (!img)
		throw std::runtime_error(stbi_failure_reason());
	data = new float[w * h * 4];
	for (int i {0}; i < w * h * 4; ++i)
		data[i] = img[i] / 255.0f;
	stbi_image_free(img);
}

image_t::image_t(int const _w, int const _h)
	: w {_w}, h {_h}, data {new float[w * h * 4]}
{}

image_t::image_t(image_t&& mv)
	:	w {mv.w}, h {mv.h}, data {mv.data}
{
	mv.w = 0;
	mv.h = 0;
	mv.data = nullptr;
}

image_t::~image_t() {
	delete[] data;
}

void image_t::write_png(char const* file) const {
	auto const img {new unsigned char[w * h * 4]};
	for (int i {0}; i < w * h * 4; ++i)
		img[i] = data[i] * 255.0f;
	auto const ok {stbi_write_png(file, w, h, STBI_rgb_alpha, img, 0)};
	delete[] img;
	if (!ok)
		throw std::runtime_error(file);
}

image_t::pixel_t::pixel_t(float* const loc)
	: r {loc[0]}, g {loc[1]}, b {loc[2]}, a {loc[3]}
{}

image_t::pixel_t& image_t::pixel_t::operator=(pixel_t const& p) {
	r = p.r;
	g = p.g;
	b = p.b;
	a = p.a;
	return *this;
}

image_t::pixel_t& image_t::pixel_t::operator=(std::initializer_list<float const> const p) {
	r = *(p.begin()+0);
	g = *(p.begin()+1);
	b = *(p.begin()+2);
	a = *(p.begin()+3);
	return *this;
}

image_t::pixel_t image_t::operator()(int const x, int const y) const {
	return data + (y * w + x) * 4;
}
