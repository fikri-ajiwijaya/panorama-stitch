#include "angle.hh"
#include "constant.hh"
#include <cmath>

float deg_rad(float const a) {
	return a / 180.0f * pi;
}

float rad_deg(float const a) {
	return a / pi * 180.0f;
}

float ang_wrap(float const a) {
	float x {static_cast<float>(std::fmod(a, 2 * pi))};
	if (x < 0.0f)
		x += 2 * pi;
	return x;
}

std::tuple<float, float> ang_ran(float const a_, float const b_) {
	float const a {ang_wrap(a_)};
	float const b {ang_wrap(b_)};
	if (a < b)
		return {a, b};
	else
		return {a, b + 2 * pi};
}

bool ang_in(float const t, float const a, float const b) {
	return (a <= t && t <= b) || (a <= t + 2.0f * pi && t + 2.0f * pi <= b);
}
