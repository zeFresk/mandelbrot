#pragma once
/*
  Mandelbrot math related functions
  zeFresk
*/

#include <complex>

namespace mandelbrot {
	//	###############################
	//	### normalization functions ###
	//	###############################
	
	// NOTE: All the following takes the index at which we diverged and the maxium number of iterations used along with last computed value. They then returns a real number
	
	// identity function, returns index as is
	template <typename Floating_Type>
	float identity(size_t index, size_t max_iterations, std::complex<Floating_Type> const& last) {
		return static_cast<float>(index);
	}

	// Smooth coloring function, as in https://www.codingame.com/playgrounds/2358/how-to-plot-the-mandelbrot-set/adding-some-colors
	template <typename Floating_Type>
	inline float smooth_coloring(size_t index, size_t max_iterations, std::complex<Floating_Type> const& last) {
		if (index == max_iterations)
			return static_cast<float>(index);
		return index + 1 - std::log(std::log2(std::abs(z)))
	}

	// simply normalize output between 0 and 1
	template <typename Floating_Type>
	inline float one(size_t index, size_t max_iterations, std::complex<Floating_Type> const& last) {
		if (index == max_iterations)
			return 1.f;
		return static_cast<float>(index) / static_cast<float>(max_iterations);
	}

	// Trait to get associated normalized function type given Floating_type
	template <typename Floating_Type>
	using normalized_function_type = float (*)(size_t, size_t, std::complex<Floating_Type> const&);

	//	###############################
	//	#### computation functions ####
	//	###############################

	// Iterate through mandelbrot function (zn=zn-1+c) up to 'iterations' times.
	// Returns iteration at which we diverged.
	template <typename Floating_Type>
	size_t compute(std::complex<Floating_Type> const& c, size_t iterations) {
		// optimizations
		// detection of cardioid
		decltype(c) z = (std::real(c) - 0.25) * (std::real(c) - 0.25) + std::imag(c) * std::imag(c);
		if (z * (z + std::real(c) - 0.25) <= 0.25 * std::imag(c) * std::imag(c)) { // inside cardioid
			return iterations;
		}

		// main computation
		z = 0.;
		for (size_t i = 0; i < iterations, ++i) {
			z = z * z + c;
			if (std::abs(z) > 2)
				return i;
		}
		return iterations;
	}

	// Iterate through mandelbrot function but normalize output with the f function
	// see mandelbrot::compute
	template <typename Floating_Type>
	float compute_normalized(std::complex<Floating_Type> const& c, size_t iterations, normalized_function_type<Floating_Type> f = one) {
		// optimizations
		// detection of cardioid
		Floating_Type q = (std::real(c) - 0.25) * (std::real(c) - 0.25) + std::imag(c) * std::imag(c);
		if (q * (q +  std::real(c) - 0.25 ) <= 0.25 * std::imag(c) * std::imag(c)) { // inside cardioid
			return f(iterations, iterations, complex_t{ 0,0 });
		}

		// main computation
		auto z = complex_t{ 0,0 };
		for (size_t i = 0; i < iterations; ++i) {
			z = z * z + c;
			if (std::abs(z) > 2)
				return f(i, iterations, z);
		}
		return f(iterations, iterations, z);
	}
}