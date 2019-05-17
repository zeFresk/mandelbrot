#pragma once
/*
  openmp backend for mandelbrot computing
  zeFresk
*/

#include "types.hpp"

namespace Backend {
	// simple monothread backend for default use
	template <typename Floating_t, typename Complex_t>
	struct OpenMP {
		void compute(pixel_array& pixels, Parameters const& params, size_t width, size_t height) const {
			Complex_t start{ std::real(params.center) - params.precision * width / 2, std::imag(params.center) - params.precision * height / 2 };
			#pragma omp parallel for
			for (size_t y = 0; y < height; ++y) {
				for (size_t x = 0; x < width; ++x) {
					float norm = mandelbrot::compute_normalized(start + Complex_t{ x * params.precision, y * params.precision }, params.iterations);
					auto color = gradients_scale(norm);
					pixels[x][y][0] = color.r;
					pixels[x][y][1] = color.g;
					pixels[x][y][2] = color.b;
					pixels[x][y][3] = color.a;
				}
			}
		}
	};
}