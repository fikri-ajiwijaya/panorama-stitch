#include "matrix.hh"

std::vector<float> mat_mul(std::vector<float> const& a, int ar, int ac, std::vector<float> const& b, int br, int bc) {
	auto c {std::vector<float>(ar * bc, 0.0f)};
	for (int i {0}; i < ar; ++i) {
	for (int j {0}; j < bc; ++j) {
	for (int k {0}; k < ac && k < br; ++k) {
		c[i * bc + j] += a[i * ac + k] * b[k * bc + j];
	}}}
	return c;
}

std::vector<float> mat_min(std::vector<float> const& a, int ar, int ac, int r, int c) {
	auto const mr {ar - 1};
	auto const mc {ac - 1};
	std::vector<float> m;
	m.resize(mr * mc);
	int mi {0};
	for (int ai {0}; ai < ar; ++ai) {
		if (ai == r)
			continue;
		int mj {0};
		for (int aj {0}; aj < ac; ++aj) {
			if (aj == c)
				continue;
			m[mi * mc + mj] = a[ai * ac + aj];
			++mj;
		}
		++mi;
	}
	return m;
}

float mat_det(std::vector<float> const& a, int an) {
	if (an == 1)
		return a[0];
	float d {0.0f};
	int s {1};
	for (int k {0}; k < an; ++k) {
		d += s * a[k] * mat_det(mat_min(a, an, an, 0, k), an - 1);
		s *= -1;
	}
	return d;
}

std::vector<float> mat_inv(std::vector<float> const& a, int an) {
	std::vector<float> a_;
	a_.resize(an * an);
	auto const d {mat_det(a, an)};
	int s {1};
	for (int i {0}; i < an; ++i) {
	for (int j {0}; j < an; ++j) {
		a_[j * an + i] = s * mat_det(mat_min(a, an, an, i, j), an - 1) / d;
		s *= -1;
	}}
	return a_;
}
