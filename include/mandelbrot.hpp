#pragma once
/*
  Mandelbrot fractal representation
  zeFresk
*/

#include "types.hpp"
#include "mandelmaths.hpp"
#include "colors.hpp"
#include "std_backend.hpp"

// Class used to represent and compute efficiently the mandelbrot fractral
// Backend contains the compute function
template <typename Floating_Type, typename Complex_Type, typename Backend = Backend::Monothread<Floating_Type, Complex_Type>>
class Mandelbrot_representation {
public:
	// create an instance using given parameters and space size pixelwise
	Mandelbrot_representation(Parameters const& params, size_t width, size_t height) : 
		params_{ params }, 
		backend_{}, 
		pixels_{ boost::extents[width][height][4], pixels_order(ordering, ascending) },
		w_{ width }, 
		h_{ height }  {	}

	// non-copiable & non-movable
	Mandelbrot_representation(Mandelbrot_representation const&) = delete;
	Mandelbrot_representation(Mandelbrot_representation&&) = delete;
	Mandelbrot_representation& operator=(Mandelbrot_representation const&) = delete;
	Mandelbrot_representation& operator=(Mandelbrot_representation&&) = delete;

	// compute all values
	void compute() {
		backend_.compute(pixels_, params_, w_, h_);
	}


	// return underlying pixels
	const std::uint8_t* raw_pixels() const {
		return pixels_.data();
	}

private:
	Parameters const& params_;
	Backend backend_;
	pixel_array pixels_;
	size_t w_;
	size_t h_;
};

#if defined(_OPENMP)
#include "openmp_backend.hpp"
using Mandelbrot = Mandelbrot_representation<real_t, complex_t, Backend::OpenMP<real_t, complex_t>>;
#else
using Mandelbrot = Mandelbrot_representation<real_t, complex_t, Backend::Monothread<real_t, complex_t>>;
#endif