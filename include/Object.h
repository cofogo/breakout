#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
using std::cerr;
using std::endl;
#include <memory>
using std::shared_ptr;

#include "SDL2/SDL.h"

class Object
{
public:
	Object(SDL_Rect _rect);
	~Object();
	
	int assign_texture(shared_ptr<SDL_Texture> _tex);
	SDL_Texture* get_texture();
	void render(SDL_Renderer* _ren);
	SDL_Rect* get_rect();
	
protected:
	SDL_Rect m_rect;
	
private:
	shared_ptr<SDL_Texture> m_texture;
};

#endif //OBJECT_H
