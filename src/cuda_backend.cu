/*
  cuda backend for mandelbrot computing
  zeFresk
*/

#include <vector>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/complex.h>

#include "types.hpp"

// give f_n(c) where f is mandelbrot function and c is a complex point
struct zn_functor {
	const size_t iterations;
	zn_functor(size_t iters) : iterations{iters} {}

	__device__
	float operator()(complex_t& z, complex_t const& c) {
		for (size_t i = 0; i < iterations; ++i) {
			if (thrust::abs(z) > static_cast<real_t>(2.f))
				return static_cast<float>(i) / static_cast<float>(iterations);
			z *= z;
			z += c;
		}
		return 1.f;
	}
};

std::vector<float> cuda_compute_normalize_all(std::vector<complex_t> const& vec, size_t iterations) {
	thrust::device_vector<complex_t> c(vec.size());
	thrust::copy(vec.begin(), vec.end(), c.begin());
	thrust::device_vector<complex_t> zn = c; // 0+0i +c array,  z1 not z0 !
	thrust::device_vector<float> normalized(vec.size());

	// z*z + c iterations times + normalize
	thrust::transform(zn.begin(), zn.end(), c.begin(), normalized.begin(), zn_functor(iterations-1));
	
	// copy normalized values
	std::vector<float> ret(vec.size());
	thrust::host_vector<float> hret = normalized;
	thrust::copy(hret.begin(), hret.end(), ret.begin());
	return ret;
}