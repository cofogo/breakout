//this is a header file to store various utility functions

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;

#include <SDL2/SDL_ttf.h>

namespace utils {
	struct vec2 {
		double x, y;
	};
}
#endif
