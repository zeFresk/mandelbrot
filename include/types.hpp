#pragma once
/*
  Types and definitions
  zeFresk
*/

#include <boost/multiprecision/mpfr.hpp>
#include <complex>

using real_t = boost::multiprecision::mpfr_float_50;
using complex_t = std::complex<real_t>;

// used to compute fractal
struct Parameters {
	complex_t center;
	real_t precision;
};