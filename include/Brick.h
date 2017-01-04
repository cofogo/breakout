#ifndef BRICK_H
#define BRICK_H

#include <SDL2/SDL.h>

#include <Object.h>

class Brick: public Object
{
public:
	Brick(SDL_Rect _rect);
	~Brick();
};

#endif //BRICK_H
