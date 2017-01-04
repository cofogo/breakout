#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Object.h"

class Paddle: public Object
{
public:
	Paddle(SDL_Rect _rect, const int _s);
	~Paddle();
	
	void move_l();
	void move_r();
	void set_speed(const int _amt);
	
private:
	int m_speed;
};

#endif //PADDLE_H
