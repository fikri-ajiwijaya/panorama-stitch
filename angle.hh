#ifndef ANGLE_HH
#define ANGLE_HH

#include <tuple>

float deg_rad(float const);
float rad_deg(float const);

float ang_wrap(float const);
std::tuple<float, float> ang_ran(float const, float const);
bool ang_in(float const, float const, float const);

#endif
