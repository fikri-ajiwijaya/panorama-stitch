#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>

std::vector<float> mat_mul(std::vector<float> const&, int, int, std::vector<float> const&, int, int);
std::vector<float> mat_min(std::vector<float> const&, int, int, int, int);
float mat_det(std::vector<float> const&, int);
std::vector<float> mat_inv(std::vector<float> const&, int);

#endif
