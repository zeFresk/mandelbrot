#pragma once
/*
  abstraction of a pixel array
  zeFresk
*/

#include <cstdint>
#include <vector>

#if defined(EXTENSIVE_DEBUG)
	#include <cassert>
#endif

#include "colors.hpp"

class PixelArray {
public: 
	// construct an array of w*h*d
	PixelArray(size_t width, size_t height, size_t depth) : 
		vec_(width*height*depth), 
		w_{width},
		h_{height},
		d_{depth}
	{}

	// non-copyable & non-movable
	PixelArray(PixelArray const&) = delete;
	PixelArray(PixelArray&&) = delete;
	PixelArray& operator=(PixelArray const&) = delete;
	PixelArray& operator=(PixelArray&&) = delete;

	// access element
	std::uint8_t& operator()(size_t x, size_t y, size_t c) {
	#if defined(EXTENSIVE_DEBUG)
		assert(x < w_ && "x too large.");
		assert(y < h_ && "y too large.");
		assert(c < d_ && "c too large.");
	#endif
		return vec_[y * w_ * d_ + x * d_ + c];
	}
	const std::uint8_t& operator()(size_t x, size_t y, size_t c) const {
	#if defined(EXTENSIVE_DEBUG)
		assert(x < w_ && "x too large.");
		assert(y < h_ && "y too large.");
		assert(c < d_ && "c too large.");
	#endif
		return vec_[y * w_ * d_ + x * d_ + c]; }

	// wrappers
	const std::uint8_t* data() const { return vec_.data(); }

private:
	std::vector<std::uint8_t> vec_;
	size_t w_;
	size_t h_;
	size_t d_;
};