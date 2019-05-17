#pragma once
/*
  Command line arguments parsing functions
  zeFresk
*/

#include <string>
#include <boost/multiprecision>

// final sdtructure containing parsed options
struct options_raw {
	unsigned window_width;
	unsigned window_height;
	boost:resolution;
	std::string center;
	unsigned precision;
};