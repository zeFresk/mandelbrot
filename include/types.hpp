#pragma once
/*
  Types and definitions
  zeFresk
*/

#include <boost/multi_array.hpp>
#include <cstdint>
#include <complex>

#include "configuration.h"

// real and complex
#if defined(DOUBLE_FP) // no multiprecision supported -yet
	using real_t = double;
#elif defined(MPFR_FP)
	#include <boost/multiprecision/mpfr.hpp>
	using real_t = boost::multiprecision::number<boost::multiprecision::mpfr_float_backend<PRECISION_DIGITS> >;
#elif defined(GMP_FP)
	#include <boost/multiprecision/gmp.hpp>
	using real_t = boost::multiprecision::number<boost::multiprecision::gmp_float<PRECISION_DIGITS> >;
#elif defined(BOOST_FP)
	#include <boost/multiprecision/cpp_bin_float.hpp>
	using real_t = boost::multiprecision::number<boost::multiprecision::cpp_bin_float<PRECISION_DIGITS> >;
#else
	using real_t = float;
#endif

#if defined(CUDA_BACKEND)
	#include <thrust/complex.h>
	using complex_t = thrust::complex<real_t>;
#else
	using complex_t = std::complex<real_t>;
#endif

// pixels array
#include "pixel_array.hpp"
using pixel_array = PixelArray;

// used to compute fractal
struct Parameters {
	complex_t center;
	real_t precision;
	size_t iterations;
};