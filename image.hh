#ifndef IMAGE_HH
#define IMAGE_HH

#include <initializer_list>

struct image_t {
	int w;
	int h;
	float* data;

	image_t(char const* const);
	image_t(int const, int const);
	image_t(image_t const&) = delete;
	image_t(image_t&&);
	~image_t();

	void write_png(char const*) const;

	struct pixel_t {
		float& r;
		float& g;
		float& b;
		float& a;

		pixel_t(float* const);
		pixel_t& operator=(pixel_t const&);
		pixel_t& operator=(std::initializer_list<float const> const);
	};

	pixel_t operator()(int const, int const);
};

#endif
