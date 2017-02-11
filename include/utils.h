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

SDL_Texture* load_txt_texture(string _s,
             TTF_Font* _font, SDL_Colour _col,
             SDL_Renderer* _ren);
}
#endif
