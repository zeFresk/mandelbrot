#pragma once
/*
  Types and definitions
  zeFresk
*/

#include <boost/multiprecision/mpfr.hpp>
#include <boost/multi_array.hpp>
#include <cstdint>
#include <complex>

// real and complex
using real_t = double;//boost::multiprecision::mpfr_float_50;
using complex_t = std::complex<real_t>;

// pixels array
using pixel_array = boost::multi_array<std::uint8_t, 3>; // 3D array
using pixels_order = boost::general_storage_order<3>;
constexpr pixel_array::size_type ordering[] = { 2,0,1 };
constexpr bool ascending[] = { true, false, true };

// used to compute fractal
struct Parameters {
	complex_t center;
	real_t precision;
	size_t iterations;
};