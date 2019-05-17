#pragma once
/*
  Mandelbrot color management
  zeFresk
*/

#include <cstdint>
#include <cassert>
#include <cmath>

template <class T, std::size_t N>
constexpr size_t size(const T(&array)[N]) noexcept
{
	return N;
}

struct color {
	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
	std::uint8_t a;
};

namespace gradients {
	constexpr std::array<color, 16> wikipedia = { { {0,0,0, 255}, {25, 7, 26, 255} , {9,1,47, 255}, {4,4,73, 255}, {0,7,100, 255}, {12,44,138, 255}, {24,82, 177, 255}, {57,125, 209, 255},
				{134,181, 229, 255}, {211,236,248,255}, {241, 233, 191,255}, {248, 201, 95,255}, {255,170, 0,255}, {204,128,0,255 }, {153,87,0,255}, {106,52,3,255 } } };
}

// convert value to greyscale
inline color greyscale(float x) {
	std::uint8_t v = static_cast<std::uint8_t>(static_cast<float>(255) * x);
	return { v, v, v, 255};
}

// x in [0, 1] !!
template <typename Container = std::array<color,16>>
color gradients_scale(float x, Container const& gradient = gradients::wikipedia) {
	assert(x <= 1.f && "x must be normalized !");
	if (x == 1.f)
		return { 0,0,0,255 }; //black
	size_t index = static_cast<size_t>(x * static_cast<std::size_t>(std::size(gradient)));
	return gradient[index];
}
