#pragma once
/*
  cuda backend (interface) for mandelbrot computing
  zeFresk
*/

#include <vector>

#include "types.hpp"
#include "colors.hpp"

// cuda functions in .cu
// Similar to mandel_math compute + normalize
std::vector<float> cuda_compute_normalize_all(std::vector<complex_t> const& c, size_t iterations);

namespace Backend {
	// simple monothread backend for default use
	template <typename Floating_t, typename Complex_t>
	struct CUDA {
		void compute(pixel_array& pixels, Parameters const& params, size_t width, size_t height) const {
			complex_t start{ (params.center.real()) - params.precision * width / 2, -(params.center.imag()) - params.precision * height / 2 };
			
			// create full 2D vector
			std::vector<Complex_t> space(width * height); 
			for (size_t y = 0; y < height; ++y) {
				for (size_t x = 0; x < width; ++x) {
					space[y*width + x] = complex_t{start.real() + x * params.precision, start.imag() + y * params.precision };
				}
			}

			// compute everything + normalize (hidden)
			std::vector<float> normalized = cuda_compute_normalize_all(space, params.iterations);

			// normalized to pixels
			for (size_t y = 0; y < height; ++y) {
				for (size_t x = 0; x < width; ++x) {
					color c = gradients_scale(normalized[y * width + x]);
					pixels(x, y, 0) = c.r;
					pixels(x, y, 1) = c.g;
					pixels(x, y, 2) = c.b;
					pixels(x, y, 3) = c.a;
				}
			}
		}
	};
}